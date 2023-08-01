/******************************************************************************
 * Copyright 2018 The Hesai Technology Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef HGUARD_PANDAR_LOG
#define HGUARD_PANDAR_LOG

#include <stdio.h>
#include <thread>
#include <chrono>
#include <ctime>

class TranceFunc
{

private:

    const char* m_cFile = nullptr;
    const char* m_cFunc = nullptr;

    static void getUnixSecsAndMillis(time_t& time, int& millis) {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        auto time_since_epoch = now.time_since_epoch();
        time = std::chrono::system_clock::to_time_t(now);
        millis = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count() % 1000;
    }

    void writeLog() {
        time_t t{};
        int millis{};
        getUnixSecsAndMillis(t, millis);
        struct tm* ptm = localtime(&t);
        printf("[T] %02d:%02d:%02d.%03d pid:%d tid:%10d ->[File:%s Function:%s ]\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, millis, getpid(), std::hash<std::thread::id>()(std::this_thread::get_id()), m_cFile, m_cFunc);
    }

public:
    
    TranceFunc(const char* file, const char* func){
        m_cFile = file;
        m_cFunc = func;
        writeLog();
    }

    ~TranceFunc() { writeLog(); }

};

#define LOG_D(format,...) {\
    struct tm *ptm;\
    struct timeb stTimeb;\
    ftime(&stTimeb);\
    ptm = localtime(&stTimeb.time);\
    printf("[D] %02d:%02d:%02d.%03d pid:%d tid:%10d File:%s Function:%s Line:%d " format"\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm, getpid(), std::hash<std::thread::id>()(std::this_thread::get_id()), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
}

#define LOG_E(format,...) {\
    struct tm *ptm;\
    struct timeb stTimeb;\
    ftime(&stTimeb);\
    ptm = localtime(&stTimeb.time);\
    printf("[E] %02d:%02d:%02d.%03d pid:%d tid:%10d File:%s Function:%s Line:%d " format"\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm, getpid(), std::hash<std::thread::id>()(std::this_thread::get_id()), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
}

#define LOG_FUNC() TranceFunc tf( __FILE__, __FUNCTION__)
    
#endif