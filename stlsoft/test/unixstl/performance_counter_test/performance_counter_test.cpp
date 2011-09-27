
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <unixstl.h>

#include <unixstl_performance_counter.h>

unixstl_ns_using(performance_counter)

#include <stdio.h>

/* ////////////////////////////////////////////////////////////////////////// */


int main(int /* argc */, char ** /* argv */)
{
	performance_counter	counter;

	counter.start();

	for(volatile int i = 0; i < 0x7fffffff; ++i)
	{}

	counter.stop();

	performance_counter::interval_type	period_count	=	counter.get_period_count();
	performance_counter::interval_type	seconds			=	counter.get_seconds();
	performance_counter::interval_type	milliseconds	=	counter.get_milliseconds();
	performance_counter::interval_type	microseconds	=	counter.get_microseconds();

	printf(	"period: %lld %llds %lldms %lldus\n",
			period_count,
			seconds,
			milliseconds,
			microseconds);

	return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
