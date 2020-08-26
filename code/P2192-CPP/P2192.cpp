#define _CRT_SECURE_NO_WARNINGS

/*
(c) 2020 by dbj@dbj.org CC BY SA 4.0
*/
//#include "./lib/common.h"
//#include "./lib/http_status_codes.h"
#include "dbj_valstat.h"

#include "applicability.h"
#include "modern_fopen.h"
#include "oo+valstat.h"

#ifdef __clang__
#define  PURE_FUNCTION __attribute__((const))
#else
#define  PURE_FUNCTION 
#endif


static void ad_hoc_testing()
{
	using std::cout;

	// value and status are value types
	// managed by std::optional
	auto [ value, errcode ] = dbj::valstat< int, int >{ {}, EINVAL };

	/* ERROR metastate: empty value and non empty status */
	if (! value && errcode ) {
		/* fetching the errcode value */
		cout << strerror( *errcode );
	}
}


void test_win32_valstat();

int main ( int , char ** argv )  
{
	ad_hoc_testing();

	stl_valstat_test();
	oo_valstat_test();

	test_modern_fopen();
	// 
	dbj::experimental::test_reference_with_state();

	test_win32_valstat();
	dbj::valstat_testing::test_constructor_usage();

	dbj::valstat_testing::test_shiny_foot_gun();

	//archived
	//test_using_two_pointers();
	//test_using_two_optionals();
 
  return EXIT_SUCCESS ;
}