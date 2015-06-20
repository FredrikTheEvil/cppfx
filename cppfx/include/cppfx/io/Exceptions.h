#pragma once
#ifndef __CPPFX_IO_EXCEPTIONS_H
#define __CPPFX_IO_EXCEPTIONS_H
#include <cppfx/Exceptions.h>

namespace cppfx
{
	namespace io
	{
		class CPPFX_API IoException : public Exception
		{
		public:
			explicit IoException(string&& str);
			explicit IoException(const string& str);
			explicit IoException(const char* str);
			IoException(const IoException& e);;
			IoException(IoException&& e);
			IoException& operator = (const IoException&);
			IoException& operator = (IoException&&);
		};
		class CPPFX_API EofException : public IoException
		{
		public:
			explicit EofException(string&& str);
			explicit EofException(const string& str);
			explicit EofException(const char* str);
			EofException(const EofException& e);;
			EofException(EofException&& e);
			EofException& operator = (const EofException&);
			EofException& operator = (EofException&&);
		};
		class CPPFX_API FileNotFoundException : public IoException
		{
		public:
			explicit FileNotFoundException(string&& str);
			explicit FileNotFoundException(const string& str);
			explicit FileNotFoundException(const char* str);
			FileNotFoundException(const FileNotFoundException& e);;
			FileNotFoundException(FileNotFoundException&& e);
			FileNotFoundException& operator = (const FileNotFoundException&);
			FileNotFoundException& operator = (FileNotFoundException&&);
		};
		class CPPFX_API BadFileFormatException : IoException
		{
		public:
			explicit BadFileFormatException(string&& str);
			explicit BadFileFormatException(const string& str);
			explicit BadFileFormatException(const char* str);
			BadFileFormatException(const BadFileFormatException& e);;
			BadFileFormatException(BadFileFormatException&& e);
			BadFileFormatException& operator = (const BadFileFormatException&);
			BadFileFormatException& operator = (BadFileFormatException&&);
		};
	}
}

#endif