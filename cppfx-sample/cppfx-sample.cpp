#include <cppfx/platform/Window.h>
#include <cppfx/graphics/BmpTextureLoader.h>
#include <cppfx/gui/elements/Document.h>
#include <cppfx/Time.h>
#include <iostream>

using namespace cppfx;
using namespace cppfx::graphics;

int main(int argc, char* argv[]) {
	try {
		ref_ptr<platform::Window> window = new platform::Window(1280, 720, false, "Heisann");
		ref_ptr<BmpTextureLoader> loader = new BmpTextureLoader(window);
		window->setTextureLoader(loader.get());
		gui::elements::Element::registerCoreElements();
		auto doc = gui::elements::Document::loadDocument(window, "sample.xml");
		double deltaTime = 0.16;
		double time = Time::getElapsedTime();
		while (window->processEvents()) {
			window->setClearColor(colors::blue);
			window->clear(ClearFlags::COLOR_BUFFER | ClearFlags::DEPTH_BUFFER);

			doc->setScreenSize(vector2f(window->getWidth(), window->getHeight()));
			doc->update(deltaTime);
			doc->render();

			window->swapBuffers();

			auto x = Time::getElapsedTime();
			deltaTime = x - time;
			time = x;
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "unknown exception occured" << std::endl;
	}
}