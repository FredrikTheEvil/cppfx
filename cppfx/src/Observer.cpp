#include <cppfx/Observer.h>
#include <set>

namespace cppfx {
	typedef std::set<Observer*> Observers;

	Observer::Observer()
	{
	}

	Observer::~Observer()
	{
	}

	ObserverSet::ObserverSet(const Referenced* observedObject) :
		_observedObject(const_cast<Referenced*>(observedObject))
	{
		_observers = new Observers;
	}

	ObserverSet::~ObserverSet()
	{
		delete static_cast<Observers*>(_observers);
	}

	void ObserverSet::addObserver(Observer* observer)
	{
		mutex_scope<> lock(_mutex);
		auto observers = static_cast<Observers*>(_observers);
		observers->insert(observer);
	}

	void ObserverSet::removeObserver(Observer* observer)
	{
		mutex_scope<> lock(_mutex);
		auto observers = static_cast<Observers*>(_observers);
		observers->erase(observer);
	}

	Referenced* ObserverSet::addRefLock()
	{
		mutex_scope<> lock(_mutex);

		if (!_observedObject) return 0;

		int refCount = _observedObject->ref();
		if (refCount == 1)
		{
			// The object is in the process of being deleted, but our
			// objectDeleted() method hasn't been run yet (and we're
			// blocking it -- and the final destruction -- with our lock).
			_observedObject->unref_no_delete();
			return 0;
		}

		return _observedObject;
	}

	void ObserverSet::signalObjectDeleted(void* ptr)
	{
		mutex_scope<> lock(_mutex);
		auto observers = static_cast<Observers*>(_observers);
		for (Observers::iterator itr = observers->begin();
			itr != observers->end();
			++itr)
		{
			(*itr)->ObjectDeleted(ptr);
		}
		observers->clear();

		// reset the observed object so that we know that it's now detached.
		_observedObject = 0;
	}
}