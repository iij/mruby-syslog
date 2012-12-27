mruby-syslog
============

### To build:

Prerequisites:

    * mruby
    * libc

    activate GEMs in *$(MRUBY_ROOT)/mrbgems/GEMS.active*
    env ENABLE_GEMS=true ruby ./minirake

### To run the tests:

    env ENABLE_GEMS=true ruby ./minirake test

# LICENSE
    * See Copyright Notice in mruby.h
