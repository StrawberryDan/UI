#include <Strawberry/Graphics/Window.hpp>


using namespace Strawberry;


int main()
{
	Graphics::Window::Window window("StrawberryUI Test", Core::Math::Vec2i(1920, 1080));

	while(!window.CloseRequested())
	{
		Graphics::Window::PollInput();
		while (auto event = window.NextEvent())
		{

		}

		window.SwapBuffers();
	}
	return 0;
}