module Syslog
  def self.log(priority, format, *arg)
    Syslog._log0 priority, (sprintf(format, *arg) rescue format)
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

  class << self
    [ "emerg",   "alert",  "crit", "err",
      "warning", "notice", "info", "debug" ].each { |lstr|
      lnum = Syslog.const_get("LOG_#{lstr.upcase}")
      self.define_method(lstr) do |*args|
        Syslog.log lnum, *args
      end
    }
  end
end
