// (c) 2020 by dbj@dbj.org CC BY SA 4.0
#define SEMVER "0.2.0"
#ifndef __clang__
#error Please use clang for the release 0.2.0
#endif

// archived --> #include "c_valstat_overengineered.h"
#include "c_valstat.h"

#include <stdio.h>
#include <stdlib.h>

void test_two();

int main( const int argc , const char ** argv )
{
	// canonical
	test_two();

	return EXIT_SUCCESS;
}
