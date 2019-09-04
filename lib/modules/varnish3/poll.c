#include <inttypes.h>

#include "monikor.h"
#include "varnish3.h"

static const struct {
  char *varnish_name;
  char *name;
  uint8_t flags;
} metrics[] = {
  {"client_req", "varnish.requests_per_second", MONIKOR_METRIC_TIMEDELTA},
  {"cache_hit", "varnish.cache.hits", MONIKOR_METRIC_DELTA},
  {"cache_hitpass", "varnish.cache.hitpasses", MONIKOR_METRIC_DELTA},
  {"cache_miss", "varnish.cache.misses", MONIKOR_METRIC_DELTA},
  {"n_lru_nuked", "varnish.cache.evictions", MONIKOR_METRIC_DELTA},
  {NULL, NULL, 0}
};

static int varnish_fetch_metric(void *data, const struct VSC_point * const pt) {
  varnish_iterator_data_t *iterator = (varnish_iterator_data_t *)data;
  char metric_name[256];

  if (!pt)
    return 0;
  for (size_t i = 0; metrics[i].name; i++) {
    if (!strcmp(pt->name, metrics[i].varnish_name)) {
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

  if (VSM_Open(mod->vd, 1)) {
    monikor_log_mod(LOG_ERR, MOD_NAME, "Cannot open varnish log\n");
    return -1;
  }
  iterator.mod = mod;
  iterator.mon = mod->mon;
  iterator.clock = clock;
  iterator.count = 0;
  VSC_Iter(mod->vd, &varnish_fetch_metric, (void *)&iterator);
  VSM_Close(mod->vd);
  return iterator.count;
}
