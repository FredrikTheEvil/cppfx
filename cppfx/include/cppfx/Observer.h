#pragma once
#ifndef __CPPFX_OBSERVER_H
#define __CPPFX_OBSERVER_H

#include <cppfx/Referenced.h>

namespace cppfx {
	class CPPFX_API Observer {
	public:
		Observer();
		virtual ~Observer();
		virtual void ObjectDeleted(void*) {}
	};

	class CPPFX_API ObserverSet : public virtual cppfx::Referenced {
	public:
		ObserverSet(const Referenced* observedObject);
		Referenced* getObservedObject() { return _observedObject; }
		const Referenced* getObservedObject() const { return _observedObject; }

		Referenced* addRefLock();

		inline mutex* getObserverSetMutex() const { return &_mutex; }

		void addObserver(Observer* observer);
		void removeObserver(Observer* observer);

		void signalObjectDeleted(void* ptr);
	protected:
		ObserverSet(const ObserverSet& rhs) : cppfx::Referenced(rhs) {}
		ObserverSet& operator = (const ObserverSet&) { return *this; }
		virtual ~ObserverSet();

		mutable mutex _mutex;
		Referenced* _observedObject;
		void* _observers;
	};
}

#endif