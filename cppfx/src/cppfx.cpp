#include "cppfx/cppfx.h"

#include <cstdio>

namespace cppfx {
	static char version_name_buffer[512];
	static bool version_generated = false;

	const char * get_version_info() {
		if (version_generated)
			return version_name_buffer;
#ifdef CPPFX_HAVE_SPRINTF_S
		sprintf_s(version_name_buffer, 512, "%d.%d.%d", CPPFX_VERSION_MAJOR, CPPFX_VERSION_MINOR, CPPFX_VERSION_PATCH);
#else
		std::sprintf(version_name_buffer, "%d.%d.%d-%s", CPPFX_VERSION_MAJOR, CPPFX_VERSION_MINOR, CPPFX_VERSION_PATCH);
#endif
		version_generated = true;
		return version_name_buffer;
	}
}