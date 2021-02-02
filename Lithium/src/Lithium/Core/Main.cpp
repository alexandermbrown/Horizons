#include "lipch.h"

#define SDL_MAIN_HANDLED
#include "Lithium/Core/Application.h"
#include "Lithium/Core/Memory.h"

extern Li::Unique<Li::Application> Li::CreateApplication();

int main(int argc, char** argv)
{
	Li::Log::InitLog();

	Li::Unique<Li::Application> app = Li::CreateApplication();
	app->Run();
	app.reset();

	return 0;
}
