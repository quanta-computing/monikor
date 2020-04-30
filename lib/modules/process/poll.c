#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "monikor.h"
#include "process.h"


static int looks_like_pid_dir(const struct dirent *entry) {
  if (entry->d_type != DT_DIR)
    return 0;
  for (size_t i = 0; entry->d_name[i]; i++)
    if (!isdigit(entry->d_name[i]))
      return 0;
  return 1;
}

static void fetch_one_process_metrics(uint64_t *metrics, char *pid) {
  char *proc_stat;
  char *info;
  char *status;
  char filepath[512];

  sprintf(filepath, "/proc/%s/stat", pid);
  if (!(proc_stat = monikor_read_file(filepath))
  || !(info = strchr(proc_stat, ')'))) {
    free(proc_stat);
    return;
  }
  if ((status = strchr(PROC_STATUSES, *(info + 2))))
    metrics[status - PROC_STATUSES]++;
  metrics[PROC_TOTAL]++;
  free(proc_stat);
}

static const char *metric_name[] = {
  "process.running", "process.sleeping", "process.waiting",
  "process.zombie", "process.stopped", "process.paging", "process.total",
  NULL
};

int poll_process_metrics(monikor_t *mon, struct timeval *clock) {
  DIR *dir;
  struct dirent *entry;
  uint64_t metrics[NB_PROC_METRICS] = {0};

  if (!(dir = opendir("/proc")))
    return 0;
  while ((entry = readdir(dir))) {
    if (looks_like_pid_dir(entry))
      fetch_one_process_metrics(metrics, entry->d_name);
  }
  closedir(dir);
  for (size_t i = 0; i < NB_PROC_METRICS; i++)
    monikor_metric_push(mon, monikor_metric_integer(metric_name[i], clock, metrics[i], 0));
  return NB_PROC_METRICS;
}
