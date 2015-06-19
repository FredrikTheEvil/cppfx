#include <cppfx/Referenced.h>
#include <cppfx/Observer.h>
#include <cppfx/DeleteHandler.h>

namespace cppfx {
	template<typename T>
	struct ResetPointer {
		ResetPointer() :
			_ptr(nullptr) {}

		ResetPointer(T* ptr) :
			_ptr(ptr) {}

		~ResetPointer()
		{
			if (_ptr != nullptr)
				delete _ptr;
			_ptr = nullptr;
		}

		inline ResetPointer& operator = (T* ptr)
		{
			if (_ptr == ptr) return *this;
			if (_ptr != nullptr)
				delete _ptr;
			_ptr = ptr;
			return *this;
		}

		void Reset(T* ptr)
		{
			if (_ptr == ptr) return;
			if (_ptr != nullptr)
				delete _ptr;
			_ptr = ptr;
		}

		inline T& operator*() { return *_ptr; }
		inline const T& operator*() const { return *_ptr; }
		inline T* operator->() { return _ptr; }
		inline const T* operator->() const { return _ptr; }
		T* get() { return _ptr; }
		const T* get() const { return _ptr; }

		T* _ptr;
	};

	typedef ResetPointer<DeleteHandler> DeleteHandlerPointer;
	typedef ResetPointer<mutex> GlobalmutexPointer;

	mutex* Referenced::getGlobalReferencedMutex()
	{
		static GlobalmutexPointer s_ReferencedGlobalmutext = new mutex();
		return s_ReferencedGlobalmutext.get();
	}
	struct InitGlobalmutexes
	{
		InitGlobalmutexes()
		{
			Referenced::getGlobalReferencedMutex();
		}
	};
	static InitGlobalmutexes s_initGlobalmutexes;
	static bool s_useThreadSafeReferenceCounting = false;
	static DeleteHandlerPointer s_deleteHandler(nullptr);

	void Referenced::setThreadSafeReferenceCounting(bool enableThreadSafeReferenceCounting)
	{
		s_useThreadSafeReferenceCounting = enableThreadSafeReferenceCounting;
	}
	bool Referenced::getThreadSafeReferenceCounting()
	{
		return s_useThreadSafeReferenceCounting;
	}
	void Referenced::setDeleteHandler(DeleteHandler* handler)
	{
		s_deleteHandler.Reset(handler);
	}

	DeleteHandler* Referenced::getDeleteHandler()
	{
		return s_deleteHandler.get();
	}

	Referenced::Referenced() :
		_refMutex(nullptr),
		_refCount(0),
		_observerSet(nullptr)
	{
#ifndef ENFORCE_THREADSAFE
		if (s_useThreadSafeReferenceCounting)
#endif
			_refMutex = new mutex;
	}

	Referenced::Referenced(const Referenced&) :
		_refMutex(nullptr),
		_refCount(0),
		_observerSet(nullptr)
	{
#ifndef ENFORCE_THREADSAFE
		if (s_useThreadSafeReferenceCounting)
#endif
			_refMutex = new mutex();
	}

	Referenced::~Referenced()
	{
		// signal observers that we are being deleted.
		signalObserversAndDelete(true, false);

		// delete the ObserverSet
		if (_observerSet) static_cast<ObserverSet*>(_observerSet)->unref();
		if (_refMutex) delete _refMutex;

		_observerSet = nullptr;
		_refMutex = nullptr;
	}

	ObserverSet* Referenced::getOrCreateObserverSet() const
	{
		if (_refMutex)
		{
			mutex_scope<> lock(*_refMutex);
			if (!_observerSet)
			{
				_observerSet = new ObserverSet(this);
				static_cast<ObserverSet*>(_observerSet)->ref();
			}
			return static_cast<ObserverSet*>(_observerSet);
		}
		else
		{
			if (!_observerSet)
			{
				_observerSet = new ObserverSet(this);
				static_cast<ObserverSet*>(_observerSet)->ref();
			}
			return static_cast<ObserverSet*>(_observerSet);
		}
	}

	void Referenced::addObserver(Observer* observer) const
	{
		getOrCreateObserverSet()->addObserver(observer);
	}

	void Referenced::removeObserver(Observer* observer) const
	{
		getOrCreateObserverSet()->removeObserver(observer);
	}

	void Referenced::signalObserversAndDelete(bool signalDelete, bool doDelete) const
	{
		ObserverSet* observerSet = static_cast<ObserverSet*>(_observerSet);

		if (observerSet && signalDelete)
		{
			observerSet->signalObjectDeleted(const_cast<Referenced*>(this));
		}

		if (doDelete)
		{
			if (getDeleteHandler()) deleteUsingDeleteHandler();
			else delete this;
		}
	}


	void Referenced::setThreadSafeRefUnref(bool threadSafe)
	{
		if (threadSafe)
		{
			if (!_refMutex)
			{
				_refMutex = new mutex();
			}
		}
		else
		{
			if (_refMutex)
			{
				// we don't want thread safe ref()/unref() so remove any assigned mutex
				mutex* tmpmutexPtr = _refMutex;
				_refMutex = nullptr;
				delete tmpmutexPtr;
			}
		}
	}

	int Referenced::unref_no_delete() const
	{
		if (_refMutex)
		{
			mutex_scope<> lock(*_refMutex);
			return --_refCount;
		}
		else
		{
			return --_refCount;
		}
	}

	void Referenced::deleteUsingDeleteHandler() const
	{
		getDeleteHandler()->requestDelete(this);
	}
}