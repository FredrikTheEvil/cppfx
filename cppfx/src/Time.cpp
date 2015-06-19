#include <cppfx/Time.h>

#if defined(CPPFX_USE_GLFW)
#include <GLFW/glfw3.h>
#elif defined(CPPFX_USE_SDL)
#include <SDL.h>
#endif

namespace cppfx {
	double Time::getElapsedTime() {
#if defined(CPPFX_USE_SDL)
		return ((double)SDL_GetTicks()) / 1000.0;
#elif defined(CPPFX_USE_GLFW)
		return glfwGetTime();
#endif
	}
}