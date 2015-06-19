#pragma once
#ifndef __CPPFX_TIME_H
#define __CPPFX_TIME_H

#include <cppfx/config.h>
#include <cppfx/export.h>

namespace cppfx {
	class CPPFX_API Time {
	public:
		static double getElapsedTime();
	};
}

#endif