#include <time.h>

#ifdef __linux__
	#include <unistd.h>
	#include <errno.h>
	#include <time.h>
	#include <sys/time.h>
	#include <sys/times.h>
	#include <time.h>
#elif defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	#include <windows.h>
#else
	#include <unistd.h>
	#include <errno.h>
	#include <time.h>
	#include <sys/time.h>
	#include <sys/times.h>
	#include <time.h>
#endif

#if defined(__APPLE__) || defined(__linux__)
#define __cdecl 
#endif

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
void __cdecl sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
#endif

void __cdecl writeInt(unsigned int i)
{
	printf("%d ", i);
}

void __cdecl writeLineEnd()
{
	printf("\n");
}

unsigned int __cdecl getTimeInMS()
{
#ifdef __linux__
	double rt;
	struct timeval r_time;
	struct timezone z_time;
	gettimeofday(&r_time, &z_time);
	rt = (double)r_time.tv_sec + ((double)r_time.tv_usec) * 0.000001;
    return (unsigned int)(rt*1000.0);
#elif defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	LONGLONG current;
	LONGLONG tmp;
	QueryPerformanceFrequency((LARGE_INTEGER*)&tmp);
	double cpuFrequency = (double)tmp;
	QueryPerformanceCounter((LARGE_INTEGER*)&current);
	return (unsigned int)( (double)(current / cpuFrequency) * 1000.0 );
#endif
}

void __cdecl writeTime()
{
	char text [9];
#ifndef __APPLE__
	_strtime(text);
#endif
	printf("%s ", text);
}

void __cdecl waitAnyKey()
{
	getchar();
}

void __cdecl processMessages()
{
#ifdef __linux__
#elif defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    MSG       msg;
    if (PeekMessage (&msg, NULL, 0, 0,PM_NOREMOVE))
        if ((msg.message == WM_QUIT)
          ||(msg.message == WM_CLOSE)
          ||(msg.message == WM_DESTROY)
          ||(msg.message == WM_NCDESTROY)
          ||(msg.message == WM_HSCROLL)
          ||(msg.message == WM_VSCROLL)
          )
          return;
    if (PeekMessage (&msg, NULL, 0, 0,PM_REMOVE))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
#endif
}
