//  filesystem path_traits.cpp  --------------------------------------------------------//

//  Copyright Beman Dawes 2008, 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------//

#include <boost/config.hpp>
#if !defined( BOOST_NO_STD_WSTRING )
// Boost.Filesystem V3 and later requires std::wstring support.
// During the transition to V3, libraries are compiled with both V2 and V3 sources.
// On old compilers that don't support V3 anyhow, we just skip everything so the compile
// will succeed and the library can be built.

// define BOOST_FILESYSTEM_SOURCE so that <boost/system/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_FILESYSTEM_SOURCE 

#ifndef BOOST_SYSTEM_NO_DEPRECATED 
# define BOOST_SYSTEM_NO_DEPRECATED
#endif

#include <boost/filesystem/v3/config.hpp>
#include <boost/filesystem/v3/path_traits.hpp>
#include <boost/system/system_error.hpp>
#include <boost/scoped_array.hpp>
#include <locale>   // for codecvt_base::result
#include <cstring>  // for strlen
#include <cwchar>   // for wcslen

namespace pt = pdalboost::filesystem3::path_traits;
namespace fs = pdalboost::filesystem3;
namespace bs = pdalboost::system;

//--------------------------------------------------------------------------------------//
//                                  configuration                                       //
//--------------------------------------------------------------------------------------//

#ifndef BOOST_FILESYSTEM_CODECVT_BUF_SIZE
# define BOOST_FILESYSTEM_CODECVT_BUF_SIZE 256
#endif

namespace {

  const std::size_t default_codecvt_buf_size = BOOST_FILESYSTEM_CODECVT_BUF_SIZE;


//--------------------------------------------------------------------------------------//
//                                                                                      //
//  The public convert() functions do buffer management, and then forward to the        //
//  convert_aux() functions for the actual call to the codecvt facet.                   //
//                                                                                      //
//--------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------//
//                      convert_aux const char* to wstring                             //
//--------------------------------------------------------------------------------------//

  void convert_aux(
                   const char* from,
                   const char* from_end,
                   wchar_t* to, wchar_t* to_end,
                   std::wstring & target,
                   const pt::codecvt_type & cvt)
  {
    //std::cout << std::hex
    //          << " from=" << std::size_t(from)
    //          << " from_end=" << std::size_t(from_end)
    //          << " to=" << std::size_t(to)
    //          << " to_end=" << std::size_t(to_end)
    //          << std::endl;

    std::mbstate_t state  = std::mbstate_t();  // perhaps unneeded, but cuts bug reports
    const char* from_next;
    wchar_t* to_next;

    std::codecvt_base::result res;

    if ((res=cvt.in(state, from, from_end, from_next,
           to, to_end, to_next)) != std::codecvt_base::ok)
    {
      //std::cout << " result is " << static_cast<int>(res) << std::endl;
      BOOST_FILESYSTEM_THROW(bs::system_error(res, fs::codecvt_error_category(),
        "pdalboost::filesystem::path codecvt to wstring"));
    }
    target.append(to, to_next); 
  }

//--------------------------------------------------------------------------------------//
//                      convert_aux const wchar_t* to string                           //
//--------------------------------------------------------------------------------------//

  void convert_aux(
                   const wchar_t* from,
                   const wchar_t* from_end,
                   char* to, char* to_end,
                   std::string & target,
                   const pt::codecvt_type & cvt)
  {
    //std::cout << std::hex
    //          << " from=" << std::size_t(from)
    //          << " from_end=" << std::size_t(from_end)
    //          << " to=" << std::size_t(to)
    //          << " to_end=" << std::size_t(to_end)
    //          << std::endl;

    std::mbstate_t state  = std::mbstate_t();  // perhaps unneeded, but cuts bug reports
    const wchar_t* from_next;
    char* to_next;

    std::codecvt_base::result res;

    if ((res=cvt.out(state, from, from_end, from_next,
           to, to_end, to_next)) != std::codecvt_base::ok)
    {
      //std::cout << " result is " << static_cast<int>(res) << std::endl;
      BOOST_FILESYSTEM_THROW(bs::system_error(res, fs::codecvt_error_category(),
        "pdalboost::filesystem::path codecvt to string"));
    }
    target.append(to, to_next); 
  }
  
}  // unnamed namespace

//--------------------------------------------------------------------------------------//
//                                   path_traits                                        //
//--------------------------------------------------------------------------------------//

namespace pdalboost{} namespace boost = pdalboost; namespace pdalboost{ namespace filesystem3 { namespace path_traits {

//--------------------------------------------------------------------------------------//
//                          convert const char* to wstring                             //
//--------------------------------------------------------------------------------------//

  BOOST_FILESYSTEM_DECL
  void convert(const char* from,
                const char* from_end,    // 0 for null terminated MBCS
                std::wstring & to,
                const codecvt_type & cvt)
  {
    BOOST_ASSERT(from);

    if (!from_end)  // null terminated
    {
      from_end = from + std::strlen(from);
    }

    if (from == from_end) return;

    std::size_t buf_size = (from_end - from) * 3;  // perhaps too large, but that's OK

    //  dynamically allocate a buffer only if source is unusually large
    if (buf_size > default_codecvt_buf_size)
    {
      pdalboost::scoped_array< wchar_t > buf(new wchar_t [buf_size]);
      convert_aux(from, from_end, buf.get(), buf.get()+buf_size, to, cvt);
    }
    else
    {
      wchar_t buf[default_codecvt_buf_size];
      convert_aux(from, from_end, buf, buf+default_codecvt_buf_size, to, cvt);
    }
  }

//--------------------------------------------------------------------------------------//
//                         convert const wchar_t* to string                            //
//--------------------------------------------------------------------------------------//

  BOOST_FILESYSTEM_DECL
  void convert(const wchar_t* from,
                const wchar_t* from_end,  // 0 for null terminated MBCS
                std::string & to,
                const codecvt_type & cvt)
  {
    BOOST_ASSERT(from);

    if (!from_end)  // null terminated
    {
      from_end = from + std::wcslen(from);
    }

    if (from == from_end) return;

    //  The codecvt length functions may not be implemented, and I don't really
    //  understand them either. Thus this code is just a guess; if it turns
    //  out the buffer is too small then an error will be reported and the code
    //  will have to be fixed.
    std::size_t buf_size = (from_end - from) * 4;  // perhaps too large, but that's OK
    buf_size += 4;  // encodings like shift-JIS need some prefix space

    //  dynamically allocate a buffer only if source is unusually large
    if (buf_size > default_codecvt_buf_size)
    {
      pdalboost::scoped_array< char > buf(new char [buf_size]);
      convert_aux(from, from_end, buf.get(), buf.get()+buf_size, to, cvt);
    }
    else
    {
      char buf[default_codecvt_buf_size];
      convert_aux(from, from_end, buf, buf+default_codecvt_buf_size, to, cvt);
    }
  }
}}} // namespace pdalboost::filesystem3::path_traits

#endif  // no wide character support
