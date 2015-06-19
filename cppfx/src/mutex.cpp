#include <cppfx/mutex.h>

namespace cppfx {
#ifdef CPPFX_THREADING
	mutex::mutex() { m = new std::mutex(); }
	mutex::~mutex() { delete static_cast<std::mutex*>(m); }
	void mutex::lock() {
		static_cast<std::mutex*>(m)->lock();
	}
	void mutex::unlock() {
		static_cast<std::mutex*>(m)->unlock();
	}
#else
	mutex::mutex() {}
	mutex::~mutex() {}
	void mutex::lock() {}
	void mutex::unlock() {}
#endif
}