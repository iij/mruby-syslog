#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "mruby.h"
#include "mruby/string.h"
#include "error.h"

static int saved_stderr;   /* XXX */

mrb_value
mrb_syslogtest_last(mrb_state *mrb, mrb_value cls)
{
  mrb_value path;
  FILE *fp;
  size_t n;
  char *cp;
  char buf[1000];

  mrb_get_args(mrb, "S", &path);
  cp = mrb_str_to_cstr(mrb, path);

  fp = fopen(cp, "r");
  if (fp == NULL) {
    mrb_sys_fail(mrb, "cannot open syslog file");
  }

  buf[0] = '\0';
  while (fgets(buf, sizeof(buf), fp) != NULL)
    ;
  n = strcspn(buf, "\n");
  buf[n] = '\0';

  fclose(fp);

  return mrb_str_new(mrb, buf, n);
}

mrb_value
mrb_syslogtest_setup(mrb_state *mrb, mrb_value cls)
{
  mrb_value path;
  int d;
  char *cp;

  mrb_get_args(mrb, "S", &path);
  cp = mrb_str_to_cstr(mrb, path);

  d = open(cp, O_WRONLY|O_CREAT|O_TRUNC, 0666);
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
  mrb_value path;
  char *cp;

  mrb_get_args(mrb, "S", &path);
  cp = mrb_str_to_cstr(mrb, path);
  (void)unlink(cp);

  if (dup2(saved_stderr, STDERR_FILENO) == -1) {
    mrb_sys_fail(mrb, "dup2");
  }
  return mrb_nil_value();
}

void
mrb_mruby_syslog_gem_test(mrb_state *mrb)
{
  struct RClass *m = mrb_define_module(mrb, "SyslogTest");
  mrb_define_class_method(mrb, m, "last", mrb_syslogtest_last, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, m, "setup", mrb_syslogtest_setup, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, m, "teardown", mrb_syslogtest_teardown, MRB_ARGS_OPT(1));
}
