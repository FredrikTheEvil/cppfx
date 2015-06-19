#include <cppfx/DeleteHandler.h>
#include <list>

namespace cppfx {
	typedef std::pair<unsigned int, const Referenced*> FrameNumberObjectPair;
	typedef std::list<FrameNumberObjectPair> ObjectsToDeleteList;

	DeleteHandler::DeleteHandler(int numberOfFramesToRetainObjects) :
		_numFramesToRetainObjects(numberOfFramesToRetainObjects),
		_currentFrameNumber(0)
	{
		_objectsToDelete = new ObjectsToDeleteList();
	}

	DeleteHandler::~DeleteHandler()
	{
		// flushAll();
		delete _objectsToDelete;
	}

	void DeleteHandler::flush()
	{
		typedef std::list<const cppfx::Referenced*> DeletionList;
		DeletionList deletionList;

		{
			// gather all the objects to delete whilst holding the mutex to the _objectsToDelete
			// list, but delete the objects outside this scoped lock so that if any objects deleted
			// unref their children then no deadlock happens.
			mutex_scope<> lock(_mutex);
			unsigned int frameNumberToClearTo = _currentFrameNumber - _numFramesToRetainObjects;

			auto objectsToDelete = static_cast<ObjectsToDeleteList*>(_objectsToDelete);

			ObjectsToDeleteList::iterator itr;
			for (itr = objectsToDelete->begin();
				itr != objectsToDelete->end();
				++itr)
			{
				if (itr->first > frameNumberToClearTo) break;

				deletionList.push_back(itr->second);

				itr->second = 0;
			}

			objectsToDelete->erase(objectsToDelete->begin(), itr);
		}

		for (DeletionList::iterator ditr = deletionList.begin();
			ditr != deletionList.end();
			++ditr)
		{
			doDelete(*ditr);
		}

	}

	void DeleteHandler::flushAll()
	{
		unsigned int temp_numFramesToRetainObjects = _numFramesToRetainObjects;
		_numFramesToRetainObjects = 0;

		typedef std::list<const cppfx::Referenced*> DeletionList;
		DeletionList deletionList;

		{
			// gather all the objects to delete whilst holding the mutex to the _objectsToDelete
			// list, but delete the objects outside this scoped lock so that if any objects deleted
			// unref their children then no deadlock happens.
			mutex_scope<> lock(_mutex);
			auto objectsToDelete = static_cast<ObjectsToDeleteList*>(_objectsToDelete);
			ObjectsToDeleteList::iterator itr;
			for (itr = objectsToDelete->begin();
				itr != objectsToDelete->end();
				++itr)
			{
				deletionList.push_back(itr->second);
				itr->second = 0;
			}

			objectsToDelete->erase(objectsToDelete->begin(), objectsToDelete->end());
		}

		for (DeletionList::iterator ditr = deletionList.begin();
			ditr != deletionList.end();
			++ditr)
		{
			doDelete(*ditr);
		}

		_numFramesToRetainObjects = temp_numFramesToRetainObjects;
	}

	void DeleteHandler::requestDelete(const cppfx::Referenced* object)
	{
		if (_numFramesToRetainObjects == 0) doDelete(object);
		else
		{
			mutex_scope<> lock(_mutex);
			auto objectsToDelete = static_cast<ObjectsToDeleteList*>(_objectsToDelete);
			objectsToDelete->push_back(FrameNumberObjectPair(_currentFrameNumber, object));
		}
	}
}