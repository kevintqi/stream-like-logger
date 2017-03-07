# stream-like-logger

## Overview
The stream-like-logger is a C++ logger utility uses iostream like interfaces.
```code
INFO_LOG << " 1 + 1 = " << 2 << LOG_EL;
ERROR_LOG << "Something is very wrong!" << LOG_EL;
```
Possible log meesages for the code might be
```log
1488492126 my_module[I]main.cpp:8 1 + 1 = 2
1488492126 my_module[E]main.cpp:9 Something is very wrong!
```
The stream-like-logger is a header-only library. It is MT safe.

## Customization
### Logger Prefix
You can define your own prefix by implemting the `ILoggerPrefix` interface.
```code
class ILoggerPrefix {
public:
  virtual ~ILoggerPrefix() {}
  virtual std::string operator()(LoggerLevel lv, const char *file, int line) = 0;
};
```
There are sample implementation included - feel free to remove them if they don't fit your need. The log message shown above is created from the `TimedSignaturePrefix` 

### Logger Sink
You can determine where you put your log messages by implemting the 'ILoggerSink' interface.
```code
class ILoggerSink {
public:
   virtual ~ILoggerSink() {}
   virtual void flush(const std::string& str) = 0;
};
```
There are sample implementation included
* `StdOutSink`
* `FileLoggerSink`
* `SyslogSink`

Remove them if they don't fit your need.

## Sample Usage
The library provides a static instance of the `Logger`, which can be accessed by calling `theLogger()`. It also defined a few ready-to-use MACROs. These should be sufficient for most of use cases. By default, the Logger doesn't do anything. It has be configured with
* number concurrent Logger streams (3 might be good enough for most cases - unless you know your application has less threads)
* an shared_ptr of `ILoggerPrefix` instance
* an shared_ptr of `ILoggerSink` instance

```code
#include "slogger/Logger.h"

using namespace slogger;
int main(int argc, char* argv[]) {
 ILoggerPrefixPtr prefix(new TimedSignaturePrefix("my_module"));
 ILoggerSinkPtr sink(new FileLoggerSink("./test.log"));
 theLogger().configure(3, prefix, sink);
 DEBUG_LOG << "The debug stuff is normally not visible unless the level is specifically set " << LOG_EL;
 INFO_LOG << "1 + 1 = " << 2 << LOG_EL;
 ERROR_LOG << "Something is very wrong!"<< LOG_EL;
 ALWAYS_LOG << "I want my lucky number " << 8  << "to be always shown" << LOG_EL;
 return 0;
}
```
