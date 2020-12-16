/*
https://godbolt.org/z/7bn9r5
*/

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#define __STDC_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1

#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
 (c) 2020 by dbj@dbj.org
 LICENSE_DBJ -- https://dbj.org/license_dbj/
 valstat(tm) and metastate(tm) are protected trade marks

std-proposals discussion -- implement this with valstat

 std::optional<std::string> FindUsersCity() {
   std::optional<ContactsServer> contacts = GetOrOpenContactsServerConnection();
   std::optional<UserId>         uid      = contacts.GetOrReturnNullOpt()->GetUserId();
   std::optional<GeoServer>      geo      = GetOrOpenGeoServerConnection();
   std::optional<Location>       uloc     = geo.GetOrReturnNullOpt()->GetLocation(*uid);
   return uloc.GetOrReturnNullOpt()->GetCityName();
}
   where GetOrReturnNullOpt() is a method that
   (1) either returns a value if std::optional is not empty,
   (2) or returns from FindUsersCity() if std::optional is empty.
*/
#undef  DBJ_CPP03
#undef  DBJ_CPP11
#undef  DBJ_CPP14
#undef  DBJ_CPP17
#undef  DBJ_CPP20

/*	199711L(until C++11) */
#if   (__cplusplus == 199711)
#define DBJ_CPP03
#elif (__cplusplus == 201103)
#define DBJ_CPP11
#elif (__cplusplus == 201402)
#define DBJ_CPP14
#elif (__cplusplus == 201703)
#define DBJ_CPP17
#elif (__cplusplus == 202002)
#define DBJ_CPP20
#else
#error Unsuported C++ version
#endif

//
// system_error is legacy + technical debt
// in reality one can implement her own 
// auto ecs_ =  std::make_error_code( ec_ ).message() ;
// easily
//
#include<system_error> 
#include<optional> 
#include<vector> 

using std::string ;
using buff_t = std::vector<char> ;

#undef  SX
#define SX(fmt_,x_) fprintf( stdout, "\n%s : " fmt_, #x_, (x_) )

static string errc_str ( std::errc ec_, const char * user_msg_ )
{
    auto ecs_ =  std::make_error_code( ec_ ).message() ;

    size_t size_required = snprintf( nullptr,0, "%s" , user_msg_);
    buff_t buf ( size_required, char(0) );
    std::snprintf( buf.data(), buf.size(), "%s" , user_msg_ );
    return ecs_.append(buf.data());
}

static auto initialize_once = []() { 
        srand((unsigned)time(0));
        return true; 
    } ();

// random 0/1 flip
// srand() is called above
#define RANDOM_0_OR_1 bool(rand() % 2)

/*
    // for tight runtimes
    // this depends on two facts
    // (value) yields true for value != null
    // (status) yields true if status != null
	template<typename T> struct valstat final {
		T* value;
        // by using the outcome message
        // instead of some specific status and its type 
        // we are completely decoupled from the 
        // HOW is this message made
        // for traceability one might mandate
        // JSON format for status messages
        // obviously mandate is never to free status messages
 		const char * status;
	};

    // one could aid the key metastate attribute
    // and that is one of emptiness
    inline bool empty_value = [] ( auto vstat_ ) { return vstat_->value != nullptr ; };

    inline bool empty_status = [] ( auto vstat_ ) { return vstat_->status != nullptr ; };

*/
/* 
    full fat version 
*/
template<typename T> struct valstat final {
    std::optional<T> value;
/* 
for this demo it is ok to "carry arround" instances of std::string 
prargmatic leaner ans faster solution is vector<char>
otherwise use the "lean valstat" as above
*/
    std::optional<std::string> status;
};

    // bellow we will be deliberately meandering between various
    // types when composing various adhoc valstat's
    // this is to show its maleability
    // and composability  

	struct UserId         final {};
	struct ContactsServer final {
		valstat<UserId>    GetUserId() {
			static UserId uid_;
			// random flip OK or ERROR metestate return 
			if (RANDOM_0_OR_1)	return { uid_, {} };
			return { {}, errc_str( std::errc::owner_dead, " -- while trying to obtain user id.") };
		}
	};

	struct Location       final
	{
    // we are deliberately meandering between various
    // types when composing the various valstat's
    // this is to show its maleability
    // and composability
    // remember we have choosen valstat to be struct of two std options
    // where second option (aka status) is predefined to hold string
		valstat<const char *>  GetCityName() { return { "Valhala", {} }; }
	};
	struct GeoServer      final
	{
		valstat<Location>  GetLocation(UserId* uid) {
			static Location loc_;
			if (RANDOM_0_OR_1) return { loc_ , {} }; 
            return { {} , errc_str( std::errc::state_not_recoverable, " --  upon getting the location. " )};
		}
	};

	valstat<ContactsServer> GetOrOpenContactsServerConnection() noexcept {
		static ContactsServer cs{}; if (RANDOM_0_OR_1) return{ cs, {} }; 
        return { {}, errc_str( std::errc::protocol_error, " --  while trying to get or open the server connection" ) };
	}

	valstat<GeoServer>      GetOrOpenGeoServerConnection() noexcept {
		static GeoServer gs_;
		if (RANDOM_0_OR_1) return { gs_, {} }; 
        return { {}, errc_str( std::errc::protocol_not_supported, " -- Could not open geo server connection") };
	}

// call actually returns on error
// and error is "value field is empty"
// note the absolute key and core of the trick here
// we do not look into the value, thus we do not need 
// to define what does it mean "empty" for each and every
// type, value type might be
// instead we just check if field potentially holding it is empty
// notice we can return as we do because we know valstat is return typee
// and valstat is an record of two fields
#define return_on_no_value(VS) if ( ! VS.value ) return {{}, VS.status}
#define call( VS, F ) auto VS = F ;	return_on_no_value(VS)

// return type is record of two fields aka valstat
valstat<std::string> FindUsersCity() noexcept
{
    call( contacts , GetOrOpenContactsServerConnection() );
    call( uid , contacts.value->GetUserId() );
    call( geo , GetOrOpenGeoServerConnection() ) ;
    call( uloc , geo.value->GetLocation( & * uid.value) );
    call( cityname, (uloc.value)->GetCityName() );
    // all the call's above do return_on_no_value
    // thus the value must exist here 
    return { { *cityname.value } , {}} ;
}

#undef call
#undef return_on_no_value
    
	static int test_()
	{
		int id_ = 0;

        // loop until conditions are right
		do {
			id_ += 1;
			auto [city_name, errc] = FindUsersCity();

			if (city_name) {
				SX("%s", city_name->data());
				break;
			}

			if (errc) 
				SX( "%s", errc->data() );

		} while (true);

		return id_;
	}

int main () {

		for (unsigned k = 0; k < 10; ++k)
		{
            // test "tries" until city is found
			int tries = test_();
			printf("\n%d tries", tries);
			printf("\n---------------------------------");
		}
}