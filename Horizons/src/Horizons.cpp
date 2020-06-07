#include "pch.h"
#include "Horizons.h"

#include "Horizons/Layers/GameLayer.h"

Horizons::Horizons()
	: li::Application()
{
	li::ResourceManager::Init("data/resources.lab");
	li::Localization::SetLocale("en-us");
	PushLayer(new GameLayer());
}

li::Application* li::CreateApplication()
{
	return new Horizons();
}
