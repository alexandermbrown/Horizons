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

		s_Data->ViewProjUB = UniformBuffer::Create("ViewProjectionMatrices", 0, {
			{ "u_ViewProj", ShaderDataType::Mat4 },
		});
		s_Data->ViewProjUB->BindToSlot();

		s_Data->TransformMatrixUB = UniformBuffer::Create("TransformMatrix", 1, {
			{ "u_Transform", ShaderDataType::Mat4 },
		});
		s_Data->TransformMatrixUB->BindToSlot();

		s_Data->ColorUB = UniformBuffer::Create("Color", 2, {
			{ "u_Color", ShaderDataType::Float4 },
		});
		s_Data->ColorUB->BindToSlot();
		
		s_Data->Camera = nullptr;

		s_Data->TextureShader = Shader::Create("data/shaders/Texture.glsl");
		s_Data->TextureShader->AddUniformBuffer(s_Data->ViewProjUB);
		s_Data->TextureShader->AddUniformBuffer(s_Data->TransformMatrixUB);

		Ref<Window>& window = Application::Get()->GetWindow();
		s_Data->UICamera = CreateScope<OrthographicCamera>(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight());

		RendererAPI::Init();
		RendererAPI::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		// SETUP WHITE TEXTURE
		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		whiteTexture->SetData(&data, sizeof(data));

		// SETUP SCENE FLAT COLOR TEXTURE ALTAS
		Ref<TextureAtlas> flatColorAtlas = CreateRef<TextureAtlas>(TextureAtlas(whiteTexture, {
			{ "texture_white", glm::vec4(0.5f) }
		}));

		s_Data->SceneRenderer = BatchRenderer({ 0.5f, 0.5f });
		s_Data->UIRenderer = BatchRenderer({ 0.0f, 0.0f });
		s_Data->SceneRenderer.AddTextureAtlas(flatColorAtlas);
		s_Data->UIRenderer.AddTextureAtlas(flatColorAtlas);

		//////////////////////////////////
		// Create Textured Quad Buffers //
		//////////////////////////////////

		s_Data->QuadVA = VertexArray::Create();

		float quadVertices[16] = {
			0.0, 0.0, 0.0f, 0.0f,
			1.0, 0.0, 1.0f, 0.0f,
			1.0, 1.0, 1.0f, 1.0f,
			0.0, 1.0, 0.0f, 1.0f
		};

		Ref<VertexBuffer> quadVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices), BufferUsage::StaticDraw);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		quadVB->SetLayout({
			{ ShaderDataType::Float2, "a_Position", 0 },
			{ ShaderDataType::Float2, "a_TexCoord", 1 }
			});

		s_Data->QuadVA->SetIndexBuffer(quadIB);
		s_Data->QuadVA->AddVertexBuffer(quadVB);
	}

	void Renderer::InitPostResourceLoad()
	{
		s_Data->FontShader = ResourceManager::Get<Shader>("shader_label");
		s_Data->SceneRenderer.SetUniformBuffer(s_Data->ViewProjUB);
		s_Data->UIRenderer.SetUniformBuffer(s_Data->ViewProjUB);
		s_Data->ResourcesLoaded = true;
	}

	void Renderer::Shutdown()
	{
		s_Data.reset();
	}

	void Renderer::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		s_Data->SceneRenderer.AddTextureAtlas(atlas);
		s_Data->UIRenderer.AddTextureAtlas(atlas);
	}

	void Renderer::BeginScene(OrthographicCamera* camera)
	{
		s_Data->Camera = camera;

		s_Data->ViewProjUB->SetMat4("u_ViewProj", camera->GetViewProjectionMatrix());
		s_Data->ViewProjUB->UploadData();

		s_Data->SceneRenderer.BeginScene(camera);
	}

	void Renderer::EndScene()
	{
		RendererAPI::SetDepthTest(true);
		s_Data->SceneRenderer.EndScene();
	}

	void Renderer::BeginUI()
	{
		s_Data->ViewProjUB->SetMat4("u_ViewProj", s_Data->UICamera->GetViewProjectionMatrix());
		s_Data->ViewProjUB->UploadData();

		s_Data->UIRenderer.BeginScene(s_Data->UICamera.get());
	}

	void Renderer::EndUI()
	{
		RendererAPI::SetDepthTest(true);
		s_Data->UIRenderer.EndScene();
	}

	void Renderer::SubmitTextured(const std::string& textureAlias, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer.Submit(textureAlias, glm::vec4(1.0f), transform);
	}

	void Renderer::SubmitColored(const glm::vec4& color, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer.Submit("texture_white", color, transform);
	}

	void Renderer::SubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer.Submit(textureAlias, color, transform);
	}

	void Renderer::SubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		LI_CORE_ASSERT(false, "Not implemented.");
		//RenderLabel(label, transform, color, s_Data->Camera->GetViewProjectionMatrix());
	}

	void Renderer::UISubmitTextured(const std::string& textureAlias, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer.Submit(textureAlias, glm::vec4(1.0f), transform);
	}

	void Renderer::UISubmitColored(const glm::vec4& color, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer.Submit("texture_white", color, transform);
	}

	void Renderer::UISubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer.Submit(textureAlias, color, transform);
	}

	void Renderer::UISubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		RenderLabel(label, transform, color);
	}

	void Renderer::UISubmit(const Ref<Texture>& texture, const glm::mat4& transform)
	{
		RendererAPI::SetDepthTest(true);
		s_Data->TextureShader->Bind();

		s_Data->TransformMatrixUB->SetMat4("u_Transform", transform);
		s_Data->TransformMatrixUB->UploadData();

		s_Data->ViewProjUB->SetMat4("u_ViewProj", s_Data->UICamera->GetViewProjectionMatrix());
		s_Data->ViewProjUB->UploadData();

		s_Data->TextureShader->SetTexture("u_Texture", 0);

		texture->Bind();
		s_Data->QuadVA->Bind();
		RendererAPI::DrawIndexed(s_Data->QuadVA);
	}

	void Renderer::Resize(int width, int height)
	{
		s_Data->UICamera->SetProjection(0, (float)width, 0, (float)height);
	}

	void Renderer::RenderLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		RendererAPI::SetDepthTest(false);
		const Ref<VertexArray>& vertexArray = label->GetVertexArray();
		s_Data->FontShader->Bind();

		s_Data->TransformMatrixUB->SetMat4("u_Transform", transform);
		s_Data->TransformMatrixUB->UploadData();

		s_Data->ColorUB->SetFloat4("u_Color", color);
		s_Data->ColorUB->UploadData();

		s_Data->FontShader->SetTexture("u_Texture", 0);

		label->GetFont()->GetTexture()->Bind();
		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}
}