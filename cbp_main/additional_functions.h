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
void __cdecl cbp_sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
#else
void cbp_sleep(unsigned int mseconds)
{
    usleep(mseconds);
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

void __cdecl printBenchmark(unsigned int times[], unsigned int avgInt, unsigned int k)
{
		double avg = (double) avgInt;//average
		//double avg = reinterpret_cast<double>(avgInt)

		double med = 0;//medium
		double min = 0;//minimum
		double max = 0;//maximum
		double dev = 0;//deviation
    	double tempTime = 0;//temporary time

		quicksort(&times[0],0,k-1);
	    med = times[k/2];
	    min = times[0];
	    max = times[k-1];

	   	//med = reinterpret_cast<double>(&times[k/2])[0];
		//min = reinterpret_cast<double>(&times[0])[0];
	    //max = reinterpret_cast<double>(&times[k-1])[0];

	    avg /= (double)k;

	    for(unsigned int i = 0; i<k; i++)
	    {
	        //dev += ((reinterpret_cast<double>(&times[i])[0]-avg)*(reinterpret_cast<double>(&times[i])[0]-avg));
	        dev += ((times[i]-avg)*(times[i]-avg));
	    }

	    dev /= (double)(k-1);
	    dev = sqrt(dev);

	    printf("minimum: %f avarage: %f median: %f deviation: %f maximum: %f" min, avg, med, dev ,max );
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
#ifdef defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	_strtime(text);
    printf("%s ", text);
#else
    time_t rawtime;

    time(&rawtime);
    printf("%s ", ctime(&rawtime));
#endif
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
