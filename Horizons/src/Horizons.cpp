#include "pch.h"
#include "Horizons.h"

#include "Horizons/Layers/GameLayer.h"

Horizons::Horizons()
	: li::Application()
{
	PushLayer(new GameLayer());
}

li::Application* li::CreateApplication()
{
	return new Horizons();
}
