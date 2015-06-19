#pragma once
#ifndef __CPPFX_WEAK_PTR_H
#define __CPPFX_WEAK_PTR_H

#include <cppfx/config.h>
#include <cppfx/Observer.h>

namespace cppfx {
	template<class T>
	class weak_ptr {
	public:
		typedef T element_type;
		weak_ptr() : _reference(nullptr), _ptr(nullptr) {}
		weak_ptr(const ref_ptr<T>& rp) {
			_reference = rp.valid() ? rp->getOrCreateObserverSet() : nullptr;
			_ptr = (_reference.valid() && _reference->getObservedObject() != nullptr) ? rp.get() : nullptr;
		}
		weak_ptr(T* ptr) {
			_reference = ptr ? ptr->getOrCreateObserverSet() : nullptr;
			_ptr = (_reference.valid() && _reference->getObservedObject() != nullptr) ? ptr : nullptr;
		}
		weak_ptr(const weak_ptr& wp) :
			_reference(wp._reference),
			_ptr(wp._ptr) {
		}
		~weak_ptr() {
		}
		weak_ptr& operator = (const weak_ptr& wp) {
			if (&wp == this)
				return *this;
			_reference = wp._reference;
			_ptr = wp._ptr;
			return *this;
		}
		weak_ptr& operator = (const ref_ptr<T>& rp) {
			_reference = rp.valid() ? rp->getOrCreateObserverSet() : nullptr;
			_ptr = (_reference.valid() && _reference->getObservedObject() != nullptr) ? rp.get() : nullptr;
			return *this;
		}
		weak_ptr& operator = (T* rp) {
			_reference = rp != nullptr ? rp->getOrCreateObserverSet() : nullptr;
			_ptr = (_reference.valid() && _reference->getObservedObject() != nullptr) ? rp : nullptr;
			return *this;
		}
		bool lock(ref_ptr<T>& rptr) const {
			if (!_reference) {
				rptr = nullptr;
				return false;
			}
			Referenced* obj = _reference->addRefLock();
			if (obj == nullptr) {
				rptr = nullptr;
				return false;
			}
			rptr = _ptr;
			obj->unref_no_delete();
			return rptr.valid();
		}
		bool operator == (const weak_ptr& wp) const { return _reference == wp._reference; }
		bool operator != (const weak_ptr& wp) const { return _reference != wp._reference; }
		bool operator < (const weak_ptr& wp) const { return _reference < wp._reference; }
		bool operator >(const weak_ptr& wp) const { return _reference > wp._reference; }

		inline bool operator == (const T* ptr) const { return _ptr == ptr; }
		inline bool operator != (const T* ptr) const { return _ptr != ptr; }
		inline bool operator < (const T* ptr) const { return _ptr < ptr; }
		inline bool operator >(const T* ptr) const { return _ptr > ptr; }

		inline T& operator*() const { return *_ptr; }
		inline T* operator->() const { return _ptr; }

		inline T* get() const { return (_reference.valid() && _reference->getObservedObject() != 0) ? _ptr : 0; }

		inline bool operator!() const { return get() == 0; }
		inline bool valid() const { return get() != 0; }

		ref_ptr<ObserverSet> _reference;
		T* _ptr;
	};
} // cppfx

#endif