
#include "../common/common.h"
#include "dbj_valstat.h"
#include <winerror.h> 
// from above: #define HRESULT_CODE(hr)    ((hr) & 0xFFFF)

#include <system_error>
#include <string>

	template<typename T>
	using valstat_hresult = dbj::valstat<T, HRESULT>;

	void test_win32_valstat() {

		// info state, value and status
		valstat_hresult<int> info_ = { 42,  ((HRESULT)0x00000000L) };

		SX(
			std::system_category().message(*info_.status).data()
		);
	}

