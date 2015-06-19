#pragma once
#ifndef __CPPFX_REFERENCED_H
#define __CPPFX_REFERENCED_H

#include <cppfx/config.h>
#include <cppfx/export.h>
#include <cppfx/mutex.h>

namespace cppfx {
	class DeleteHandler;
	class Observer;
	class ObserverSet;

	template<typename T, T M()>
	struct depends_on {
		depends_on() { M(); }
	};

	class CPPFX_API Referenced {
	public:
		Referenced();
		explicit Referenced(bool threadSafe);
		Referenced(const Referenced&);
		inline Referenced& operator = (const Referenced&) { return *this; }
		virtual void setThreadSafeRefUnref(bool threadSafe);
		bool getThreadSafeRefUnref() const { return _refMutex != nullptr; }
		mutex* getRefMutex() const { return _refMutex; }
		static mutex* getGlobalReferencedMutex();
		virtual int ref() const;
		virtual int unref() const;
		int unref_no_delete() const;
		inline int refcount() const { return _refCount; }
		ObserverSet* setObserverSet() const {
			return static_cast<ObserverSet*>(_observerSet);
		}
		ObserverSet* getOrCreateObserverSet() const;
		void addObserver(Observer* observer) const;
		void removeObserver(Observer* observer) const;
		static void setThreadSafeReferenceCounting(bool enableThreadSaferefCounting);
		static bool getThreadSafeReferenceCounting();
		static void setDeleteHandler(DeleteHandler* handler);
		static DeleteHandler* getDeleteHandler();
		friend class DeleteHandler;
	protected:
		virtual ~Referenced();
		void signalObserversAndDelete(bool signalDelete, bool doDelete) const;
		void deleteUsingDeleteHandler() const;
		mutable mutex* _refMutex;
		mutable int _refCount;
		mutable void* _observerSet;
	};

	inline int Referenced::ref() const {
		if (_refMutex) {
			mutex_scope<> scope(*_refMutex);
			return ++_refCount;
		}
		else {
			return ++_refCount;
		}
	}

	inline int Referenced::unref() const {
		int newref;
		bool needDelete = false;
		if (_refMutex) {
			mutex_scope<> scope(*_refMutex);
			newref = --_refCount;
			needDelete = newref == 0;
		}
		else {
			newref = --_refCount;
			needDelete = newref == 0;
		}
		if (needDelete) {
			signalObserversAndDelete(true, true);
		}
		return newref;
	}
} // namespace cppfx

namespace boost {
	inline void intrusive_ptr_add_ref(cppfx::Referenced* p) { p->ref(); }
	inline void intrusive_ptr_release(cppfx::Referenced* p) { p->unref(); }
}  // namespace boost

#endif