#include <inttypes.h>

#include "monikor.h"
#include "varnish.h"

static const struct {
  char *varnish_name;
  char *name;
  uint8_t flags;
} metrics[] = {
  {"client_req", "varnish.requests_per_second", MONIKOR_METRIC_TIMEDELTA},
  {"s_pipe", "varnish.requests.pipe", MONIKOR_METRIC_TIMEDELTA},
  {"s_pass", "varnish.requests.pass", MONIKOR_METRIC_TIMEDELTA},
  {"s_resp_hdrbytes", "varnish.bytes.headers", MONIKOR_METRIC_TIMEDELTA},
  {"s_resp_bodybytes", "varnish.bytes.body", MONIKOR_METRIC_TIMEDELTA},

  {"cache_hit", "varnish.cache.hits", MONIKOR_METRIC_DELTA},
  {"cache_hitpass", "varnish.cache.hitpasses", MONIKOR_METRIC_DELTA},
  {"cache_miss", "varnish.cache.misses", MONIKOR_METRIC_DELTA},
  {"n_lru_nuked", "varnish.cache.evictions", MONIKOR_METRIC_DELTA},
  {"n_expired", "varnish.cache.expires", MONIKOR_METRIC_DELTA},

  {"sess_conn", "varnish.connections.total", MONIKOR_METRIC_TIMEDELTA},
  {"sess_drop", "varnish.connections.drop", MONIKOR_METRIC_TIMEDELTA},
  {"sess_fail", "varnish.connections.fail", MONIKOR_METRIC_TIMEDELTA},
  {"sess_closed", "varnish.connections.close", MONIKOR_METRIC_TIMEDELTA},
  {"sess_pipeline", "varnish.connections.pipeline", MONIKOR_METRIC_TIMEDELTA},
  {"sess_herd", "varnish.connections.herd", MONIKOR_METRIC_TIMEDELTA},
  {"sess_queued", "varnish.connections.queued", MONIKOR_METRIC_TIMEDELTA},

  {"bans", "varnish.bans.count", 0},

  {"n_object", "varnish.memory.objects", 0},
  {"g_bytes", "varnish.memory.used", 0},
  {"g_space", "varnish.memory.free", 0},
  {"c_fail", "varnish.memory.alloc_fails", MONIKOR_METRIC_DELTA},
  {"shm_records", "varnish.shm.records", MONIKOR_METRIC_TIMEDELTA},
  {"shm_writes", "varnish.shm.writes", MONIKOR_METRIC_TIMEDELTA},
  {"shm_flushes", "varnish.shm.flushes", MONIKOR_METRIC_TIMEDELTA},

  {"n_backend", "varnish.backends.count", 0},
  {"backend_req", "varnish.backends.requests_per_second", MONIKOR_METRIC_TIMEDELTA},
  {"backend_conn", "varnish.backends.connections.count", MONIKOR_METRIC_TIMEDELTA},
  {"backend_unhealthy", "varnish.backends.connections.unhealthy", MONIKOR_METRIC_TIMEDELTA},
  {"backend_busy", "varnish.backends.connections.busy", MONIKOR_METRIC_TIMEDELTA},
  {"backend_fail", "varnish.backends.connections.fail", MONIKOR_METRIC_TIMEDELTA},
  {"backend_reuse", "varnish.backends.connections.reuse", MONIKOR_METRIC_TIMEDELTA},
  {"backend_recycle", "varnish.backends.connections.recycle", MONIKOR_METRIC_TIMEDELTA},

  {"threads", "varnish.threads.count", 0},
  {"threads_failed", "varnish.threads.failed", MONIKOR_METRIC_DELTA},

  {"esi_errors", "varnish.esi.errors", MONIKOR_METRIC_DELTA},
  {"esi_warnings", "varnish.esi.warnings", MONIKOR_METRIC_DELTA},

  {"uptime", "varnish.uptime", 0},

  {NULL, NULL, 0}
};

static int varnish_fetch_metric(void *data, const struct VSC_point * const pt) {
  varnish_iterator_data_t *iterator = (varnish_iterator_data_t *)data;
  char metric_name[256];
  char *found;

  if (!pt)
    return 0;
  for (size_t i = 0; metrics[i].name; i++) {
    // If point name ends with our metric name
    // and the "section" is not "Transient"
    if ((found = strstr(pt->name, metrics[i].varnish_name))
    && strlen(found) == strlen(metrics[i].varnish_name)
    && !strstr(pt->name, ".Transient.")) {
      if (iterator->mod->instance)
        sprintf(metric_name, "%s.%s", metrics[i].name, iterator->mod->instance);
      else
        sprintf(metric_name, "%s", metrics[i].name);
      monikor_metric_push(iterator->mon, monikor_metric_integer(
        metric_name, iterator->clock, *(const volatile uint64_t *)pt->ptr, metrics[i].flags
      ));
      iterator->count++;
      break;
    }
  }
  return 0;
}

int varnish_poll_metrics(varnish_module_t *mod, struct timeval *clock) {
  varnish_iterator_data_t iterator;
  // const char *err;

  iterator.mod = mod;
  iterator.mon = mod->mon;
  iterator.clock = clock;
  iterator.count = 0;
  VSC_Iter(mod->vsc, mod->vsm, &varnish_fetch_metric, (void *)&iterator);
  return iterator.count;
}
