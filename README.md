mruby-syslog
============

### To build:

Prerequisites:

    * mruby
    * libc

    activate GEMs in *build_config.rb*
    * conf.gem :git => 'https://github.com/iij/mruby-syslog.git', :branch => 'master'
    env ENABLE_GEMS=true ruby ./minirake

### To run the tests:

    env ENABLE_GEMS=true ruby ./minirake test


For example,

    Syslog.open("syslogtest")
    Syslog.log(Syslog::LOG_WARNING, "the sky is falling in %d seconds!", 100)
    Syslog.close

# LICENSE
    * See Copyright Notice in mruby.h
