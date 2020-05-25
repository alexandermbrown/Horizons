#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/GraphicsContext.h"
#include "Lithium/Renderer/Renderer.h"

#include <SDL.h>

namespace li
{
	enum class FullscreenType
	{
		Windowed,
		Fullscreen,
		FullscreenWindowed
	};

	class Window
	{
	public:

		virtual ~Window() = default;

		virtual void Shutdown() = 0;

		virtual void SwapBuffers() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual Ref<GraphicsContext> GetContext() const = 0;
		virtual SDL_Window* GetWindow() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetFullscreen(FullscreenType type) = 0;
		virtual FullscreenType GetFullscreen() const = 0;

		virtual void OnWindowEvent(SDL_Event* event) = 0;

		static Ref<Window> Create(
			const char* title = "Lithium Engine",
			int width = 1280, int height = 720,
			bool resizable = false, bool shown = true,
			RendererAPI::API api = RendererAPI::API::OpenGL
		);
	};
}