#include "pch.h"
#include "MainMenuLayer.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/UI/UIComponents.h"
#include "Horizons/UI/UILayoutSystem.h"
#include "Horizons/UI/UIRenderSystem.h"
#include "Horizons/UI/UITransformSortSystem.h"
#include "Horizons/UI/UIHoverSystem.h"
#include "Horizons/UI/UIClickSystem.h"

#include "Horizons/Scenes/GameScene.h"
#include "Horizons/Gameplay/RNGSystem.h"
#include "Horizons/Rendering/FlickerSystem.h"

#include "Horizons/Scripting/UIPrototypes.h"

#include "glm/gtc/matrix_transform.hpp"

MainMenuLayer::MainMenuLayer()
	: Layer("MainMenuLayer"), m_Registry()
{
	UILayoutSystem::Init(m_Registry);

	entt::entity context_ent = m_Registry.view<cp::ui_context>().front();
	UIPrototypes::InstantiateUIPrototype(m_Registry, context_ent, "main_menu");

	cp::ui_context& context = m_Registry.get<cp::ui_context>(context_ent);
	context.rebuild = true;

	li::Renderer::AddTextureAtlas(li::ResourceManager::Get<li::TextureAtlas>("space_scene_1"));

	RNGSystem::Init(m_Registry);

	m_Registry.view<cp::ui_name>().each([this](entt::entity entity, cp::ui_name& name)
	{
		if (name.name == "button_play")
		{
			cp::ui_click& click = m_Registry.emplace<cp::ui_click>(entity);
			click.OnClickFn = [this](entt::registry& registry, entt::entity entity, int button) -> bool {
				if (button == 1)
				{
					m_TransitionScene = new GameScene();
				}
				return true;
			};
		}
		else if (name.name == "button_level_editor")
		{

		}
	});
}

MainMenuLayer::~MainMenuLayer()
{
	UILayoutSystem::Shutdown(m_Registry);
}

void MainMenuLayer::OnAttach()
{
}

void MainMenuLayer::OnDetach()
{
}

void MainMenuLayer::OnUpdate(float dt)
{
	UILayoutSystem::Update(m_Registry);
	UITransformSortSystem::SortTransforms(m_Registry);

	FlickerSystem::Update(m_Registry, dt);

	li::RendererAPI::BindDefaultRenderTarget();
	li::RendererAPI::Clear();

	li::Renderer::BeginUI();
	UIRenderSystem::Render(m_Registry);
	li::Renderer::EndUI();
	UIRenderSystem::RenderLabels(m_Registry);
}

void MainMenuLayer::OnImGuiRender()
{
}

void MainMenuLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		UILayoutSystem::OnWindowResize(m_Registry, event->window.data1, event->window.data2);
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		UIHoverSystem::OnMouseMove(m_Registry, event->motion.x, event->motion.y);
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		UIClickSystem::OnMouseDown(m_Registry, event->button.x, event->button.y, event->button.button);
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		UIClickSystem::OnMouseUp(m_Registry, event->button.x, event->button.y, event->button.button);
	}
}
