#include <cppfx/platform/Window.h>
#include <cppfx/graphics/BmpTextureLoader.h>
#include <cppfx/graphics/MultiTextureLoader.h>
#include <cppfx/gui/elements/Document.h>
#include <cppfx/Time.h>
#include <iostream>
#include <regex>
#include <cppfx/Exceptions.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace cppfx;
using namespace cppfx::graphics;

static ref_ptr<platform::Window> s_window = nullptr;
static ref_ptr<gui::elements::Document> s_document = nullptr;
static bool s_done = false;
static double s_currentTime = 0.0;

void main_loop()
{
	try {
		auto x = Time::getElapsedTime();
		auto deltaTime = x - s_currentTime;
		s_currentTime = x;

		if (!s_window->processEvents())
		{
			s_document = nullptr;
			s_window = nullptr;
			s_done = true;

			return;
		}

		if (s_window.valid())
		{
			s_window->setClearColor(colors::blue);
			s_window->clear(ClearFlags::COLOR_BUFFER | ClearFlags::DEPTH_BUFFER);

			if (s_document.valid()) {
				s_document->setScreenSize(vector2f(s_window->getWidth(), s_window->getHeight()));
				s_document->update(deltaTime);
				s_document->render();
			}
			s_window->swapBuffers();
		}
	}
	catch (Exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return;
	}
	catch (...) {
		std::cerr << "unknown exception occured" << std::endl;
		return;
	}
}

int main(int argc, char* argv[]) {
	try {
		s_window = new platform::Window(1280, 720, false, "Heisann");
		s_document = gui::elements::Document::loadDocument(s_window, "sample.xml");
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(main_loop, 30, 0);
#else
		while (!s_done)
		{
			main_loop();
		}
#endif
	}
	catch (Exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "unknown exception occured" << std::endl;
		return 1;
	}
	return 0;
}