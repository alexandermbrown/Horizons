#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/OrthographicCamera.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/VertexArray.h"
#include "Lithium/Renderer/Shader.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Renderer/BatchRenderer.h"
#include "Lithium/Renderer/RenderStage.h"
#include "Lithium/Renderer/Font.h"

#include <glm/glm.hpp>

namespace li
{
	class Renderer
	{
	public:

		Renderer();
		virtual ~Renderer();

		inline static void Init() { s_Instance->InitImpl(); }
		inline static void Shutdown() { s_Instance.reset(); }

		inline static void AddTextureAtlas(Ref<TextureAtlas> atlas) { s_Instance->AddTextureAtlasImpl(atlas); }

		inline static void BeginScene(OrthographicCamera* camera) { s_Instance->BeginSceneImpl(camera); }

		inline static void EndScene() { s_Instance->EndSceneImpl(); }

		inline static void SubmitTextured(const std::string& textureAlias, const glm::mat4& transform = glm::mat4(1.0f))
		{
			s_Instance->SubmitTexturedImpl(textureAlias, transform);
		}

		inline static void SubmitColored(const glm::vec4& color, const glm::mat4& transform = glm::mat4(1.0f))
		{
			s_Instance->SubmitColoredImpl(color, transform);
		}

		inline static void SubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform = glm::mat4(1.0f))
		{
			s_Instance->SubmitColoredTextureImpl(textureAlias, color, transform);
		}

		inline static void SubmitLabel(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color)
		{
			s_Instance->SubmitLabelImpl(label, transform, color);
		}

		inline static void UISubmitTextured(const std::string& textureAlias, const glm::mat4& transform = glm::mat4(1.0f))
		{
			s_Instance->UISubmitTexturedImpl(textureAlias, transform);
		}

		inline static void UISubmitColored(const glm::vec4& color, const glm::mat4& transform = glm::mat4(1.0f))
		{
			s_Instance->UISubmitColoredImpl(color, transform);
		}

		inline static void UISubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform = glm::mat4(1.0f))
		{
			s_Instance->UISubmitColoredTextureImpl(textureAlias, color, transform);
		}

		inline static void UISubmitLabel(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color)
		{
			s_Instance->UISubmitLabelImpl(label, transform, color);
		}

		inline static void Resize(uint32_t width, uint32_t height)
		{
			s_Instance->ResizeImpl(width, height);
		}

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	protected:
		void InitImpl();

		inline void AddTextureAtlasImpl(Ref<TextureAtlas> atlas) 
		{
			m_SceneRenderer.AddTextureAtlas(atlas);
		}

		void BeginSceneImpl(OrthographicCamera* camera);
		void EndSceneImpl();

		void SubmitTexturedImpl(const std::string& textureAlias, const glm::mat4& transform);
		void SubmitColoredImpl(const glm::vec4& color, const glm::mat4& transform);
		void SubmitColoredTextureImpl(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform);
		
		inline void SubmitLabelImpl(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color)
		{
			RenderLabel(label, transform, color, m_Camera->GetViewProjectionMatrix());
		}

		void UISubmitTexturedImpl(const std::string& textureAlias, const glm::mat4& transform);
		void UISubmitColoredImpl(const glm::vec4& color, const glm::mat4& transform);
		void UISubmitColoredTextureImpl(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform);
		
		inline void UISubmitLabelImpl(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color)
		{
			RenderLabel(label, transform, color, m_UICamera->GetViewProjectionMatrix());
		}

		void ResizeImpl(uint32_t width, uint32_t height);

		BatchRenderer m_SceneRenderer;
		BatchRenderer m_UIRenderer;

		Ref<Shader> m_FontShader;

		OrthographicCamera* m_Camera;
		Scope<OrthographicCamera> m_UICamera;

	private:

		void RenderLabel(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color, const glm::mat4& viewProjection);

		static Scope<Renderer> s_Instance;
	};
}
