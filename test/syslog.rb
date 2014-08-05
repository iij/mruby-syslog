$syslogtmp = "mruby-syslog.tmp"
def syslog_last_line
  SyslogTest.last $syslogtmp
end

SyslogTest.setup $syslogtmp

assert('Syslog.close') do
  Syslog.open "abc", Syslog::LOG_PERROR
  assert_nil Syslog.close
  assert_raise(RuntimeError) {
    Syslog.close
  }
end

assert('Syslog.facility') do
  Syslog.open("abc", 0, Syslog::LOG_WARNING)
  f = Syslog.facility
  Syslog.close
  assert_equal Syslog::LOG_WARNING, f
end

assert('Syslog.ident') do
  Syslog.open("abc", 0, Syslog::LOG_WARNING)
  s = Syslog.ident
  Syslog.close
  assert_equal "abc", s
end

assert('Syslog.log') do
  Syslog.open("abc", Syslog::LOG_PERROR)
  Syslog.log(Syslog::LOG_INFO, "Syslog.log message")
  Syslog.close

  log = syslog_last_line
  log.include? "abc" and log.include? "Syslog.log message"
end

assert('Syslog.log with format') do
  Syslog.open("abc", Syslog::LOG_PERROR)
  Syslog.log(Syslog::LOG_INFO, "Syslog.log %c %d %s", ?1, 2, "3")
  Syslog.close

  line = syslog_last_line
  line.include? "Syslog.log 1 2 3"
end

assert('Syslog.opened?') do
  o0 = Syslog.opened?
  Syslog.open("abc", Syslog::LOG_PERROR)
  o1 = Syslog.opened?
  Syslog.close
  o2 = Syslog.opened?
  o0 == false and o1 == true and o2 == false
end

assert('Syslog.options') do
  opts1 = Syslog::LOG_NDELAY | Syslog::LOG_PERROR | Syslog::LOG_PID;
  Syslog.open("abc", opts1)
  opts2 = Syslog.options
  Syslog.close
  opts1 == opts2
end

SyslogTest.teardown $syslogtmp
