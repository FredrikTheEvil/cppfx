#pragma once
#ifndef __CPPFX_ASYNCTASK_H
#define __CPPFX_ASYNCTASK_H

#include <cppfx/Object.h>
#include <cppfx/ref_ptr.h>
#include <cppfx/string.h>
#include <cppfx/Exceptions.h>

namespace cppfx
{
	enum class TaskStatus
	{
		INACTIVE,
		ACTIVE,
		SUCCESSFUL,
		FAILURE
	};
	template<typename T, typename std::enable_if<std::is_base_of<Object, T>::value>::type* = nullptr>
	class AsyncTask : public Object
	{
	private:
		ref_ptr<T> result;
		TaskStatus status;
		long long progressProcessed;
		long long progressTotal;
		string errorMsg;
		mutable mutex mut;
	public:
		AsyncTask() :
			result(nullptr),
			status(TaskStatus::INACTIVE),
			progressProcessed(0),
			progressTotal(0)
		{
		}

		bool isComplete() const
		{
			bool b = false;
			{
				mutex_scope<> scope(mut);
				b = status == TaskStatus::FAILURE || status == TaskStatus::SUCCESSFUL;
			}
			return b;
		}
		bool succesful() const
		{
			bool b = false;
			{
				mutex_scope<> scope(mut);
				b = status == TaskStatus::SUCCESSFUL;
			}
			return b;
		}
		bool failure() const
		{
			bool b = false;
			{
				mutex_scope<> scope(mut);
				b = status == TaskStatus::FAILURE;
			}
			return b;
		}

		const string& errorMessage() const
		{
			string e;
			{
				mutex_scope<> scope(mut);
				if (status == TaskStatus::FAILURE)
					e = errorMsg;
			}
			return e;
		}

		void setProgress(long long processed, long long total)
		{
			mutex_scope<> scope(mut);
			if (status != TaskStatus::INACTIVE && status != TaskStatus::ACTIVE)
				throw RuntimeError("Task already resolved");

			status = TaskStatus::ACTIVE;
			progressProcessed = processed;
			progressTotal = total;
		}

		void setError(const string& message)
		{
			mutex_scope<> scope(mut);
			if (status != TaskStatus::INACTIVE && status != TaskStatus::ACTIVE)
				throw RuntimeError("Task already resolved");
			status = TaskStatus::FAILURE;
			errorMsg = message;
		}

		void setValue(const ref_ptr<T>& value)
		{
			mutex_scope<> scope(mut);
			if (status != TaskStatus::INACTIVE && status != TaskStatus::ACTIVE)
				throw RuntimeError("Task already resolved");
			status = TaskStatus::SUCCESSFUL;
			result = value;
		}
		ref_ptr<T>& getValue()
		{
			ref_ptr<T> obj = nullptr;
			mutex_scope<> scope(mut);
			obj = result;
			return obj;
		}
	};
}

#endif