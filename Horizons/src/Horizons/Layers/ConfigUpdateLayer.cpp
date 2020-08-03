#include "pch.h"
#include "ConfigUpdateLayer.h"

#include "Horizons.h"

ConfigUpdateLayer::ConfigUpdateLayer()
{
}

void ConfigUpdateLayer::OnAttach()
{
	Clean();
}

void ConfigUpdateLayer::OnDetach()
{
}

void ConfigUpdateLayer::OnUpdate(float dt)
{
	Clean();
}

void ConfigUpdateLayer::OnImGuiRender()
{
}

void ConfigUpdateLayer::OnEvent(SDL_Event* event)
{
	if (!m_OnSplashScreen)
	{
		if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			ConfigStore& config = li::Application::Get<Horizons>()->GetConfig();

			int width = config.Get("window_width").GetInt();
			int height = config.Get("window_height").GetInt();

			if (width != event->window.data1 || height != event->window.data2)
			{
				config.Get("window_width").SetInt(event->window.data1);
				config.Get("window_height").SetInt(event->window.data2);
			}
		}
	}
}

void ConfigUpdateLayer::Clean()
{
	ConfigStore& config = li::Application::Get<Horizons>()->GetConfig();

	ConfigVar& vsync = config.Get("use_vsync");
	if (vsync.IsDirty())
	{
		LI_TRACE("Changed vsync!");
		li::Application::Get()->GetWindow()->SetVSync(vsync.GetBool());
		vsync.Clean();
	}
}
