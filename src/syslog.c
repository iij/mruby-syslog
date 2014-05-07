/*
** syslog.c - Syslog module
**
*/

#include <string.h>
#include <syslog.h>

#include "mruby.h"
#include "mruby/string.h"
#include "mruby/variable.h"

static mrb_value
reset_vars(mrb_state *mrb, mrb_value self)
{
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "ident"), mrb_nil_value());
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@opened"), mrb_false_value());
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@options"), mrb_nil_value());
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@facility"), mrb_nil_value());
  return self;
}

mrb_value
mrb_f_syslog_open(mrb_state *mrb, mrb_value self)
{
  mrb_value ident, opened;
  mrb_int facility, options;

  opened = mrb_cv_get(mrb, self, mrb_intern_lit(mrb, "@opened"));
  if (mrb_bool(opened)) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "syslog already open");
  }

  ident    = mrb_gv_get(mrb, mrb_intern_lit(mrb, "$0"));
  options  = LOG_PID | LOG_CONS;
  facility = LOG_USER;
  mrb_get_args(mrb, "|Sii", &ident, &options, &facility);

  ident = mrb_check_string_type(mrb, ident);

  /*
   * We assume mrb_str_new() returns a *not-shared* string with a NUL
   * character at the end of the string.
   */
  ident = mrb_str_new(mrb, RSTRING_PTR(ident), RSTRING_LEN(ident));

  /*
   * We must set "ident" to a class variable before calling openlog(3).
   * Otherwise, syslog(3) may refer to some unallocated (GC'ed out) memory
   * area when mrb_cv_set() fails.  Note that some openlog(3) implementations
   * store "ident" argument as is (not strdup(3)ed!).
   * http://man7.org/linux/man-pages/man3/syslog.3.html#NOTES
   *
   * And we make class variable "ident" inaccessible from Ruby world
   * for safety.
   */
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "ident"), ident);

  openlog(RSTRING_PTR(ident), options, facility);

  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@opened"), mrb_true_value());
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@options"), mrb_fixnum_value(options));
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@facility"), mrb_fixnum_value(facility));

  return self;
}

mrb_value
mrb_f_syslog_log0(mrb_state *mrb, mrb_value self)
{
  mrb_int len, prio;
  char *cp;

  mrb_get_args(mrb, "is", &prio, &cp, &len);
  syslog(prio, "%*s", len, cp);
  return self;
}

mrb_value
mrb_f_syslog_close(mrb_state *mrb, mrb_value self)
{
  mrb_value opened;

  opened = mrb_cv_get(mrb, self, mrb_intern_lit(mrb, "@opened"));
  if (! mrb_bool(opened)) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "syslog not opened");
  }

  closelog();
  reset_vars(mrb, self);

  return mrb_nil_value();
}

mrb_value
mrb_f_syslog_ident(mrb_state *mrb, mrb_value self)
{
  mrb_value s;
  s = mrb_cv_get(mrb, self, mrb_intern_lit(mrb, "ident"));
  if (! mrb_string_p(s)) {
    if (mrb_nil_p(s)) return s;
    mrb_raisef(mrb, E_RUNTIME_ERROR, "class variable ident of Syslog is not a string");
  }
  return mrb_str_new_cstr(mrb, RSTRING_PTR(s));
}

