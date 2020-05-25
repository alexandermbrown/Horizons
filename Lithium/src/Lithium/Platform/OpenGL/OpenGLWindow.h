#pragma once

#include "Lithium/Core/Window.h"
#include "Lithium/Platform/OpenGL/OpenGLContext.h"

namespace li
{
	class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const char* title, int width, int height, bool resizable, bool shown);
		virtual ~OpenGLWindow();

		virtual void Shutdown() override;

		virtual void SwapBuffers() override;
		virtual inline unsigned int GetWidth() const override { return m_Width; }
		virtual inline unsigned int GetHeight() const override { return m_Height; }
		virtual inline Ref<GraphicsContext> GetContext() const override { return m_Context; }
		virtual inline SDL_Window* GetWindow() const override { return m_Window; }

		virtual void SetVSync(bool enabled) override;
		virtual inline bool IsVSync() const override { return m_VSync; }
		virtual void SetFullscreen(FullscreenType type) override;
		virtual inline FullscreenType GetFullscreen() const override { return m_Fullscreen; };

		virtual void OnWindowEvent(SDL_Event* event) override;

	private:
		SDL_Window* m_Window;
		Ref<OpenGLContext> m_Context;

		const char* m_Title;
		int m_Width;
		int m_Height;
		bool m_VSync;

		FullscreenType m_Fullscreen;
	};
}