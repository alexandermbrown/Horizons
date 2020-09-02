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
		static void Init();
		static void InitPostResourceLoad();

		static void Shutdown();

		static void AddTextureAtlas(Ref<TextureAtlas> atlas);

		static void BeginScene(OrthographicCamera* camera);
		static void EndScene();

		static void BeginUI();
		static void EndUI();

		static void SubmitTextured(const std::string& texture_alias, const glm::mat4& transform, bool crop = false);

		static void SubmitColored(const glm::vec4& color, const glm::mat4& transform);

		static void SubmitColoredTexture(const std::string& texture_alias, const glm::vec4& color, const glm::mat4& transform, bool crop = false);

		static void SubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);

		static void Submit(const Ref<Texture>& texture, const glm::mat4& transform);

		static void UISubmitTextured(const std::string& texture_alias, const glm::mat4& transform, bool crop = false);

		static void UISubmitColored(const glm::vec4& color, const glm::mat4& transform);

		static void UISubmitColoredTexture(const std::string& texture_alias, const glm::vec4& color, const glm::mat4& transform, bool crop = false);

		static void UISubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);

		static void UISubmit(const Ref<Texture>& texture, const glm::mat4& transform);

		static void Resize(int width, int height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		inline static Ref<UniformBuffer> GetViewProjUniformBuffer() { return s_Data->ViewProjUB; }
		inline static Ref<UniformBuffer> GetTransformUniformBuffer() { return s_Data->TransformMatrixUB; }
		inline static Ref<Shader> GetFontShader() { return s_Data->FontShader; }

	private:
		static void RenderQuad(const Ref<Texture>& texture, const glm::mat4& transform, const glm::mat4& view_projection);
		static void RenderLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);

		struct RendererData
		{
			Scope<BatchRenderer> SceneRenderer = nullptr;
			Scope<BatchRenderer> UIRenderer = nullptr;

			Ref<Shader> FontShader;

			Ref<Shader> TextureShader;
			Ref<VertexArray> QuadVA;

			Ref<UniformBuffer> ViewProjUB;
			Ref<UniformBuffer> TransformMatrixUB;
			Ref<UniformBuffer> FontUB;

			OrthographicCamera* Camera;
			Scope<OrthographicCamera> UICamera;

			bool ResourcesLoaded = false;
		};

		static Scope<RendererData> s_Data;
	};
}
