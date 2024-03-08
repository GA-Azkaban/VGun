#pragma once
#include <vector>

namespace RocketCore::Graphics
{
	class QuadBuffer;
	class Outline;

	class OutlinePass
	{
	public:
		OutlinePass(QuadBuffer* quadBuffer);
		~OutlinePass();
		static std::vector<Outline*> outlinesList;

	private:
		QuadBuffer* _quadBuffer;
	};
}