#pragma once
#ifndef __CPPFX_STRING_H
#define __CPPFX_STRING_H

#include <cppfx/stl_export.h>
#include <type_traits>
#include <string>

namespace cppfx {
	CPPFX_STL template class CPPFX_API std::allocator < char > ;
	CPPFX_STL template class CPPFX_API std::allocator < wchar_t >;
	CPPFX_STL template class CPPFX_API std::allocator < char16_t >;
	CPPFX_STL template class CPPFX_API std::allocator < char32_t >;
	CPPFX_STL template class CPPFX_API std::basic_string< char, std::char_traits<char>, std::allocator<char> >;
	CPPFX_STL template class CPPFX_API std::basic_string < char16_t, std::char_traits<char16_t>, std::allocator<char16_t> > ;
	CPPFX_STL template class CPPFX_API std::basic_string < char32_t, std::char_traits<char32_t>, std::allocator<char32_t> >;
	CPPFX_STL template class CPPFX_API std::basic_string < wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > ;

	typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > string;
	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > wstring;
	typedef std::basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t> > string_utf16;
	typedef std::basic_string<char32_t, std::char_traits<char32_t>, std::allocator<char32_t> > string_utf32;
	
	template <typename String> struct string_traits {
		typedef char char_type;
		typedef char* pointer;
		typedef char* iterator;
		typedef const char* const_iterator;
		typedef const char* const_pointer;
	};
	template <> struct string_traits<const char*>
	{
		typedef char char_type;
		typedef char* pointer;
		typedef char* iterator;
		typedef const char* const_iterator;
		typedef const char* const_pointer;
	};
	template <> struct string_traits<std::string>
	{
		typedef std::string::value_type char_type;
		typedef std::string::pointer pointer;
		typedef std::string::iterator iterator;
		typedef std::string::const_pointer const_pointer;
		typedef std::string::const_iterator const_iterator;
	};
	template <> struct string_traits<std::u16string>
	{
		typedef std::u16string::value_type char_type;
		typedef std::u16string::pointer pointer;
		typedef std::u16string::iterator iterator;
		typedef std::u16string::const_pointer const_pointer;
		typedef std::u16string::const_iterator const_iterator;
	};
	template <> struct string_traits<std::u32string>
	{
		typedef std::u32string::value_type char_type;
		typedef std::u32string::pointer pointer;
		typedef std::u32string::iterator iterator;
		typedef std::u32string::const_pointer const_pointer;
		typedef std::u32string::const_iterator const_iterator;
	};

	// Always use a specialization of this template when you need to get a char* pointer from
	// a arbitrary string type. This ensures no modification of code is needed anywhere if
	// the string implementation changes, just a proper template specialization
	template <typename String> inline typename string_traits<String>::const_pointer get_string_buffer(const String& str)
	{
		CPPFX_STATIC_ASSERT(false);
	}
	template <> inline string_traits<string>::const_pointer get_string_buffer<string>(const string& str) {
		return str.data();
	}

	template<typename Source, typename Target>
	inline Target convert_string(const Source&)
	{
		CPPFX_STATIC_ASSERT(false);
	}
	template<>
	inline string convert_string<string, string>(const string& source) 
	{
		return source;
	}
	template<>
	inline string convert_string<string_utf16, string>(const string_utf16& source)
	{
	}
	template<>
	inline string convert_string<string_utf32, string>(const string_utf32& source)
	{
	}
	template<>
	inline string_utf16 convert_string<string, string_utf16>(const string& source)
	{
	}
	template<>
	inline string_utf16 convert_string<string_utf16, string_utf16>(const string_utf16& source)
	{
		return source;
	}
	template<>
	inline string_utf16 convert_string<string_utf32, string_utf16>(const string_utf32& source)
	{
	}
	template<>
	inline string_utf32 convert_string<string, string_utf32>(const string& source)
	{
	}
	template<>
	inline string_utf32 convert_string<string_utf16, string_utf32>(const string_utf16& source)
	{
	}
	template<>
	inline string_utf32 convert_string<string_utf32, string_utf32>(const string_utf32& source)
	{
		return source;
	}


}

#endif