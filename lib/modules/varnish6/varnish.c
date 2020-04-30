#include <stdlib.h>
#include <time.h>

#include "monikor.h"
#include "varnish.h"


void varnish_cleanup(monikor_t *mon, void *data) {
  varnish_module_t *mod = (varnish_module_t *)data;

  (void)mon;
  if (mod->vsc && mod->vsm) {
    VSC_Destroy(&mod->vsc, mod->vsm);
    VSM_Destroy(&mod->vsm);
  }
  free(mod);
}

void *varnish_init(monikor_t *mon, monikor_config_dict_t *config) {
  varnish_module_t *mod;
  const char *err;

  (void)mon;
  if (!(mod = malloc(sizeof(*mod))))
    return NULL;
  mod->mon = mon;
  if (!(mod->vsm = VSM_New())
  || !(mod->vsc = VSC_New())) {
    free(mod);
    return NULL;
  }

  mod->instance = monikor_config_dict_get_scalar(config, "varnish.instance");
  if (mod->instance) {
    if (strlen(mod->instance) >= MONIKOR_VARNISH_MAX_INSTANCE_LENGTH) {
      mod->instance[MONIKOR_VARNISH_MAX_INSTANCE_LENGTH - 1] = 0;
      monikor_log_mod(LOG_WARNING, MOD_NAME, "Instance name too long, stripped to %d chars\n",
        MONIKOR_VARNISH_MAX_INSTANCE_LENGTH);
    }
    VSM_Arg(mod->vsm, 'n', mod->instance);
  }
  if (VSM_Attach(mod->vsm, -1)) {
    if (!(err = VSM_Error(mod->vsm)))
      err = "unknown error\n";
    monikor_log_mod(LOG_WARNING, MOD_NAME,
      "cannot connect to varnish log: %s", err
    );
    varnish_cleanup(mon, mod);
    return NULL;
  }
  return (void *)mod;
}


int varnish_poll(monikor_t *mon, void *data) {
  struct timeval now;

  (void)mon;
  gettimeofday(&now, NULL);
  if (!data) {
    monikor_log_mod(LOG_ERR, MOD_NAME, "Invalid context data.\n");
    return -1;
  }
  return varnish_poll_metrics((varnish_module_t *)data, &now);
}
