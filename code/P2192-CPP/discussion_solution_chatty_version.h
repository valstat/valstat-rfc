#pragma once
/*
 (c) 2020 by dbj@dbj.org
 LICENSE_DBJ -- https://dbj.org/license_dbj/
 valstat(tm) and metastate(tm) are protected trade marks

  https://godbolt.org/z/G4qKGq

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
#include <iostream>

namespace {
	inline auto initialize_once_2 = []() {
		std::ios::sync_with_stdio(true);
		std::cout << std::boolalpha; return true; 
	} ();
}

#undef  SX
#define SX(x_) do { std::cout << "\n" #x_ " : " << (x_); } while(0)

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
#include<system_error> 
#include<optional> 

inline auto errc_str(std::errc ec_, const char* user_msg_)
{
	auto ecs_ = std::make_error_code(ec_).message();

	size_t size_required = snprintf(nullptr, 0, "%s", user_msg_);
	std::string buf(size_required, char(0));
	snprintf(buf.data(), buf.size(), "%s", user_msg_);
	return ecs_.append(buf);
}

inline ::std::ostream& operator << (::std::ostream& os, std::errc ec_)
{
	std::error_code code = std::make_error_code(ec_);
	return os << code.message().c_str();
}


// random 0/1 flip
#undef RF
#define RF bool(rand() % 2)

namespace wg22 {
	/*
		A lean version
		template<typename T> struct valstat final {
			T* value;
			std::errc status;
		};
	*/
	/* full fat version */
	template<typename T> struct valstat final {
		std::optional<T> value;
		std::optional<std::string> status;
	};

	struct UserId         final {};
	struct ContactsServer final {
		valstat<UserId>    GetUserId() {
			static UserId uid_;
			// random flip OK or ERROR metestate return 
			if (RF)	return { uid_, {} };
			return { {}, errc_str(std::errc::owner_dead, " Error while trying to obtain user id.") };
		}
	};
	struct Location       final
	{
		// we are deliberately meandering between various
		// types when composing the valstat's
		valstat<const char*>  GetCityName() { return { "Valhala", {} }; }
	};
	struct GeoServer      final
	{
		valstat<Location>  GetLocation(UserId* uid) {
			static Location loc_;
			if (RF) return { loc_ , {} };
			return { {} , errc_str(std::errc::state_not_recoverable, " Error upon getting the location. ") };
		}
	};

	valstat<ContactsServer> GetOrOpenContactsServerConnection() noexcept {
		static ContactsServer cs{}; if (RF) return{ cs, {} };
		return { {}, errc_str(std::errc::protocol_error, " Error while trying to get or open the server connection") };
	}

	valstat<GeoServer>      GetOrOpenGeoServerConnection() noexcept {
		static GeoServer gs_;
		if (RF) return { gs_, {} };
		return { {}, errc_str(std::errc::protocol_not_supported, " Could not open geo server connection") };
	}

	namespace chatty {

#define check_return(VS) if ( ! VS.value ) return {{}, VS.status}
#define call( VS, F ) auto VS = F ;	check_return(VS)

		valstat<std::string> FindUsersCity() noexcept
		{
			call(contacts, GetOrOpenContactsServerConnection());
			call(uid, contacts.value->GetUserId());
			call(geo, GetOrOpenGeoServerConnection());
			call(uloc, geo.value->GetLocation(&*uid.value));
			call(cityname, (uloc.value)->GetCityName());
			return { { *cityname.value } , {} };
		}

#undef call
#undef check_return

	} // chatty

	static int test_()
	{
		int id_ = 0;
		do {
			id_ += 1;
			auto [city_name, errc] = chatty::FindUsersCity();

			if (city_name) {
				SX(*city_name);
				break;
			}

			if (errc)
				SX(*errc);
		} while (true);
		return id_;
	}


	inline void test() {

		srand((unsigned)time(0));

		for (unsigned k = 0; k < 10; ++k)
		{
			int tries = test_();
			printf("\n%d tries", tries);
			printf("\n---------------------------------");
		}
	}

} // wg22 NS

