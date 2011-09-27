// performance_counter_test.cpp : Defines the entry point for the console application.
//

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <winstl.h>

#include <stdio.h>

#include <winstl_performance_counter.h>
#include <winstl_highperformance_counter.h>
#include <winstl_threadtimes_counter.h>
#include <winstl_processtimes_counter.h>
#include <winstl_tick_counter.h>
#include <winstl_multimedia_counter.h>

#include <winstl_performance_counter_scope.h>
#include <winstl_performance_counter_init.h>

winstl_ns_using(performance_counter)
winstl_ns_using(highperformance_counter)
winstl_ns_using(threadtimes_counter)
winstl_ns_using(processtimes_counter)
winstl_ns_using(tick_counter)
winstl_ns_using(multimedia_counter)

winstl_ns_using(performance_counter_scope)
winstl_ns_using(performance_counter_initialiser)

const int INTERVAL	=	0x3ffffff;


// The original version of this test program returned the elapsed interval,
// which made it simpler to print (than the current sequence operator
// expression - see below). However, this scared GCC, so it has to be in
// the current form

template <ss_typename_param_k C>
inline void test_counter(C &c)
{
	{
		typedef performance_counter_scope<C>	counter_scope_t;

		counter_scope_t	scope(c);
		for(int volatile i = 0; i < INTERVAL; ++i)
		{}
	}
}

int main(int /* argc */, char* /* argv */[])
{
    performance_counter     perf_cntr;
    highperformance_counter highperf_cntr;
    threadtimes_counter     thread_cntr;
    processtimes_counter    process_cntr;
    tick_counter            tick_cntr;
    multimedia_counter      media_cntr;

	{
		performance_counter_initialiser<performance_counter>	initialised_cntr;

		printf("initialised counter: %lu\n", static_cast<unsigned long>(initialised_cntr.get_milliseconds()));

		::Sleep(100);
		initialised_cntr.stop();

		printf("initialised counter: %lu\n", static_cast<unsigned long>(initialised_cntr.get_milliseconds()));

		::Sleep(100);
		initialised_cntr.stop();

		printf("initialised counter: %lu\n", static_cast<unsigned long>(initialised_cntr.get_milliseconds()));
	}

//	printf("performance_counter     : %lu\n", static_cast<unsigned long>(test_counter(perf_cntr), perf_cntr.get_milliseconds()));
	printf("performance_counter     : %lu\n", static_cast<unsigned long>(test_counter(perf_cntr), perf_cntr.get_milliseconds()));
	printf("highperformance_counter : %lu\n", static_cast<unsigned long>(test_counter(highperf_cntr), highperf_cntr.get_milliseconds()));
	printf("threadtimes_counter     : %lu\n", static_cast<unsigned long>(test_counter(thread_cntr), thread_cntr.get_milliseconds()));
	printf("processtimes_counter    : %lu\n", static_cast<unsigned long>(test_counter(process_cntr), process_cntr.get_milliseconds()));
	printf("tick_counter            : %lu\n", static_cast<unsigned long>(test_counter(tick_cntr), tick_cntr.get_milliseconds()));
	printf("multimedia_counter      : %lu\n", static_cast<unsigned long>(test_counter(media_cntr), media_cntr.get_milliseconds()));

	return 0;
}
