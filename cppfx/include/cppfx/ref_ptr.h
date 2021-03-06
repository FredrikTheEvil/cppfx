#pragma once
#ifndef __CPPFX_REF_PTR_H
#define __CPPFX_REF_PTR_H

#include <cppfx/config.h>

namespace cppfx {
	template<typename T> class weak_ptr;

	template<class T>
	class ref_ptr {
	public:
		typedef T element_type;

		ref_ptr() : _ptr(0) {}
		ref_ptr(ref_ptr&& rp) : _ptr(rp._ptr) { rp._ptr = nullptr; }
		ref_ptr(T* ptr) : _ptr(ptr) { if (_ptr) _ptr->ref(); }
		ref_ptr(const ref_ptr& rp) : _ptr(rp._ptr) { if (_ptr) _ptr->ref(); }
		template<class Other> ref_ptr(const ref_ptr<Other>& rp) : _ptr(rp._ptr) { if (_ptr) _ptr->ref(); }
		template<class Other> ref_ptr(ref_ptr<Other>&& rp) : _ptr(rp._ptr) { rp._ptr = nullptr; }
		ref_ptr(weak_ptr<T>& optr) : _ptr(0) { optr.lock(*this); }
		~ref_ptr() { if (_ptr != nullptr) _ptr->unref();  _ptr = 0; }

		ref_ptr& operator = (const ref_ptr& rp)
		{
			assign(rp);
			return *this;
		}
		ref_ptr& operator = (ref_ptr&& rp) {
			if (_ptr == rp._ptr)
				return *this;
			if (_ptr != nullptr)
				_ptr->unref();
			_ptr = rp._ptr;
			rp._ptr = nullptr;
			if (_ptr != nullptr)
				_ptr->ref();
			return *this;
		}

		template<class Other> ref_ptr& operator = (const ref_ptr<Other>& rp)
		{
			assign(rp);
			return *this;
		}

		inline ref_ptr& operator = (T* ptr)
		{
			if (_ptr == ptr) return *this;
			T* tmp_ptr = _ptr;
			_ptr = ptr;
			if (_ptr) _ptr->ref();
			if (tmp_ptr) tmp_ptr->unref();
			return *this;
		}

		// comparison operators for ref_ptr.
		bool operator == (const ref_ptr& rp) const { return (_ptr == rp._ptr); }
		bool operator == (const T* ptr) const { return (_ptr == ptr); }
		friend bool operator == (const T* ptr, const ref_ptr& rp) { return (ptr == rp._ptr); }

		bool operator != (const ref_ptr& rp) const { return (_ptr != rp._ptr); }
		bool operator != (const T* ptr) const { return (_ptr != ptr); }
		friend bool operator != (const T* ptr, const ref_ptr& rp) { return (ptr != rp._ptr); }

		bool operator < (const ref_ptr& rp) const { return (_ptr < rp._ptr); }
	private:
		typedef T* ref_ptr::*unspecified_bool_type;
	public:
		operator unspecified_bool_type() const { return valid() ? &ref_ptr::_ptr : 0; }

		T& operator*() const { return *_ptr; }
		T* operator->() const { return _ptr; }
		T* get() const { return _ptr; }

		bool operator!() const { return _ptr == 0; }
		bool valid() const { return _ptr != 0; }

		T* release() { T* tmp = _ptr; if (_ptr) _ptr->unref_nodelete(); _ptr = 0; return tmp; }

		void swap(ref_ptr& rp) { T* tmp = _ptr; _ptr = rp._ptr; rp._ptr = tmp; }
	private:

		template<class Other> void assign(const ref_ptr<Other>& rp)
		{
			if (_ptr == rp._ptr) return;
			T* tmp_ptr = _ptr;
			_ptr = rp._ptr;
			if (_ptr) _ptr->ref();
			if (tmp_ptr) tmp_ptr->unref();
		}

		template<class Other> friend class ref_ptr;

		T* _ptr;
	};

	template<class T> inline
		void swap(ref_ptr<T>& rp1, ref_ptr<T>& rp2) { rp1.swap(rp2); }

	template<class T> inline
		T* get_pointer(const ref_ptr<T>& rp) { return rp.get(); }

	template<class T, class Y> inline
		ref_ptr<T> static_pointer_cast(const ref_ptr<Y>& rp) { return static_cast<T*>(rp.get()); }

	template<class T, class Y> inline
		ref_ptr<T> dynamic_pointer_cast(const ref_ptr<Y>& rp) { return dynamic_cast<T*>(rp.get()); }

	template<class T, class Y> inline
		ref_ptr<T> const_pointer_cast(const ref_ptr<Y>& rp) { return const_cast<T*>(rp.get()); }
} // namespace cppfx

#endif