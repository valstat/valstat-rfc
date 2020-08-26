#pragma once
/*
(c) 2020 by dbj@dbj.org CC BY SA 4.0
*/
#include "lib/common.h"
#include "dbj_valstat.h"
#include "lib/http_status_codes.h"

namespace dbj::valstat_sampling {

	inline http_status_code http_endpoint () {

		while (true) {
			unsigned int probe = rand_interval(http_status_code_first_enum, http_status_code_last_enum);

			if (
				auto [value, status] = http_status_string(http_status_code(probe))
				; value)
				return  http_status_code(probe);
		}
	}

	inline dbj::valstat< std::string, http_status_code >
		fetch_response(std::string_view url) {

		http_status_code http_code = http_endpoint();

		// error metatstate
		if (http_status_is_error(http_code))
			return { {}, http_code };
	}

} // dbj::valstat_sampling