void
mrb_mruby_syslog_gem_init(mrb_state *mrb)
{
  struct RClass *slog;

  slog = mrb_define_module(mrb, "Syslog");

  mrb_define_module_function(mrb, slog, "open",    mrb_f_syslog_open,   MRB_ARGS_ANY());
  mrb_define_module_function(mrb, slog, "_log0",   mrb_f_syslog_log0,   MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, slog, "close",   mrb_f_syslog_close,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, slog, "ident",   mrb_f_syslog_ident,  MRB_ARGS_NONE());
  reset_vars(mrb, mrb_obj_value(slog));

  /* Syslog options */
#define mrb_define_syslog_option(c) \
    mrb_define_const(mrb, slog, #c, mrb_fixnum_value(c))

#ifdef LOG_PID
  mrb_define_syslog_option(LOG_PID);
#endif
#ifdef LOG_CONS
  mrb_define_syslog_option(LOG_CONS);
#endif
#ifdef LOG_ODELAY
  mrb_define_syslog_option(LOG_ODELAY); /* deprecated */
#endif
#ifdef LOG_NDELAY
  mrb_define_syslog_option(LOG_NDELAY);
#endif
#ifdef LOG_NOWAIT
  mrb_define_syslog_option(LOG_NOWAIT); /* deprecated */
#endif
#ifdef LOG_PERROR
  mrb_define_syslog_option(LOG_PERROR);
#endif

  /* Syslog facilities */
#define mrb_define_syslog_facility(c) \
  mrb_define_const(mrb, slog, #c, mrb_fixnum_value(c))

#ifdef LOG_AUTH
  mrb_define_syslog_facility(LOG_AUTH);
#endif
#ifdef LOG_AUTHPRIV
  mrb_define_syslog_facility(LOG_AUTHPRIV);
#endif
#ifdef LOG_CONSOLE
  mrb_define_syslog_facility(LOG_CONSOLE);
#endif
#ifdef LOG_CRON
  mrb_define_syslog_facility(LOG_CRON);
#endif
#ifdef LOG_DAEMON
  mrb_define_syslog_facility(LOG_DAEMON);
#endif
#ifdef LOG_FTP
  mrb_define_syslog_facility(LOG_FTP);
#endif
#ifdef LOG_KERN
  mrb_define_syslog_facility(LOG_KERN);
#endif
#ifdef LOG_LPR
  mrb_define_syslog_facility(LOG_LPR);
#endif
#ifdef LOG_MAIL
  mrb_define_syslog_facility(LOG_MAIL);
#endif
#ifdef LOG_NEWS
  mrb_define_syslog_facility(LOG_NEWS);
#endif
#ifdef LOG_NTP
   mrb_define_syslog_facility(LOG_NTP);
#endif
#ifdef LOG_SECURITY
  mrb_define_syslog_facility(LOG_SECURITY);
#endif
#ifdef LOG_SYSLOG
  mrb_define_syslog_facility(LOG_SYSLOG);
#endif
#ifdef LOG_USER
  mrb_define_syslog_facility(LOG_USER);
#endif
#ifdef LOG_UUCP
  mrb_define_syslog_facility(LOG_UUCP);
#endif
#ifdef LOG_LOCAL0
  mrb_define_syslog_facility(LOG_LOCAL0);
#endif
#ifdef LOG_LOCAL1
  mrb_define_syslog_facility(LOG_LOCAL1);
#endif
#ifdef LOG_LOCAL2
  mrb_define_syslog_facility(LOG_LOCAL2);
#endif
#ifdef LOG_LOCAL3
  mrb_define_syslog_facility(LOG_LOCAL3);
#endif
#ifdef LOG_LOCAL4
  mrb_define_syslog_facility(LOG_LOCAL4);
#endif
#ifdef LOG_LOCAL5
  mrb_define_syslog_facility(LOG_LOCAL5);
#endif
#ifdef LOG_LOCAL6
  mrb_define_syslog_facility(LOG_LOCAL6);
#endif
#ifdef LOG_LOCAL7
  mrb_define_syslog_facility(LOG_LOCAL7);
#endif

  /* Syslog levels and the shortcut methods */
#define mrb_define_syslog_level(c, m) \
  mrb_define_const(mrb, slog, #c, mrb_fixnum_value(c));

#ifdef LOG_EMERG
  mrb_define_syslog_level(LOG_EMERG, emerg);
#endif
#ifdef LOG_ALERT
  mrb_define_syslog_level(LOG_ALERT, alert);
#endif
#ifdef LOG_CRIT
  mrb_define_syslog_level(LOG_CRIT, crit);
#endif
#ifdef LOG_ERR
  mrb_define_syslog_level(LOG_ERR, err);
#endif
#ifdef LOG_WARNING
  mrb_define_syslog_level(LOG_WARNING, warning);
#endif
#ifdef LOG_NOTICE
  mrb_define_syslog_level(LOG_NOTICE, notice);
#endif
#ifdef LOG_INFO
  mrb_define_syslog_level(LOG_INFO, info);
#endif
#ifdef LOG_DEBUG
  mrb_define_syslog_level(LOG_DEBUG, debug);
#endif
}

void
mrb_mruby_syslog_gem_final(mrb_state *mrb)
{
}
