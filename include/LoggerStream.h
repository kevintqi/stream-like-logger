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

#include "LoggerSink.h"
#include <memory>
#include <sstream>
#include <string>
#include <atomic>

namespace slogger {

class LoggerStreamFlush {
public:
    static const LoggerStreamFlush &end() {
        static LoggerStreamFlush instance;
        return instance;
    }
};

class LoggerStream;
typedef std::shared_ptr<LoggerStream> LoggerStreamPtr;

class LoggerStream {
private:
    std::stringstream mLogStream;
    ILoggerSinkPtr mSink;
    std::atomic<bool> mInUse;

    template<typename T> friend LoggerStreamPtr &operator<<(LoggerStreamPtr &logStrm, const T &value); 
    friend LoggerStreamPtr &operator<<(LoggerStreamPtr &logStrm, const LoggerStreamFlush &el);

    void flush() {
        if (mSink) {
            mLogStream << std::endl;
            mSink->flush(mLogStream.str());
            mLogStream.clear();
            mLogStream.str("");
            mInUse = false;
        }
    }

public:
    LoggerStream(ILoggerSinkPtr sink = nullptr) : mSink(sink), mInUse(false) {
    }
    ~LoggerStream() {
        mInUse = false;
    }

    bool inUse() {
        return mInUse;
    }
};

template<typename T> inline LoggerStreamPtr &operator<<(LoggerStreamPtr &logStrm, const T &value) {
     if (logStrm && logStrm->mSink) {
        logStrm->mInUse = true;
        logStrm->mLogStream << value;
    }
    return logStrm;  
}

inline LoggerStreamPtr &operator<<(LoggerStreamPtr &logStrm, const LoggerStreamFlush &el) {
    if (logStrm && logStrm->mSink) {
        logStrm->flush();
    }
    return logStrm;
}

} /* namespace slogger */
