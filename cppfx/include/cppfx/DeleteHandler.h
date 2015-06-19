#pragma once
#ifndef __CPPFX_DELETEHANDLER_H
#define __CPPFX_DELETEHANDLER_H

#include <cppfx/Referenced.h>

namespace cppfx {
	class CPPFX_API DeleteHandler {
	public:
		DeleteHandler(int numberOfFramesToRetainObjects = 0);
		virtual ~DeleteHandler();
		void setNumFramesToRetainObjects(unsigned int numberOfFramesToRetainObjects) { _numFramesToRetainObjects = numberOfFramesToRetainObjects; }
		unsigned int getNumFramesToRetainObjects() const { return _numFramesToRetainObjects; }
		void setFrameNumber(unsigned int frameNumber) { _currentFrameNumber = frameNumber; }
		unsigned int getFrameNumber() const { return _currentFrameNumber; }

		inline void doDelete(const Referenced* object) { delete object; }

		virtual void flush();
		virtual void flushAll();

		virtual void requestDelete(const cppfx::Referenced* object);
	protected:

		DeleteHandler(const DeleteHandler&) :
			_numFramesToRetainObjects(0),
			_currentFrameNumber(0) {}
		DeleteHandler operator = (const DeleteHandler&) { return *this; }

		unsigned int            _numFramesToRetainObjects;
		unsigned int            _currentFrameNumber;
		mutex _mutex;
		void*     _objectsToDelete;
	};
}

#endif