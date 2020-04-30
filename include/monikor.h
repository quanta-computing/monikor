#ifndef MONIKOR_H_
#define MONIKOR_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/time.h>


#ifndef MONIKOR_STRUCT_TYPEDEF_DECL
# define MONIKOR_STRUCT_TYPEDEF_DECL
typedef struct monikor_s monikor_t;
#endif

#define MONIKOR_VERSION "1.2.2"

#include "server.h"
#include "strl.h"
#include "metric.h"
#include "module.h"
#include "logger.h"
#include "config.h"
#include "io_handler.h"
#include "utils.h"

#define MONIKOR_FLAG_RELOAD (1 << 0)

struct monikor_s {
  monikor_config_t *config;
  struct {
    monikor_mod_t **modules;
    size_t count;
  } modules;
  monikor_metric_store_t *metrics;
  monikor_server_t server;
  monikor_io_handler_list_t io_handlers;
  struct timeval last_clock;
  uint8_t flags;
};

// misc
void usage(void);

// core
int monikor_init(monikor_t *mon, char *config_path);
int monikor_init_modules(monikor_t *mon);
void monikor_cleanup(monikor_t *mon);
void monikor_cleanup_modules(monikor_t *mon);
int monikor_run(monikor_t *mon);
void monikor_exit(monikor_t *mon);
int monikor_reload(monikor_t *mon);
int monikor_daemonize(monikor_t *mon);

int monikor_load_all_modules(monikor_t *mon);
void monikor_update(monikor_t *mon, struct timeval *clock);
int monikor_poll_modules(monikor_t *mon, struct timeval *clock);
int monikor_send_metrics(monikor_t *mon, monikor_metric_list_t *metrics);
void monikor_send_all_metrics(monikor_t *mon);
void monikor_evict_metrics(monikor_t *mon);

void monikor_send_all(monikor_t *mon);
void monikor_flush_all(monikor_t *mon);
void monikor_flag_reload(monikor_t *mon);


// metrics
int monikor_metric_push(monikor_t *mon, monikor_metric_t *metric);

// Signal handling
int monikor_register_signals(monikor_t *mon);
void monikor_signal_cleanup(void);

// IO handling
void monikor_register_io_handler(monikor_t *mon, monikor_io_handler_t *handler);
void monikor_unregister_io_handler(monikor_t *mon, monikor_io_handler_t *handler);

#endif /* end of include guard: MONIKOR_H_ */
