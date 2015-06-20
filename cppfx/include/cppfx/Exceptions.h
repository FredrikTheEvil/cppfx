#pragma once
#ifndef __CPPFX_EXCEPTIONS_H
#define __CPPFX_EXCEPTIONS_H
#include <cppfx/string.h>

namespace cppfx
{
	class CPPFX_API Exception
	{
	public:
		explicit Exception(string&& str);
		explicit Exception(const string& str);
		explicit Exception(const char* str);
		Exception(const Exception& e);;
		Exception(Exception&& e);
		Exception& operator = (const Exception&);
		Exception& operator = (Exception&&);
		const string& what();
	protected:
		string msg;
	};
	class CPPFX_API RuntimeError : public Exception
	{
	public:
		explicit RuntimeError(string&& str);
		explicit RuntimeError(const string& str);
		explicit RuntimeError(const char* str);
		RuntimeError(const RuntimeError& e);;
		RuntimeError(RuntimeError&& e);
		RuntimeError& operator = (const RuntimeError&);
		RuntimeError& operator = (RuntimeError&&);
	};
	class CPPFX_API OutOfMemoryException : public RuntimeError
	{
	public:
		explicit OutOfMemoryException(string&& str);
		explicit OutOfMemoryException(const string& str);
		explicit OutOfMemoryException(const char* str);
		OutOfMemoryException(const OutOfMemoryException& e);;
		OutOfMemoryException(OutOfMemoryException&& e);
		OutOfMemoryException& operator = (const OutOfMemoryException&);
		OutOfMemoryException& operator = (OutOfMemoryException&&);
	};
}

#endif