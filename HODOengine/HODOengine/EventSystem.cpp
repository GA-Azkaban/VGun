#include "EventSystem.h"
#include "RenderSystem.h"
#include "EventHandler.h"
#include "InputSystem.h"
#include "InputData.h"
#include "MathHeader.h"
#include "..\\HODO3DGraphicsInterface\\IRenderable.h"

namespace HDEngine
{
	EventSystem::EventSystem()
	{

	}

	void EventSystem::InvokeEvent()
	{
		if (InputSystem::Instance().GetKeyDown(MOUSE_LEFT))
		{
			Vector2 mousePosition = InputSystem::Instance().GetMousePosition();
			InvokeClickEvent(mousePosition.x, mousePosition.y);
		}
		else
		{
			Vector2 mousePosition = InputSystem::Instance().GetMousePosition();
			InvokeHoverEvent(mousePosition.x, mousePosition.y);
		}
	}

	void EventSystem::InvokeClickEvent(float x, float y)
	{
		// Normalize the input coordinates
		// normalizedX = 0, normalizedY = 0 -> left top
		int screenWidth = RenderSystem::Instance().GetScreenWidth();
		int screenHeight = RenderSystem::Instance().GetScreenHeight();
		float normalizedX = x / screenWidth;
		float normalizedY = y / screenHeight;

		//IRenderable* selectedObject = RenderSystem.Click(x, y);
		//if(selectedObejct != nullptr)
		// {
		//		EventHandler::CallEvents(selectedObject, [](EventHandler* eventHandler) { eventHandler->OnClick(); });
		// }

	}

	void EventSystem::InvokeHoverEvent(float x, float y)
	{
		// Normalize the input coordinates
		// normalizedX = 0, normalizedY = 0 -> left top
		int screenWidth = RenderSystem::Instance().GetScreenWidth();
		int screenHeight = RenderSystem::Instance().GetScreenHeight();
		float normalizedX = x / screenWidth;
		float normalizedY = y / screenHeight;

		//IRenderable* hoveredObject = RenderSystem.Click(x, y);
		//if(hoveredObject != nullptr)
		// {
		//		EventHandler::CallEvents(hoveredObject, [](EventHandler* eventHandler) { eventHandler->OnHover(); });
		// }
	}

}