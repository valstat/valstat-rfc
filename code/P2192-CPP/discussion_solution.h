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

namespace wg21 {

	template<typename T> struct valstat final {
		T* value;
		std::errc status;
	};
	struct UserId         final
	{};
	struct ContactsServer final {
		valstat<UserId>    GetUserId() {
			static UserId uid_;
			return { &uid_, {} };
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
			return { &loc_ , {} };
		}
	};

	valstat<ContactsServer> GetOrOpenContactsServerConnection() noexcept {
		static ContactsServer cs{}; return{ &cs, {} };
	}
	valstat<GeoServer>      GetOrOpenGeoServerConnection() noexcept {
		static GeoServer gs_;
		return { &gs_, {} };
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
	}

} // wg21 NS