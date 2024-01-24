#include "LineRenderer.h"

namespace RocketCore::Graphics
{
	void LineRenderer::DrawLine(Vector3 startPos, Vector3 endPos, Color color)
	{
		auto startVec = DirectX::XMLoadFloat3(&startPos);
		auto endVec = DirectX::XMLoadFloat3(&endPos);
		auto colorVec = DirectX::XMLoadFloat4(&color);

		_lines.emplace_back(startVec, endVec, colorVec);
	}

	void LineRenderer::DrawLine(Vector3 startPos, Vector3 direction, float length, Color color)
	{
		auto dirVec = DirectX::XMLoadFloat3(&direction);
		dirVec = DirectX::XMVector3Normalize(dirVec);
		dirVec = DirectX::XMVectorScale(dirVec, length);

		auto startVec = DirectX::XMLoadFloat3(&startPos);
		auto endVec = DirectX::XMVectorAdd(startVec, dirVec);
		auto colorVec = DirectX::XMLoadFloat4(&color);

		_lines.emplace_back(startVec, endVec, colorVec);
	}

	std::vector<RocketCore::Graphics::LineRenderer::Line>& LineRenderer::GetLines()
	{
		return _lines;
	}

	void LineRenderer::Flush()
	{
		_lines.clear();
	}

}
