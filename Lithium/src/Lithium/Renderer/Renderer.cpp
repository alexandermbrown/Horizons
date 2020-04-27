#include "lipch.h"
#include "Renderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

namespace li
{
	Scope<Renderer> Renderer::s_Instance = CreateScope<Renderer>();

	Renderer::Renderer()
		: m_SceneRenderer({ 0.5f, 0.5f }), m_UIRenderer({ 0.0f, 0.0f })
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::InitImpl()
	{
		RendererAPI::Init();
		RendererAPI::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		// TODO: Change path to user defined.
		m_SceneRenderer.Init("shader_instance");
		m_UIRenderer.Init("shader_instance");

		// SETUP WHITE TEXTURE
		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		whiteTexture->SetData(&data, sizeof(data));

		// SETUP SCENE FLAT COLOR TEXTURE ALTAS
		Ref<TextureAtlas> sceneFlatColorAtlas = CreateRef<TextureAtlas>(TextureAtlas(whiteTexture, {
			{ "texture_white", glm::vec4(0.5f) }
		}));
		m_SceneRenderer.AddTextureAtlas(sceneFlatColorAtlas);

		// SETUP UI FLAT COLOR TEXTURE ALTAS
		Ref<TextureAtlas> uiFlatColorAtlas = CreateRef<TextureAtlas>(TextureAtlas(whiteTexture, {
			{ "texture_white", glm::vec4(0.5f) }
			}));
		m_UIRenderer.AddTextureAtlas(uiFlatColorAtlas);

		Ref<Window>& window = Application::Get().GetWindow();

		m_UICamera = CreateScope<OrthographicCamera>(0, window->GetWidth(), 0, window->GetHeight());
	}

	void Renderer::BeginSceneImpl(OrthographicCamera* camera)
	{
		m_FontShader = ResourceManager::Get<Shader>("shader_label");
		RendererAPI::Clear();
		m_Camera = camera;
		m_SceneRenderer.BeginScene(camera);
		m_UIRenderer.BeginScene(m_UICamera.get());
	}

	void Renderer::EndSceneImpl()
	{
		RendererAPI::SetDepthTest(true);
		m_SceneRenderer.EndScene();
		m_UIRenderer.EndScene();
	}

	void Renderer::SubmitTexturedImpl(const std::string& textureAlias, const glm::mat4& transform)
	{
		m_SceneRenderer.Submit(textureAlias, glm::vec4(1.0f), transform);
	}

	void Renderer::SubmitColoredImpl(const glm::vec4& color, const glm::mat4& transform)
	{
		m_SceneRenderer.Submit("texture_white", color, transform);
	}

	void Renderer::SubmitColoredTextureImpl(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform)
	{
		m_SceneRenderer.Submit(textureAlias, color, transform);
	}

	void Renderer::UISubmitTexturedImpl(const std::string& textureAlias, const glm::mat4& transform)
	{
		m_UIRenderer.Submit(textureAlias, glm::vec4(1.0f), transform);
	}

	void Renderer::UISubmitColoredImpl(const glm::vec4& color, const glm::mat4& transform)
	{
		m_UIRenderer.Submit("texture_white", color, transform);
	}

	void Renderer::UISubmitColoredTextureImpl(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform)
	{
		m_UIRenderer.Submit(textureAlias, color, transform);
	}

	void Renderer::ResizeImpl(uint32_t width, uint32_t height)
	{
		m_UICamera->SetProjection(0, width, 0, height);
	}

	void Renderer::RenderLabel(const Ref<Label> label, const glm::mat4& transform, const glm::vec4& color, const glm::mat4& viewProjection)
	{
		RendererAPI::SetDepthTest(false);
		const Ref<VertexArray>& vertexArray = label->GetVertexArray();
		m_FontShader->Bind();

		m_FontShader->SetMat4("u_ViewProjection", viewProjection);
		m_FontShader->SetMat4("u_Transform", transform);
		m_FontShader->SetFloat4("u_Color", color);
		m_FontShader->SetInt("u_Texture", 0);

		label->GetFont()->GetTexture()->Bind();
		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}
}