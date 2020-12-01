#include "pch.h"
#include "PlayerSystem.h"

#include "PlayerComponents.h"
#include "Horizons/Gameplay/Physics/PhysicsComponents.h"
#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/Gameplay/Components.h"

#include "Horizons/Core/Game.h"

void PlayerSystem::Init(entt::registry& registry, SyncEventQueue* queue)
{
	entt::entity player = registry.create();
	registry.emplace<cp::player>(player);

	// Sync player with rendering thread.
	cp::sync_manager& sync_manager = registry.ctx<cp::sync_manager>();

	uint64_t sync_id = sync_manager.count;
	sync_manager.count++;

	registry.emplace<cp::sync>(player, sync_id);
	registry.emplace<cp::sync_transform>(player, sync_id, glm::vec3{ 5.0f, 1.0f, 0.0f }  );

	cp::texture* texture = new cp::texture{ "test_small" }; // TODO: set player texture.
	cp::sync_transform* transform = new cp::sync_transform{ sync_id, { 5.0f, 1.0f, 0.0f } };

	queue->enqueue(SyncEvent::CreateEntity(sync_id));
	queue->enqueue(SyncEvent::AddComponent<cp::player>(sync_id, nullptr));
	queue->enqueue(SyncEvent::AddComponent<cp::sync_transform>(sync_id, transform));
	queue->enqueue(SyncEvent::AddComponent<cp::texture>(sync_id, texture));

	// Init physics body.
	cp::physics_body& physBody = registry.emplace<cp::physics_body>(player);

	// Get physics world from the registry.
	cp::physics_world& world = registry.ctx<cp::physics_world>();

	// Create the player's physics body.
	b2BodyDef playerBodyDef;
	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set(5.0f, 1.0f); // TODO: define where the player is in the world.
	physBody.body = world.world->CreateBody(&playerBodyDef);

		
	b2CircleShape playerShape;
	playerShape.m_p.Set(0.0f, 0.0f);
	playerShape.m_radius = 0.5f;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &playerShape;
	fixtureDef.density = 1.0f; // TODO: Set player density.
	fixtureDef.friction = 0.3f; // TODO: Set player friction.
	fixtureDef.restitution = 0.0f; // TODO: Set player restitution.

	physBody.body->CreateFixture(&fixtureDef);
}

void PlayerSystem::Update(entt::registry& registry, li::duration::us dt)
{
	auto view = registry.view<cp::player, cp::physics_body>();
	cp::player& player = view.get<cp::player>(view.front());
	cp::physics_body& body = view.get<cp::physics_body>(view.front());

	player.move_direction = {
		(player.right ? 1.0f : 0.0f) - (player.left ? 1.0f : 0.0f),
		(player.up ? 1.0f : 0.0f) - (player.down ? 1.0f : 0.0f)
	};

	// Ensure the move direction is of length 1.
	if (glm::length(player.move_direction) > 1.0f)
	{
		player.move_direction = glm::normalize(player.move_direction);
	}
	
	// Apply an impulse to the physics body to get instant movement.
	b2Vec2 vel = body.body->GetLinearVelocity();
	b2Vec2 desired_vel = { player.move_direction.x * player.move_speed, player.move_direction.y * player.move_speed };
	b2Vec2 delta_vel = desired_vel - vel;

	// Only apply if changed.
	if (delta_vel != b2Vec2_zero)
		body.body->ApplyLinearImpulse(body.body->GetMass() * delta_vel, body.body->GetWorldCenter(), true);
}

void PlayerSystem::OnEvent(entt::registry& registry, SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN)
	{
		ConfigStore& config = Game::Get()->GetConfig();

		if (event->key.keysym.scancode == config.Get("keybind_move_left").GetInt())
		{
			auto view = registry.view<cp::player>();
			cp::player& player = view.get<cp::player>(view.front());

			player.left = true;
		}
		else if (event->key.keysym.scancode == config.Get("keybind_move_right").GetInt())
		{
			auto view = registry.view<cp::player>();
			cp::player& player = view.get<cp::player>(view.front());

			player.right = true;
		}
		else if (event->key.keysym.scancode == config.Get("keybind_move_up").GetInt())
		{
			auto view = registry.view<cp::player>();
			cp::player& player = view.get<cp::player>(view.front());

			player.up = true;
		}
		else if (event->key.keysym.scancode == config.Get("keybind_move_down").GetInt())
		{
			auto view = registry.view<cp::player>();
			cp::player& player = view.get<cp::player>(view.front());

			player.down = true;
		}
	}
	else if (event->type == SDL_KEYUP)
	{
		ConfigStore& config = Game::Get()->GetConfig();

		if (event->key.keysym.scancode == config.Get("keybind_move_left").GetInt())
		{
			auto view = registry.view<cp::player>();
			cp::player& player = view.get<cp::player>(view.front());

			player.left = false;
		}
		else if (event->key.keysym.scancode == config.Get("keybind_move_right").GetInt())
		{
			auto view = registry.view<cp::player>();
			cp::player& player = view.get<cp::player>(view.front());

			player.right = false;
		}
		else if (event->key.keysym.scancode == config.Get("keybind_move_up").GetInt())
		{
			auto view = registry.view<cp::player>();
			cp::player& player = view.get<cp::player>(view.front());

			player.up = false;
		}
		else if (event->key.keysym.scancode == config.Get("keybind_move_down").GetInt())
		{
			auto view = registry.view<cp::player>();
			cp::player& player = view.get<cp::player>(view.front());

			player.down = false;
		}
	}
}
