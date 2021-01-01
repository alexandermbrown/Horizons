#include "lipch.h"
#include "Renderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Li
{
	Unique<Renderer::RendererData> Renderer::s_Data;

	void Renderer::Init()
	{
		s_Data = MakeUnique<Renderer::RendererData>();

		Application::Get().GetWindow().GetContext()->SetDepthTest(false);
		Application::Get().GetWindow().GetContext()->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		s_Data->ViewProjUB = UniformBuffer::Create("ViewProjectionMatrices", 0, ShaderType::Vertex, {
			{ "u_ViewProj", ShaderDataType::Mat4 },
		});
		s_Data->ViewProjUB->BindToSlot();

		s_Data->TransformMatrixUB = UniformBuffer::Create("TransformMatrix", 1, ShaderType::Vertex, {
			{ "u_Transform", ShaderDataType::Mat4 },
		});
		s_Data->TransformMatrixUB->BindToSlot();

		s_Data->FontUB = UniformBuffer::Create("Font", 2, ShaderType::Fragment, {
			{ "u_Color", ShaderDataType::Float4 },
			{ "u_DistanceFactor", ShaderDataType::Float }
		});
		s_Data->FontUB->BindToSlot();
		
		s_Data->Camera = nullptr;

		s_Data->TextureShader = Li::ResourceManager::Get<Shader>("shader_splash");

		Window& window = Application::Get().GetWindow();
		s_Data->UICamera = MakeUnique<OrthographicCamera>(0.0f, (float)window.GetWidth(), 0.0f, (float)window.GetHeight());
		
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
			{ ShaderDataType::Float2, "POSITION", 0 },
			{ ShaderDataType::Float2, "TEXCOORD", 1 }
		});

		s_Data->QuadVA->SetIndexBuffer(quadIB);
		s_Data->QuadVA->AddVertexBuffer(quadVB);
		s_Data->QuadVA->Finalize(s_Data->TextureShader);
	}

	void Renderer::InitPostResourceLoad()
	{
		s_Data->FontShader = ResourceManager::Get<Shader>("shader_label");

		// SETUP WHITE TEXTURE
		uint32_t data = 0xffffffff;
		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1, 4, &data);

		// SETUP SCENE FLAT COLOR TEXTURE ALTAS
		Ref<TextureAtlas> flatColorAtlas = MakeRef<TextureAtlas>(TextureAtlas(whiteTexture, {
			{ "texture_white", glm::vec4(0.5f) }
		}));

		s_Data->SceneRenderer = MakeUnique<BatchRenderer>(glm::vec2{ 0.5f, 0.5f }, s_Data->ViewProjUB, s_Data->TransformMatrixUB);
		s_Data->UIRenderer = MakeUnique<BatchRenderer>(glm::vec2{ 0.0f, 0.0f }, s_Data->ViewProjUB, s_Data->TransformMatrixUB);
		s_Data->SceneLineRenderer = MakeUnique<LineBatchRenderer>(s_Data->ViewProjUB);

		s_Data->SceneRenderer->AddTextureAtlas(flatColorAtlas);
		s_Data->UIRenderer->AddTextureAtlas(flatColorAtlas);
		s_Data->ResourcesLoaded = true;
	}

	void Renderer::Shutdown()
	{
		s_Data.reset();
	}

	void Renderer::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		s_Data->SceneRenderer->AddTextureAtlas(atlas);
		s_Data->UIRenderer->AddTextureAtlas(atlas);
	}

	void Renderer::BeginScene(OrthographicCamera* camera)
	{
		s_Data->Camera = camera;

		s_Data->ViewProjUB->SetMat4("u_ViewProj", camera->GetViewProjectionMatrix());
		s_Data->ViewProjUB->UploadData();

		s_Data->SceneRenderer->BeginScene();
		s_Data->SceneLineRenderer->BeginScene();
	}

	void Renderer::EndScene()
	{
		s_Data->SceneRenderer->EndScene();
		s_Data->SceneLineRenderer->EndScene();
	}

	void Renderer::BeginUI()
	{
		s_Data->ViewProjUB->SetMat4("u_ViewProj", s_Data->UICamera->GetViewProjectionMatrix());
		s_Data->ViewProjUB->UploadData();

		s_Data->UIRenderer->BeginScene();
	}

	void Renderer::EndUI()
	{
		s_Data->UIRenderer->EndScene();
	}

	void Renderer::SubmitTextured(const std::string& textureAlias, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer->Submit(textureAlias, glm::vec4(1.0f), transform, crop);
	}

	void Renderer::SubmitColored(const glm::vec4& color, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer->Submit("texture_white", color, transform, false);
	}

	void Renderer::SubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer->Submit(textureAlias, color, transform, crop);
	}

	void Renderer::SubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		RenderLabel(label, transform, color);
	}

	void Li::Renderer::SubmitLine(const glm::vec4& color, const glm::vec3& point1, const glm::vec3& point2)
	{
		s_Data->SceneLineRenderer->Submit(color, point1, point2);
	}

	void Li::Renderer::SubmitCircle(const glm::vec4& color, const glm::vec3& center, float radius)
	{
		glm::vec3 prev_point = {
				center.x + radius,
				center.y,
				center.z
		};

		constexpr int resolution = 32;
		for (int i = 1; i <= resolution; i++)
		{
			float angle = (float)i / (float)resolution * 2.0f * (float)M_PI;

			glm::vec3 current_point = {
				center.x + std::cos(angle) * radius,
				center.y + std::sin(angle) * radius,
				center.z
			};
			Li::Renderer::SubmitLine(color, prev_point, current_point);
			prev_point = current_point;
		}
	}

	void Renderer::Submit(const Ref<Texture>& texture, const glm::mat4& transform)
	{
		RenderQuad(texture, transform, s_Data->Camera->GetViewProjectionMatrix());
	}

	void Renderer::UISubmitTextured(const std::string& textureAlias, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer->Submit(textureAlias, glm::vec4(1.0f), transform, crop);
	}

	void Renderer::UISubmitColored(const glm::vec4& color, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer->Submit("texture_white", color, transform, false);
	}

	void Renderer::UISubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer->Submit(textureAlias, color, transform, crop);
	}

	void Renderer::UISubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		RenderLabel(label, transform, color);
	}

	void Renderer::UISubmit(const Ref<Texture>& texture, const glm::mat4& transform)
	{
		RenderQuad(texture, transform, s_Data->UICamera->GetViewProjectionMatrix());
	}

	void Renderer::Resize(int width, int height)
	{
		s_Data->UICamera->SetProjection(0, (float)width, 0, (float)height);
	}

	void Renderer::RenderQuad(const Ref<Texture>& texture, const glm::mat4& transform, const glm::mat4& view_projection)
	{
		s_Data->TextureShader->Bind();

		s_Data->TransformMatrixUB->SetMat4("u_Transform", transform);
		s_Data->TransformMatrixUB->UploadData();

		s_Data->ViewProjUB->SetMat4("u_ViewProj", view_projection);
		s_Data->ViewProjUB->UploadData();

		s_Data->TextureShader->SetTexture("u_Texture", 0);

		s_Data->ViewProjUB->Bind();
		s_Data->TransformMatrixUB->Bind();
		texture->Bind();
		s_Data->QuadVA->Bind();
		Application::Get().GetWindow().GetContext()->SetDrawMode(DrawMode::Triangles);
		Application::Get().GetWindow().GetContext()->DrawIndexed(s_Data->QuadVA->GetIndexBuffer()->GetCount());
	}

	void Renderer::RenderLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		const Ref<VertexArray>& vertexArray = label->GetVertexArray();
		s_Data->FontShader->Bind();

		s_Data->TransformMatrixUB->SetMat4("u_Transform", transform);
		s_Data->TransformMatrixUB->UploadData();

		s_Data->FontUB->SetFloat4("u_Color", color);
		s_Data->FontUB->SetFloat("u_DistanceFactor", label->GetDistanceFactor());
		s_Data->FontUB->UploadData();

		s_Data->FontShader->SetTexture("u_Texture", 0);

		s_Data->ViewProjUB->Bind();
		s_Data->TransformMatrixUB->Bind();
		s_Data->FontUB->Bind();
		label->GetFont()->GetTexture()->Bind();
		vertexArray->Bind();
		Application::Get().GetWindow().GetContext()->SetDrawMode(DrawMode::Triangles);
		Application::Get().GetWindow().GetContext()->DrawIndexed(vertexArray->GetIndexBuffer()->GetCount());
		vertexArray->Unbind();
	}
}