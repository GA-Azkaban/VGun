﻿#include "DynamicBoxCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::DynamicBoxCollider::DynamicBoxCollider()
	: _width(1.0f), _height(1.0f), _depth(1.0f)
{
	_cubeDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCubePrimitive();
	_debugStruct = _cubeDebugStruct;
}

void HDData::DynamicBoxCollider::SetVolume(float w, float h, float d)
{
	_width = w;
	_height = h;
	_depth = d;
}

float HDData::DynamicBoxCollider::GetWidth() const
{
	//return _width * _scaleOffset.x * GetGameObject()->GetTransform()->GetWorldScale().x;
	return _width;
}

float HDData::DynamicBoxCollider::GetHeight() const
{
	//return _height * _scaleOffset.x * GetGameObject()->GetTransform()->GetWorldScale().y;
	return _height;
}

float HDData::DynamicBoxCollider::GetDepth() const
{
	//return _depth * _scaleOffset.x * GetGameObject()->GetTransform()->GetWorldScale().z;
	return _depth;
}

void HDData::DynamicBoxCollider::DrawDebug()
{
	Matrix colWorld = Matrix::Identity;
	colWorld *= GetTransformMatrix();
	colWorld *= GetTransform()->GetWorldTM();
	_debugStruct->worldTM = colWorld;
	_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };

	_cubeDebugStruct->widthHeightDepth = { _width, _height, _depth };
}
