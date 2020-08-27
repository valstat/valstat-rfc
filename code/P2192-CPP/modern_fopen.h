#pragma once
//
// https://gitlab.com/dbjdbj/valstat
// valstat concept simple sampling
// (c) 2020 by dbj@dbj.org , CC BY SA 4.0
//
#include "../common/common.h"
#include "dbj_valstat.h" // dbj::posix_valstat
#include <stdio.h>
#include <stdlib.h>

/// -------------------------------------------------------------------------
inline dbj::posix_valstat<FILE*> 
  modern_fopen(const char* name_, const char* mode_) 
noexcept
{
    FILE* fp_{};
    int ec_ = fopen_s(&fp_, name_, mode_);

    if (NULL == fp_)
        // returning the ERROR metastate
        // dbj::errc is internal enum with POSIX error codes
        return { {} ,  dbj::dbj_errc(ec_) }; 

    // returning the OK metastate
    return { fp_, {} }; // OK metastate
}

/// -------------------------------------------------------------------------
namespace {
    /// -------------------------------------------------------------------------
    inline void test_drive_modern_fopen(const char* file_name_, const char* mode_)
    {
        auto [fp_, errc_] = modern_fopen(file_name_, mode_);

        dbj::show_metastate("(" __FILE__ ") test_drive_modern_fopen", fp_, errc_ );

        //printf(
        //    "\n\nfile name \"%s\"\nmodern_fopen() returned metastate\n file pointer: %p\nposix error code: %d, message: \"%s\"",
        //    file_name_,
        //    (fp_ ? *fp_ : nullptr),
        //    (errc_ ? int(*errc_) : 0),
        //    (errc_ ? dbj::error_message(*errc_) : "not an error")
        //);
    }
    /// -------------------------------------------------------------------------
    /// _MSC_VER 
#include <io.h> // _mktemp_s
    static inline void test_modern_fopen(void)
    {
        char file_name_[] = "prefXXXXXX";
        test_drive_modern_fopen(file_name_, "r");
        _mktemp_s(file_name_, _countof(file_name_));
        test_drive_modern_fopen(file_name_, "w");
    }

} // nspace