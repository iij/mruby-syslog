module Syslog
  def self.log(priority, format, *arg)
    Syslog._log0 priority, sprintf(format, *arg)
  end

  def self.facility
    @facility
  end

  def self.opened?
    @opened
  end

  def self.options
    @options
  end
end
