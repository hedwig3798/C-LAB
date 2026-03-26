#include "CollisionTest.h"
#include "SDLRenderer.h"

int main()
{
	SDLRenderer* sdl = new SDLRenderer();

	sdl->InitWindow();
	sdl->SDLLoop();

	return 0;
}
