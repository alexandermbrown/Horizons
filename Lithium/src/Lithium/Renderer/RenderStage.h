#pragma once

#include <stdint.h>
#include "OrthographicCamera.h"

namespace Li
{
	class RenderStage
	{
	public:

		RenderStage(bool enabled = true)
			: m_Enabled(enabled) {}
		virtual ~RenderStage() = default;

		virtual void BeginScene(OrthographicCamera* camera) = 0;
		virtual void BindFramebuffer() const = 0;
		virtual void UnbindFramebuffer() const = 0;
		virtual void EndScene() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		inline virtual bool IsEnabled() const { return m_Enabled; };
		inline virtual void Enable() { m_Enabled = true; };
		inline virtual void Disable() { m_Enabled = false; };

	protected:
		bool m_Enabled;
	};
}