#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "monikor.h"


static int write_pid(monikor_t *mon, pid_t pid) {
  int fd;

  if ((fd = open(mon->config->pidfile, O_WRONLY|O_CREAT, 0644)) == -1)
    return -1;
  dprintf(fd, "%d\n", pid);
  close(fd);
  return 0;
}

static int perform_daemonize(monikor_t *mon) {
  pid_t pid;
  int nullfd;

  pid = fork();
  if (pid == -1) {
    monikor_log(LOG_ERR, "Cannot fork(2): %s\n", strerror(errno));
    return 1;
  } else if (pid != 0) {
    exit(0);
  }
  if (setsid() == -1) {
    monikor_log(LOG_ERR, "Cannot setsid(2): %s\n", strerror(errno));
    return 1;
  }
  umask(0);
  pid = fork();
  if (pid == -1) {
    monikor_log(LOG_ERR, "Cannot fork(2): %s\n", strerror(errno));
    return 1;
  } else if (pid != 0) {
    if (write_pid(mon, pid)) {
      monikor_log(LOG_ERR, "Cannot write pid file: %s\n", strerror(errno));
    }
    exit(0);
  }
  if (chdir(mon->config->run_dir) == -1) {
    monikor_log(LOG_ERR, "Cannot chdir(2) to %s: %s\n", mon->config->run_dir, strerror(errno));
    return 1;
  }
  if ((nullfd = open("/dev/null", O_RDWR)) == -1
  || dup2(nullfd, 0) == -1
  || dup2(nullfd, 1) == -1
  || dup2(nullfd, 2) == -1) {
    monikor_log(LOG_ERR, "Cannot replace standard file descriptors with /dev/null: %s\n",
      strerror(errno));
  }
  close(nullfd);
  return 0;
}

static int drop_privileges(monikor_t *mon) {
  uid_t uid;
  gid_t gid;
  struct passwd *pwd_ent;

  if (!mon->config->user) {
    monikor_log(LOG_WARNING, "Running as root is strongly discouraged!\n");
    return 0;
  }
  if (!(pwd_ent = getpwnam(mon->config->user))) {
    monikor_log(LOG_ERR, "Cannot get passwd entry for user %s\n", mon->config->user);
    return 1;
  }
  uid = pwd_ent->pw_uid;
  if (mon->config->group) {
    struct group *grp_ent;

    if (!(grp_ent = getgrnam(mon->config->group))) {
      monikor_log(LOG_ERR, "Cannot get group entry for group %s\n", mon->config->group);
      return 1;
    }
    gid = grp_ent->gr_gid;
  } else {
    gid = pwd_ent->pw_gid;
  }
  if (getuid() != 0) {
    monikor_log(LOG_ERR, "Cannot drop privileges since we are not running as root\n");
    return 0;
  }
  if (setgid(gid) == -1
  || initgroups(mon->config->user, gid) == -1
  || setuid(uid) == -1) {
    monikor_log(LOG_ERR, "Cannot drop root privileges: %s\n", strerror(errno));
    return 1;
  }
  return 0;
}

int monikor_daemonize(monikor_t *mon) {
  if (mon->config->daemonize && perform_daemonize(mon))
      return 1;
  if (drop_privileges(mon))
    return 1;
  return 0;
}
