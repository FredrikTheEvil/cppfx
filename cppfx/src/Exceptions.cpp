#include <cppfx/Exceptions.h>

namespace cppfx
{
	Exception::Exception(string&& str) :
		msg(str)
	{
	}
	Exception::Exception(const string& str) :
		msg(str)
	{
	}

	Exception::Exception(const char* str) :
		msg(str)
	{
	}

	Exception::Exception(const Exception& e) :
		msg(e.msg)
	{
	}
	Exception::Exception(Exception&& e) :
		msg(std::move(e.msg))
	{
	}

	Exception& Exception::operator=(const Exception& e)
	{
		msg = e.msg;
		return *this;
	}
	Exception& Exception::operator=(Exception&& e)
	{
		msg = std::move(e.msg);
		return *this;
	}
	const string& Exception::what()
	{
		return msg;
	}
	RuntimeError::RuntimeError(string&& str) :
		Exception(str)
	{	
	}
	RuntimeError::RuntimeError(const string& str) :
		Exception(str)
	{
	}
	RuntimeError::RuntimeError(const char* str) :
		Exception(str)
	{
	}

	RuntimeError::RuntimeError(const RuntimeError& e) :
		Exception(e.msg)
	{
	}
	RuntimeError::RuntimeError(RuntimeError&& e) :
		Exception(std::move(e.msg))
	{
	}

	RuntimeError& RuntimeError::operator=(const RuntimeError& e)
	{
		msg = e.msg;
		return *this;
	}

	RuntimeError& RuntimeError::operator=(RuntimeError&& e)
	{
		msg = std::move(e.msg);
		return *this;
	}



	OutOfMemoryException::OutOfMemoryException(string&& str) :
		RuntimeError(str)
	{
	}
	OutOfMemoryException::OutOfMemoryException(const string& str) :
		RuntimeError(str)
	{
	}
	OutOfMemoryException::OutOfMemoryException(const char* str) :
		RuntimeError(str)
	{
	}

	OutOfMemoryException::OutOfMemoryException(const OutOfMemoryException& e) :
		RuntimeError(e.msg)
	{
	}
	OutOfMemoryException::OutOfMemoryException(OutOfMemoryException&& e) :
		RuntimeError(std::move(e.msg))
	{
	}

	OutOfMemoryException& OutOfMemoryException::operator=(const OutOfMemoryException& e)
	{
		msg = e.msg;
		return *this;
	}

	OutOfMemoryException& OutOfMemoryException::operator=(OutOfMemoryException&& e)
	{
		msg = std::move(e.msg);
		return *this;
	}
}