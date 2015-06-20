#include <cppfx/io/Exceptions.h>

namespace cppfx
{
	namespace io
	{
		IoException::IoException(string&& str) :
			Exception(str)
		{
		}
		IoException::IoException(const string& str) :
			Exception(str)
		{
		}
		IoException::IoException(const char* str) :
			Exception(str)
		{
		}

		IoException::IoException(const IoException& e) :
			Exception(e.msg)
		{
		}
		IoException::IoException(IoException&& e) :
			Exception(std::move(e.msg))
		{
		}

		IoException& IoException::operator=(const IoException& e)
		{
			msg = e.msg;
			return *this;
		}

		IoException& IoException::operator=(IoException&& e)
		{
			msg = std::move(e.msg);
			return *this;
		}

		EofException::EofException(string&& str) :
			IoException(str)
		{
		}
		EofException::EofException(const string& str) :
			IoException(str)
		{
		}
		EofException::EofException(const char* str) :
			IoException(str)
		{
		}

		EofException::EofException(const EofException& e) :
			IoException(e.msg)
		{
		}
		EofException::EofException(EofException&& e) :
			IoException(std::move(e.msg))
		{
		}

		EofException& EofException::operator=(const EofException& e)
		{
			msg = e.msg;
			return *this;
		}

		EofException& EofException::operator=(EofException&& e)
		{
			msg = std::move(e.msg);
			return *this;
		}
		FileNotFoundException::FileNotFoundException(string&& str) :
			IoException("file not found: " + str)
		{
		}
		FileNotFoundException::FileNotFoundException(const string& str) :
			IoException("file not found: " + str)
		{
		}
		FileNotFoundException::FileNotFoundException(const char* str) :
			IoException(string("file not found: ") + str)
		{
		}

		FileNotFoundException::FileNotFoundException(const FileNotFoundException& e) :
			IoException(e.msg)
		{
		}
		FileNotFoundException::FileNotFoundException(FileNotFoundException&& e) :
			IoException(std::move(e.msg))
		{
		}

		FileNotFoundException& FileNotFoundException::operator=(const FileNotFoundException& e)
		{
			msg = e.msg;
			return *this;
		}
		FileNotFoundException& FileNotFoundException::operator=(FileNotFoundException&& e)
		{
			msg = std::move(e.msg);
			return *this;
		}
		BadFileFormatException::BadFileFormatException(string&& str) :
			IoException(str)
		{
		}
		BadFileFormatException::BadFileFormatException(const string& str) :
			IoException(str)
		{
		}
		BadFileFormatException::BadFileFormatException(const char* str) :
			IoException(str)
		{
		}

		BadFileFormatException::BadFileFormatException(const BadFileFormatException& e) :
			IoException(e.msg)
		{
		}
		BadFileFormatException::BadFileFormatException(BadFileFormatException&& e) :
			IoException(std::move(e.msg))
		{
		}

		BadFileFormatException& BadFileFormatException::operator=(const BadFileFormatException& e)
		{
			msg = e.msg;
			return *this;
		}

		BadFileFormatException& BadFileFormatException::operator=(BadFileFormatException&& e)
		{
			msg = std::move(e.msg);
			return *this;
		}
	}
}