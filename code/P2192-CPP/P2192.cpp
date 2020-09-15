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
#include "discussion_solution.h"
#include "discussion_solution_chatty_version.h"

#ifdef __clang__
#define  PURE_FUNCTION __attribute__((const))
#else
#define  PURE_FUNCTION 
#endif


static void ad_hoc_testing()
{
	using std::cout;

	auto [name, errc] = wg21::FindUsersCity(); 

	if (name)
			cout << "OK or INFO metatstate, name is:" << name;
	if ( int(errc) > 0 ) {
		cout << "status: " << strerror( int(errc) );
	}
}


void test_win32_valstat();

int main ( int , char ** argv )  
{
	// wg21::test_valsta_using_wg21_debate();
	wg22::test();

#if 0

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
#endif // 0
  return EXIT_SUCCESS ;
}