### OS specific error codes

all of the major OS's are using C like, more or less elaborate, "specially coded" values as error codes. mixing valstat and OS specific error codes is obviously very simple. 

```cpp
// WIN32 API + valstat
#include <winerror.h>
#include <valstat>

	template<typename T>
	using valstat_hresult = std::valstat<T, HRESULT>;

	// info metastate, both value and status
	valstat_hresult<int> info_ = { 42,  ((HRESULT)0x00000000L) };
```
