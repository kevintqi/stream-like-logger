// =============================================================================
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// Copyright (c) 2017 Kevin T Qi
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================

#pragma once

#include <memory>
#include <mutex>
#include <fstream>
#include <iostream>
#include <string>
#include <syslog.h>

namespace slogger {
   
class ILoggerSink {
public:
   virtual ~ILoggerSink() {}
   virtual void flush(const std::string& str) = 0;
};

typedef std::shared_ptr<ILoggerSink> ILoggerSinkPtr;

class StdOutSink : public ILoggerSink{
private:
   std::mutex mMutex;
   
public:
   void flush(const std::string& str) {
      std::lock_guard<std::mutex> lock(mMutex);
      std::cout << str << std::endl;
   }
};

class FileLoggerSink : public ILoggerSink{
private:
   std::mutex mMutex;
   std::ofstream mFile;
   
public:
   FileLoggerSink(const char *fileName): mFile(fileName, std::ofstream::app) {}
   
   void flush(const std::string& str) {
      std::lock_guard<std::mutex> lock(mMutex);
      mFile << str;
      mFile.flush();
   }
};

class SyslogSink : public ILoggerSink{
public:
   SyslogSink(const char *name) {
      openlog(name, LOG_PID, LOG_USER);
   }
   
   void flush(const std::string& str) {
      syslog(LOG_CRIT, "%s", str.c_str());
   }
};


} /* namespace slogger */