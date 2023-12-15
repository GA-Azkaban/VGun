#include "EventSystem.h"
#include "RenderSystem.h"
#include "EventHandler.h"
#include "..\\HODO3DGraphicsInterface\\IRenderable.h"

namespace HDEngine
{
	EventSystem::EventSystem()
	{

	}

	void EventSystem::InvokeClickEvent(float x, float y)
	{
		// normalizedX = 0, normalizedY = 0 -> left top
		int screenWidth = RenderSystem::Instance().GetScreenWidth();
		int screenHeight = RenderSystem::Instance().GetScreenHeight();
		float normalizedX = x / screenWidth;
		float normalizedY = y / screenHeight;

		//IRenderable* selectedObject = RenderSystem.Click(x, y);
		//EventHandler::CallEvents(selectedObject, [](EventHandler* eventHandler) { eventHandler->OnClick(); });
	}
}