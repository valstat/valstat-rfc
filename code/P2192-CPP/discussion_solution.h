#pragma once
/*
 (c) 2020 by dbj@dbj.org
 LICENSE_DBJ -- https://dbj.org/license_dbj/
 valstat(tm) and metastate(tm) are protected trade marks

 implement this with valstat

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

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#define __STDC_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1

#include <xerrc.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
// couild not make strerror / perror work? , ditto ...
#include<system_error> 
#include<errno.h> 

// random 0/1 flip
#define RF bool(rand() % 2)

namespace wg21 {

	template<typename T> struct valstat final {
		T* value;
		std::errc status;
	};
	struct UserId         final {};
	struct ContactsServer final {
		valstat<UserId>    GetUserId() {
			static UserId uid_;
			// random flip OK or ERROR metestate return 
			if (RF)	return { &uid_, {} };
			return { {}, std::errc::owner_dead };
		}
	};
	struct Location       final
	{
		valstat<const char>  GetCityName() { return { "Valhala", {} }; }
	};
	struct GeoServer      final
	{
		valstat<Location>  GetLocation(UserId* uid) {
			static Location loc_;
			if (RF) return { &loc_ , {} }; return { 0,  std::errc::state_not_recoverable };
		}
	};

	valstat<ContactsServer> GetOrOpenContactsServerConnection() noexcept {
		static ContactsServer cs{}; if (RF) return{ &cs, {} }; return { 0,  std::errc::protocol_error };
	}
	valstat<GeoServer>      GetOrOpenGeoServerConnection() noexcept {
		static GeoServer gs_;
		if (RF) return { &gs_, {} }; return { 0,  std::errc::protocol_not_supported };
	}

	inline namespace V1 {

#define check_return(VS) if ( int(VS.status) > 0) return {0, VS.status}
#define call( VS, F ) auto VS = F ;	check_return(VS)

		valstat<const char> FindUsersCity() noexcept
		{
			call( contacts , GetOrOpenContactsServerConnection() );
			call( uid , contacts.value->GetUserId() );
			call( geo , GetOrOpenGeoServerConnection() ) ;
			call( uloc , geo.value->GetLocation(uid.value) );
			return uloc.value ->GetCityName();
		}

#undef call
#undef check_return

	} // V1

	void test_valsta_using_wg21_debate()
	{
		srand((unsigned)time(0));
		for (int k{} ; k < 10; ++k) {
			auto [city_name, errc] = FindUsersCity();

			if (city_name)
				printf("\nCity name: %s", city_name);

			if (int(errc) > 0) {

				// works as advertised
				std::error_code ec_ = make_error_code(errc);
				printf("\nError (#%3d) '%s'", unsigned(errc), ec_.message().c_str());

				// does give "unknown error" all the time?
				errno = int(errc);
				perror("PERROR");
				errno = 0;
			}
		}
	}

} // wg21 NS