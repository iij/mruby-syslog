module Syslog
  def self.log(priority, format, *arg)
    Syslog._log0 priority, sprintf(format, *arg)
  end
end
