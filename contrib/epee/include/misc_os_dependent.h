// Copyright (c) 2006-2013, Andrey N. Sabelnikov, www.sabelnikov.net
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// * Neither the name of the Andrey N. Sabelnikov nor the
// names of its contributors may be used to endorse or promote products
// derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER  BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
#ifdef WIN32
  #ifndef WIN32_LEAN_AND_MEAN 
  #define WIN32_LEAN_AND_MEAN
  #endif

  //#ifdef _WIN32_WINNT 
  //  #undef _WIN32_WINNT
  //  #define _WIN32_WINNT 0x0600
  //#endif

  
#include <windows.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#pragma once 
namespace epee
{
namespace misc_utils
{

        inline uint64_t get_tick_count()
        {
#if defined(_MSC_VER)
        LARGE_INTEGER TicksPerSecond = { 0 };  
        LARGE_INTEGER Tick;  
        if (!TicksPerSecond.QuadPart)  
            QueryPerformanceFrequency(&TicksPerSecond);  
        QueryPerformanceCounter(&Tick);  
        __int64 Seconds = Tick.QuadPart / TicksPerSecond.QuadPart;  
        __int64 LeftPart = Tick.QuadPart - (TicksPerSecond.QuadPart*Seconds);  
        __int64 MillSeconds = LeftPart * 1000 / TicksPerSecond.QuadPart;  
        __int64 Ret = Seconds * 1000 + MillSeconds;  
        return Ret;  
#elif defined(__MACH__)
                clock_serv_t cclock;
                mach_timespec_t mts;

                host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
                clock_get_time(cclock, &mts);
                mach_port_deallocate(mach_task_self(), cclock);

                return (mts.tv_sec * 1000) + (mts.tv_nsec/1000000);
#else
                struct timespec ts;
                if(clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
                        return 0;
                }
                return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
#endif
        }


        inline int call_sys_cmd(const std::string& cmd)
	{      
                std::cout << "# " << cmd << std::endl;

		FILE * fp ;
		//char tstCommand[] ="ls *";
		char path[1000] = {0};
#if !defined(__GNUC__) 
		fp = _popen(cmd.c_str(), "r");
#else
		fp = popen(cmd.c_str(), "r");
#endif
		while ( fgets( path, 1000, fp ) != NULL )
			std::cout << path;

#if !defined(__GNUC__) 
		_pclose(fp);
#else
		pclose(fp);
#endif
		return 0;

	}


	inline std::string get_thread_string_id()
	{
#if defined(_MSC_VER)
		return boost::lexical_cast<std::string>(GetCurrentThreadId());
#elif defined(__GNUC__)  
		return boost::lexical_cast<std::string>(pthread_self());
#endif
	}
}
}
