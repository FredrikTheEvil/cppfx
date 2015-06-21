#include <cppfx/Time.h>

#if defined(CPPFX_USE_SDL) || defined(__EMSCRIPTEN__)
#include <SDL.h>
#elif defined(CPPFX_USE_GLFW)
#include <GLFW/glfw3.h>
#endif

namespace cppfx {
	double Time::getElapsedTime() {
#if defined(CPPFX_USE_SDL) || defined(__EMSCRIPTEN__)
		return ((double)SDL_GetTicks()) / 1000.0;
#elif defined(CPPFX_USE_GLFW)
		return glfwGetTime();
#endif
	}
}