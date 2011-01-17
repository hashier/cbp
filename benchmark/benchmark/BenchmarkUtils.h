#ifndef BENCHMARKUTILS_H
#define BENCHMARKUTILS_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

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

// Returns the time in seconds as double resolution
inline double getTimeInSeconds() 
{ 
#ifdef __linux__
	double rt;
	struct timeval r_time;
	struct timezone z_time;
	gettimeofday(&r_time, &z_time);
	rt = (double)r_time.tv_sec + ((double)r_time.tv_usec) * 0.000001;
    return(rt);
#elif defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	LONGLONG current;
	LONGLONG tmp;
	QueryPerformanceFrequency((LARGE_INTEGER*)&tmp);
	double cpuFrequency = (double)tmp;
	QueryPerformanceCounter((LARGE_INTEGER*)&current);
	return (double)(current / cpuFrequency); 
#endif
}

//quicksort
inline void quicksort(double *a, int left, int right) 
{
	if (left < right) {

	double pivot = a[right]; 
	int l = left;
	int r = right;

    do {
      while (a[l] < pivot) l++;
      while (a[r] > pivot) r--;
      if (l <= r) {
          double swap = a[l];
          a[l] = a[r];
          a[r] = swap;
          l++;
          r--;
      }
    } while (l <= r);

    quicksort(a, left, r);
    quicksort(a, l, right);
  }
}
#endif
