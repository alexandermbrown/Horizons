#include "GameLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

GameLayer::GameLayer()
	: Layer("GameLayer"),
	m_Camera((float)li::Application::Get().GetWindow()->GetWidth() / (float)li::Application::Get().GetWindow()->GetHeight(), 10.0f, true),
	m_QuadColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
{
	m_Label = li::CreateRef<li::Label>(L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec lacinia eget sem in elementum.", 20.0f, li::ResourceManager::Get<li::Font>("Lato-Regular"));
	m_Transform = glm::mat4(1.0f);

	li::Renderer::AddTextureAtlas(li::ResourceManager::Get<li::TextureAtlas>("atlas_test"));

	
}

void GameLayer::OnAttach()
{
	m_AudioSource = li::CreateRef<li::AudioSource>();
	m_AudioSource->SetAudio(li::ResourceManager::Get<li::Audio>("audio_wind"));
	m_AudioSource->Play();
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(float dt)
{
	m_Camera.OnUpdate(dt);

	li::Renderer::BeginScene(&m_Camera.GetCamera());

	li::Renderer::SubmitColored(m_QuadColor, glm::translate(m_Transform, glm::vec3(2.0f, 1.0f, 0.0f)));
	li::Renderer::SubmitColoredTexture("test_small", m_QuadColor, m_Transform);
	li::Renderer::SubmitLabel(m_Label, glm::scale(glm::mat4(1.0f), {0.01f, 0.01f, 1.0f}), glm::vec4(1.0f));


	glm::mat4 uiTransform = glm::translate(glm::mat4(1.0f), { 20.0f, 20.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { 200.0f, 500.0f, 0.9f });

	//li::Renderer::UISubmitColored({ 0.3f, 0.8f, 0.8f, 1.0f }, uiTransform);
	li::Renderer::UISubmitLabel(m_Label, glm::translate(glm::mat4(1.0f), { 100.0f, 100.0f, 1.0f }), glm::vec4(1.0f));

	li::Renderer::EndScene();
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("Horizons");
	ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));
	ImGui::End();
}

void GameLayer::OnEvent(li::Event& event)
{
	m_Camera.OnEvent(event);
}
