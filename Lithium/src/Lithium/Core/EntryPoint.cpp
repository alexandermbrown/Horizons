#include "lipch.h"
#include "EntryPoint.h"

#include "Lithium/Core/Log.h"

int main(int argc, char** argv)
{
	li::Log::InitLog();

	li::Application* app = li::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

