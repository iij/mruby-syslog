#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "mruby.h"
#include "error.h"

static int saved_stderr;
static char *tmppath = NULL;

mrb_value
mrb_syslogtest_setup(mrb_state *mrb, mrb_value cls)
{
  int d, len;
  char *cp;

  mrb_get_args(mrb, "s", &cp, &len);
  tmppath = malloc(len + 1);
  memcpy(tmppath, cp, len);
  tmppath[len] = '\0';

  d = open(tmppath, O_WRONLY|O_CREAT|O_TRUNC, 0666);
  if (d == -1) {
    mrb_sys_fail(mrb, "cannot open syslog file");
  }

  saved_stderr = dup(STDERR_FILENO);
  if (dup2(d, STDERR_FILENO) == -1) {
    mrb_sys_fail(mrb, "dup2");
  }

  return mrb_nil_value();
}

mrb_value
mrb_syslogtest_teardown(mrb_state *mrb, mrb_value cls)
{
  mrb_value b;

  b = mrb_true_value();
  mrb_get_args(mrb, "|o", &b);

  if (tmppath != NULL) {
    if (mrb_bool(b)) {
      unlink(tmppath);
    }
    free(tmppath);
  }
  if (dup2(saved_stderr, STDERR_FILENO) == -1) {
    mrb_sys_fail(mrb, "dup2");
  }
  return mrb_nil_value();
}

void
mrb_mruby_syslog_gem_test(mrb_state *mrb)
{
  struct RClass *m = mrb_define_module(mrb, "SyslogTest");
  mrb_define_class_method(mrb, m, "setup", mrb_syslogtest_setup, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, m, "teardown", mrb_syslogtest_teardown, MRB_ARGS_OPT(1));
}
