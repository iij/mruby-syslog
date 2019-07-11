mruby-syslog
============
[![Build Status](https://travis-ci.org/iij/mruby-syslog.svg?branch=master)](https://travis-ci.org/iij/mruby-syslog)

"mruby-syslog" provides a mruby interface to the POSIX system logging facility.
API is compatible with [Syslog module for CRuby](http://ruby-doc.org/stdlib-2.1.0/libdoc/syslog/rdoc/Syslog.html).

### How To Use:

Add a line to your `build_config.rb`:

```Ruby
conf.gem :github => 'iij/mruby-syslog'
```

### Example:

```Ruby
Syslog.open("syslogtest")
Syslog.log(Syslog::LOG_WARNING, "the sky is falling in %d seconds!", 100)
Syslog.close

# or simply
Syslog.err("something's wrong here")
```

### Compatibility

Supported methods are:

- Syslog.alert
- Syslog.close
- Syslog.crit
- Syslog.debug
- Syslog.emerg
- Syslog.err
- Syslog.facility
- Syslog.ident
- Syslog.info
- Syslog.log(priority, format, \*arg)
- Syslog.notice
- Syslog.open
- Syslog.opened?
- Syslog.options
- Syslog.warning

Note: You can write syslog messages without calling Syslog.open though it is required on CRuby.

## License

Copyright (c) 2012 Internet Initiative Japan Inc.

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
