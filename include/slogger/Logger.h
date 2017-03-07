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

#include "LoggerStream.h"
#include "LoggerPrefix.h"
#include <memory>
#include <string>
#include <vector>

namespace slogger {

class Logger {
private:
    int mNumberofLoggerStreams;
    LoggerLevel mLevel;
    ILoggerPrefixPtr mPrefix;
    ILoggerSinkPtr mSink;
    std::vector<LoggerStreamPtr> mLoggerStreams;
    LoggerStreamPtr mNullStream;

    void initLoggerStream() {
        mLoggerStreams.clear();
        if (mSink) {
            for (int i = 0; i < mNumberofLoggerStreams; ++i) {
                mLoggerStreams.push_back(LoggerStreamPtr(new LoggerStream(mSink)));
            }
        }
    }
    LoggerStreamPtr &availableLoggerStream() {
        for (LoggerStreamPtr &strm : mLoggerStreams) {
            if (strm && !strm->inUse()) {
                return strm;
            }
        }
        return mNullStream;
    }

public:
    Logger() : mNumberofLoggerStreams(0), mLevel(LoggerLevel::INFO) {
    }

    void configure(int numberOfLoggerStreams, ILoggerPrefixPtr prefix, ILoggerSinkPtr sink) {
        mNumberofLoggerStreams = numberOfLoggerStreams;
        mPrefix = prefix;
        mSink = sink;
        initLoggerStream();
    }

    void setLevel(LoggerLevel lv) {
        mLevel = lv;
    }

    LoggerLevel level() const {
        return mLevel;
    }

    LoggerStreamPtr &prefixStream(LoggerLevel lv, const char *file, int line) {
        if (mLevel >= lv) {
            return availableLoggerStream() << (*mPrefix)(lv, file, line);
        } else {
            return mNullStream;
        }
    }

    LoggerStreamPtr &stream(LoggerLevel lv) {
        if (mLevel >= lv) {
            return availableLoggerStream();
        } else {
            return mNullStream;
        }
    }
};

static Logger &theLogger() {
    static Logger instance;
    return instance;
}

#define ALWAYS_LOG theLogger().prefixStream(LoggerLevel::ALWAYS, __FILE__, __LINE__)
#define ERROR_LOG theLogger().prefixStream(LoggerLevel::ERROR, __FILE__, __LINE__)
#define INFO_LOG theLogger().prefixStream(LoggerLevel::INFO, __FILE__, __LINE__)
#define DEBUG_LOG theLogger().prefixStream(LoggerLevel::DEBUG, __FILE__, __LINE__)
#define LOG_EL LoggerStreamFlush::end()

} /* namespace slogger */