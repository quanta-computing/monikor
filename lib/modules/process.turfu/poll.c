#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

#include "monikor.h"
#include "process.h"


static int fetch_one_process_metrics(monikor_t *mon, struct timeval *clock,
pid_t pid, char *name, char *proc_info) {
  int n;
  size_t idx = 0;
  char *value;
  char *save;
  char metric_name[512];
  char *metric_base_end;

  metric_base_end = metric_name + sprintf(metric_name, "process.%s.", name);
  strcpy(metric_base_end, "count");
  monikor_metric_push(mon, monikor_metric_integer(
    metric_name, clock, 1, MONIKOR_METRIC_AGGREGATE
  ));
  n = 1;
  for (value = strtok_r(proc_info, " ", &save); value; value = strtok_r(NULL, " ", &save)) {
    n++;
    switch (idx) {
    case 11:
      strcpy(metric_base_end, "cpu.user");
      monikor_metric_push(mon, monikor_metric_float_id(
        metric_name, clock, strtof(value, NULL) / sysconf(_SC_CLK_TCK),
        MONIKOR_METRIC_TIMEDELTA | MONIKOR_METRIC_AGGREGATE, pid
      ));
      break;
    case 12:
      strcpy(metric_base_end, "cpu.kernel");
      monikor_metric_push(mon, monikor_metric_integer_id(
        metric_name, clock, strtof(value, NULL) / sysconf(_SC_CLK_TCK),
        MONIKOR_METRIC_TIMEDELTA | MONIKOR_METRIC_AGGREGATE, pid
      ));
      break;
    case 20:
      strcpy(metric_base_end, "mem.vsize");
      monikor_metric_push(mon, monikor_metric_integer(
        metric_name, clock, strtol(value, NULL, 10), MONIKOR_METRIC_AGGREGATE
      ));
      break;
    case 21:
      strcpy(metric_base_end, "mem.rss");
      monikor_metric_push(mon, monikor_metric_integer(
        metric_name, clock, strtol(value, NULL, 10) * getpagesize(), MONIKOR_METRIC_AGGREGATE
      ));
      break;
    default:
      n--;
    }
    idx++;
  }
  return n;
}

static int poll_one_process(monikor_t *mon, struct timeval *clock, pid_t pid) {
  char *proc_stat;
  char *name;
  char *info;
  char filepath[256];
  int n;

  sprintf(filepath, "/proc/%d/stat", pid);
  if (!(proc_stat = monikor_read_file(filepath))
  || !(name = strchr(proc_stat, '('))
  || !(info = strchr(name, ')'))) {
    free(proc_stat);
    return 0;
  }
  *info = 0;
  n = fetch_one_process_metrics(mon, clock, pid, name + 1, info + 1);
  free(proc_stat);
  return n;
}

static int looks_like_pid_dir(const struct dirent *entry) {
  if (entry->d_type != DT_DIR)
    return 0;
  for (size_t i = 0; entry->d_name[i]; i++)
    if (!isdigit(entry->d_name[i]))
      return 0;
  return 1;
}

int poll_processes_metrics(monikor_t *mon, struct timeval *clock) {
  int n = 0;
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir("/proc")))
    return 0;
  while ((entry = readdir(dir))) {
    if (looks_like_pid_dir(entry))
      n += poll_one_process(mon, clock, atoi(entry->d_name));
  }
  closedir(dir);
  return n;
}
