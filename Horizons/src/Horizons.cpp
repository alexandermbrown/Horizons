#include "Horizons.h"

#include "GameLayer.h"

Horizons::Horizons()
{
	PushLayer(new GameLayer());
}

li::Application* li::CreateApplication()
{
	return new Horizons();
}
