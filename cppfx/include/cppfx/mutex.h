#pragma once
#ifndef __CPPFX_MUTEX_H
#define __CPPFX_MUTEX_H

#include <cppfx/config.h>
#include <cppfx/export.h>

#ifdef CPPFX_THREADING
#include <mutex>
#endif

namespace cppfx {
	template<typename TMutex>
	inline void lock_mutex(TMutex&) {
	}
	template<typename TMutex>
	inline void unlock_mutex(TMutex&) {
	}

#ifdef CPPFX_THREADING
	class CPPFX_API mutex {
	public:
		mutex();
		~mutex();

		void lock();
		void unlock();
	private:
		mutex(const mutex&);
		mutex& operator = (const mutex&);
		void* m;
	};

	template <> inline void lock_mutex<mutex>(mutex& m) {
		m.lock();
	}
	template <> inline void unlock_mutex<mutex>(mutex& m) {
		m.unlock();
	}
#else
	struct CPPFX_API mutex {
		mutex();
		~mutex();

		void lock();
		void unlock();
	};
#endif

	template<typename TMutex = mutex>
	class mutex_scope {
	public:
		mutex_scope(TMutex& m) : mutex(m) { lock_mutex<TMutex>(m); }
		~mutex_scope() { unlock_mutex<TMutex>(mutex); }
	private:
		TMutex& mutex;
		mutex_scope(const mutex_scope&);
		mutex_scope& operator = (const mutex_scope&);
	};
}

#endif