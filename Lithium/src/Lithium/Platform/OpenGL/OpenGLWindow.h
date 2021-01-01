#pragma once

#include "Lithium/Core/Window.h"
#include "Lithium/Platform/OpenGL/OpenGLContext.h"

namespace Li
{
	class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const WindowProps& props);
		virtual ~OpenGLWindow();

		virtual void SwapBuffers() override;
		virtual int GetWidth() const override { return m_Width; }
		virtual int GetHeight() const override { return m_Height; }
		virtual GraphicsContext* GetContext() override { return m_Context.get(); }
		virtual SDL_Window* GetWindow() const override { return m_Window; }
		virtual uint32_t GetWindowID() const override { return m_ID; }

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override { return m_VSync; }
		virtual void SetFullscreen(FullscreenType type) override;
		virtual FullscreenType GetFullscreen() const override { return m_Fullscreen; };
		virtual void SetBordered(bool bordered) override;
		virtual void SetResizable(bool resizable) override;
		virtual void Show() override;
		virtual void Hide() override;
		virtual void SetSize(int width, int height) override;
		virtual void SetPosition(int x, int y) override;

		virtual void SetIcon(const char* path) override;

		virtual void OnWindowResize(int width, int height) override;

	private:
		SDL_Window* m_Window;
		Unique<OpenGLContext> m_Context;
		uint32_t m_ID;

		SDL_Surface* m_Icon;
		unsigned char* m_IconData;

		const char* m_Title;
		int m_Width;
		int m_Height;
		bool m_VSync;

		FullscreenType m_Fullscreen;
	};
}
