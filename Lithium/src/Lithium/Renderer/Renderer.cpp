#include "lipch.h"
#include "Renderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

namespace li
{
	Scope<Renderer::RendererData> Renderer::s_Data;

	void Renderer::Init()
	{
		s_Data = CreateScope<Renderer::RendererData>();
		s_Data->m_SceneRenderer = BatchRenderer({ 0.5f, 0.5f });
		s_Data->m_UIRenderer = BatchRenderer({ 0.0f, 0.0f });
		s_Data->m_Camera = nullptr;

		Ref<Window>& window = Application::Get()->GetWindow();
		s_Data->m_UICamera = CreateScope<OrthographicCamera>(0, (float)window->GetWidth(), 0, (float)window->GetHeight());

		RendererAPI::Init();
		RendererAPI::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		// TODO: Change path to user defined.
		s_Data->m_SceneRenderer.Init("shader_instance");
		s_Data->m_UIRenderer.Init("shader_instance");

		// SETUP WHITE TEXTURE
		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		whiteTexture->SetData(&data, sizeof(data));

		// SETUP SCENE FLAT COLOR TEXTURE ALTAS
		Ref<TextureAtlas> sceneFlatColorAtlas = CreateRef<TextureAtlas>(TextureAtlas(whiteTexture, {
			{ "texture_white", glm::vec4(0.5f) }
		}));
		s_Data->m_SceneRenderer.AddTextureAtlas(sceneFlatColorAtlas);

		// SETUP UI FLAT COLOR TEXTURE ALTAS
		Ref<TextureAtlas> uiFlatColorAtlas = CreateRef<TextureAtlas>(TextureAtlas(whiteTexture, {
			{ "texture_white", glm::vec4(0.5f) }
			}));
		s_Data->m_UIRenderer.AddTextureAtlas(uiFlatColorAtlas);
	}

	void Renderer::Shutdown()
	{
		s_Data.reset();
	}

	void Renderer::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		s_Data->m_SceneRenderer.AddTextureAtlas(atlas);
	}

	void Renderer::BeginScene(OrthographicCamera* camera)
	{
		s_Data->m_FontShader = ResourceManager::Get<Shader>("shader_label");
		RendererAPI::Clear();
		s_Data->m_Camera = camera;
		s_Data->m_SceneRenderer.BeginScene(camera);
		s_Data->m_UIRenderer.BeginScene(s_Data->m_UICamera.get());
	}

	void Renderer::EndScene()
	{
		RendererAPI::SetDepthTest(true);
		s_Data->m_SceneRenderer.EndScene();
		s_Data->m_UIRenderer.EndScene();
	}

	void Renderer::SubmitTextured(const std::string& textureAlias, const glm::mat4& transform)
	{
		s_Data->m_SceneRenderer.Submit(textureAlias, glm::vec4(1.0f), transform);
	}

	void Renderer::SubmitColored(const glm::vec4& color, const glm::mat4& transform)
	{
		s_Data->m_SceneRenderer.Submit("texture_white", color, transform);
	}

	void Renderer::SubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform)
	{
		s_Data->m_SceneRenderer.Submit(textureAlias, color, transform);
	}

	void Renderer::SubmitLabel(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color)
	{
		RenderLabel(label, transform, color, s_Data->m_Camera->GetViewProjectionMatrix());
	}

	void Renderer::UISubmitTextured(const std::string& textureAlias, const glm::mat4& transform)
	{
		s_Data->m_UIRenderer.Submit(textureAlias, glm::vec4(1.0f), transform);
	}

	void Renderer::UISubmitColored(const glm::vec4& color, const glm::mat4& transform)
	{
		s_Data->m_UIRenderer.Submit("texture_white", color, transform);
	}

	void Renderer::UISubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform)
	{
		s_Data->m_UIRenderer.Submit(textureAlias, color, transform);
	}

	void Renderer::UISubmitLabel(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color)
	{
		RenderLabel(label, transform, color, s_Data->m_UICamera->GetViewProjectionMatrix());
	}

	void Renderer::UISubmit(const Ref<Texture> texture, const glm::vec4& color, const glm::mat4& transform)
	{
		// TODO: render simple texture.
	}

	void Renderer::Resize(uint32_t width, uint32_t height)
	{
		s_Data->m_UICamera->SetProjection(0, (float)width, 0, (float)height);
	}

	void Renderer::RenderLabel(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color, const glm::mat4& viewProjection)
	{
		RendererAPI::SetDepthTest(false);
		const Ref<VertexArray>& vertexArray = label->GetVertexArray();
		s_Data->m_FontShader->Bind();

		s_Data->m_FontShader->SetMat4("u_ViewProjection", viewProjection);
		s_Data->m_FontShader->SetMat4("u_Transform", transform);
		s_Data->m_FontShader->SetFloat4("u_Color", color);
		s_Data->m_FontShader->SetInt("u_Texture", 0);

		label->GetFont()->GetTexture()->Bind();
		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}
}