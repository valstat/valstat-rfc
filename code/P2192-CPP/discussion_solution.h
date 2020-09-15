#pragma once
/*
 immplement this with valstat

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

#include <xerrc.h>
#include<stdio.h>
#include<stdlib.h>
#define __STDC_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__
#include<string.h>

#include<system_error>

// random 0/1 flip
#define RF bool(rand() % 2)

namespace wg21 {

	template<typename T> struct valstat final {
		T* value;
		std::errc status;
	};
	struct UserId         final	{};
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
		valstat<const char>  GetCityName() { return { "Valhala", {} };  }
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

	valstat<const char> FindUsersCity() noexcept
	{
		if (auto [contacts, errc] = GetOrOpenContactsServerConnection(); contacts)
		{
			if (auto [uid, errc] = contacts->GetUserId(); uid) {
				if (auto [geo, errc] = GetOrOpenGeoServerConnection(); geo) {
					if (auto [uloc, errc] = geo->GetLocation(uid); uloc) {
						return uloc->GetCityName();
					}
					else {
						return { {}, errc };
					}
				}
				else {
					return { {}, errc };
				}
			}
			else {
				return { {}, errc };
			}
		}
		else {
			return { {}, errc };
		}
	}

	void test_valsta_using_wg21_debate()
	{
		auto [city_name, errc] = FindUsersCity();

			if (city_name)
				printf("\nCity name: %s", city_name);

			if (int(errc) > 0) {
				std::error_code ec_ = make_error_code(errc);
				printf("\nError (#%3d) '%s'", unsigned(errc), ec_.message().c_str() );
			}
	}

} // wg21 NS