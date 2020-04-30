#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <curl/curl.h>

#include "monikor.h"

#define MAX_HEADER_LENGTH 1024
#define RESPONSE_BODY_SIZE 1024


typedef struct {
  monikor_t *mon;
  monikor_metric_list_t *metrics;
  struct curl_slist *headers;
  char response[RESPONSE_BODY_SIZE + 1];
  char error[CURL_ERROR_SIZE];
  void *data;
  size_t data_size;
} send_data_t;


static size_t data_handler(char *ptr, size_t size, size_t nmemb, void *userdata) {
  char *data = userdata;

  strncpy(data, ptr, RESPONSE_BODY_SIZE > size * nmemb ? size * nmemb : RESPONSE_BODY_SIZE);
  data[RESPONSE_BODY_SIZE > size * nmemb ? size * nmemb : RESPONSE_BODY_SIZE] = 0;
  return size * nmemb;
}

static struct curl_slist *set_http_headers(monikor_t *mon) {
  struct curl_slist *headers = NULL;
  char hdr[MAX_HEADER_LENGTH];
  char hostname[256];

  if (mon->config->hostname) {
    strncpy(hostname, mon->config->hostname, 255);
  } else if (gethostname(hostname, 255)) {
    monikor_log(LOG_ERR, "Canot get hostname: %s\n", strerror(errno));
    return NULL;
  }
  hostname[255] = 0;
  if (!(headers = curl_slist_append(headers, "Content-Type: application/octet-stream")))
    return NULL;
  snprintf(hdr, MAX_HEADER_LENGTH, "Authorization: Token %s",
    mon->config->quanta_token ? mon->config->quanta_token : "unknown");
  if (!(headers = curl_slist_append(headers, hdr)))
    return NULL;
  snprintf(hdr, MAX_HEADER_LENGTH, "X-Quanta-Hostid: %s %s %s",
    mon->config->hostid ? mon->config->hostid : "unknown", hostname, MONIKOR_VERSION);
  if (!(headers = curl_slist_append(headers, hdr)))
    return NULL;
  return headers;
}

static void setup_curl_request(monikor_curl_handler_t *handler) {
  send_data_t *data = (send_data_t *)handler->data;

  strcpy(data->response, "Unknown error");
  data->error[0] = 0;
  curl_easy_setopt(handler->curl, CURLOPT_URL, data->mon->config->server.url);
  curl_easy_setopt(handler->curl, CURLOPT_POSTFIELDSIZE, data->data_size);
  curl_easy_setopt(handler->curl, CURLOPT_POSTFIELDS, (char *)data->data);
  curl_easy_setopt(handler->curl, CURLOPT_HTTPHEADER, data->headers);
  curl_easy_setopt(handler->curl, CURLOPT_TIMEOUT, data->mon->config->server.timeout);
  curl_easy_setopt(handler->curl, CURLOPT_WRITEDATA, (void *)data->response);
  curl_easy_setopt(handler->curl, CURLOPT_WRITEFUNCTION, &data_handler);
  curl_easy_setopt(handler->curl, CURLOPT_ERRORBUFFER, data->error);
  if (data->mon->config->server.proxy_url)
    curl_easy_setopt(handler->curl, CURLOPT_PROXY, data->mon->config->server.proxy_url);
}

static void handle_server_response(monikor_curl_handler_t *handler, CURLcode result) {
  send_data_t *data = (send_data_t *)handler->data;
  long http_code;

  curl_easy_getinfo(handler->curl, CURLINFO_RESPONSE_CODE, &http_code);
  if (result != CURLE_OK) {
    size_t errlen = strlen(data->error);
    if (errlen) {
      if (data->error[errlen - 1] == '\n')
        data->error[errlen - 1] = 0;
      monikor_log(LOG_ERR, "Send metrics failed: %s\n", data->error);
    } else {
      monikor_log(LOG_ERR, "Send metrics failed: %s\n", curl_easy_strerror(result));
    }
  } else if (http_code != 200) {
    monikor_log(LOG_ERR, "Send metrics failed: %s\n", data->response);
  } else {
    monikor_log(LOG_INFO, "Metrics successfully sent\n");
  }
  if (result != CURLE_OK || http_code != 200) {
    monikor_log(LOG_INFO, "Caching %zu metrics (%zuB)\n",
      data->metrics->size, data->metrics->data_size);
    monikor_metric_list_concat(data->mon->metrics->cache, data->metrics);
  }
  monikor_metric_list_free(data->metrics);
  curl_slist_free_all(data->headers);
  free(data->data);
  free(data);
}

int monikor_send_metrics(monikor_t *mon, monikor_metric_list_t *metrics) {
  monikor_curl_handler_t *handler = NULL;
  send_data_t *data = NULL;
  struct curl_slist *headers = NULL;

  if (!mon->config->server.url
  || !(data = malloc(sizeof(*data)))
  || monikor_metric_list_serialize(metrics, &data->data, &data->data_size)
  || !(headers = set_http_headers(mon))
  || !(handler = monikor_curl_handler_new(&handle_server_response, data))) {
    monikor_log(LOG_ERR, "Cannot perform metric send\n");
    if (data)
      free(data->data);
    free(data);
    if (handler)
      curl_easy_cleanup(handler->curl);
    free(handler);
    curl_slist_free_all(headers);
    return -1;
  }
  data->mon = mon;
  data->metrics = metrics;
  data->headers = headers;
  setup_curl_request(handler);
  monikor_io_handler_list_push_curl(&mon->io_handlers, handler);
  monikor_log(LOG_INFO, "Sending %zu metrics to %s\n",
    metrics->size, mon->config->server.url);
  return 0;
}

void monikor_send_all_metrics(monikor_t *mon) {
  monikor_metric_list_t *metrics;

  if (!mon->metrics->current->size && !mon->metrics->cache->size) {
    monikor_log(LOG_DEBUG, "No metrics to send\n");
    return;
  }
  if (!(metrics = monikor_metric_list_new())) {
    monikor_log(LOG_ERR, "Cannot send metrics: out of memory\n");
    monikor_metric_store_cache(mon->metrics);
    return;
  }
  monikor_metric_store_uncache(mon->metrics);
  while (mon->metrics->current->size && metrics->data_size < mon->config->server.max_send_size) {
    monikor_metric_list_push(metrics, monikor_metric_list_pop(mon->metrics->current));
  }
  if (monikor_send_metrics(mon, metrics)) {
    monikor_log(LOG_INFO, "Caching %zu metrics (%zub)\n", metrics->size, metrics->data_size);
    monikor_metric_list_concat(mon->metrics->cache, metrics);
  }
  monikor_metric_store_cache(mon->metrics);
}
