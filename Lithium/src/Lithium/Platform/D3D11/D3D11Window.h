#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/GraphicsContext.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Core/Window.h"

#include "D3D11Context.h"

#include <SDL.h>

namespace li
{
	class D3D11Window : public Window
	{
	public:

		D3D11Window(const WindowProps& props);
		virtual ~D3D11Window();

		virtual void SwapBuffers() override;
		virtual inline int GetWidth() const override { return m_Width; }
		virtual inline int GetHeight() const override { return m_Height; }
		virtual inline GraphicsContext* GetContext() const override { return m_Context; }
		virtual inline SDL_Window* GetWindow() const override { return m_Window; }

		virtual void SetVSync(bool enabled) override;
		virtual inline bool IsVSync() const override { return false; } // { return m_VSync; } TODO
		virtual void SetFullscreen(FullscreenType type) override;
		virtual inline FullscreenType GetFullscreen() const override { return m_Fullscreen; };
		virtual void SetBordered(bool bordered) override;
		virtual void SetResizable(bool resizable) override;
		virtual void Show() override;
		virtual void Hide() override;
		virtual void SetSize(int width, int height) override;
		virtual void SetPosition(int x, int y) override;

		virtual void SetIcon(const std::string& path) override;

		virtual void OnWindowResize(int width, int height) override;

	private:
		SDL_Window* m_Window;
		D3D11Context* m_Context;

		SDL_Surface* m_Icon;
		unsigned char* m_IconData;

		const char* m_Title;
		int m_Width;
		int m_Height;

		FullscreenType m_Fullscreen;
	};
}