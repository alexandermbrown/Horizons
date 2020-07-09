#include "pch.h"
#include "HUDLayer.h"

HUDLayer::HUDLayer()
{

}

void HUDLayer::OnAttach()
{

}

void HUDLayer::OnDetach()
{

}

void HUDLayer::OnUpdate(float dt)
{
	li::Renderer::BeginScene(nullptr);

	li::UIRenderInfo ui_default;
	ui_default.Color = { 0.3f, 0.3f, 0.3f, 0.5f };

	li::UI::Begin();

	li::UI::BeginChild(&ui_default);

	li::UI::SetSize(0, 80);
	li::UI::SetBehave(LAY_BOTTOM | LAY_HFILL);
	li::UI::SetMargins(30, 0, 30, 30);
	li::UI::SetContain(LAY_ROW);

	for (int i = 0; i < 10; i++)
	{
		li::UI::BeginChild(&ui_default);

		li::UI::SetSize(60, 60);
		li::UI::SetMargins(10, 10, 10, 10);

		li::UI::EndChild();
	}

	li::UI::EndChild();
	li::UI::End();

	li::Renderer::EndScene();
}

void HUDLayer::OnImGuiRender()
{

}

void HUDLayer::OnEvent(SDL_Event* event)
{

}
