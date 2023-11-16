#pragma once

#include <vector>

using namespace DirectX;

static HRESULT hr = NULL;

class XLParticleEffect;

class XLParticleSystem
{
public:
	XLParticleSystem();
	~XLParticleSystem();

private:
	XLParticleSystem(const XLParticleSystem* rhs);
	XLParticleSystem& operator=(const XLParticleSystem& rhs);

public:
	void SetEyePos(const XMFLOAT3& eyePosW);
	void SetEmitPos(const XMFLOAT3& emitPosW);
	void SetEmitDir(const XMFLOAT3& emitDirW);

	float GetAge() const { return mAge; }

public:
	void AddParticleEffect(XLParticleEffect* effect);

public:
	static ID3DX11Effect* CompileFX(LPCWSTR fxFilePath);
	static ID3D11ShaderResourceView* LoadTexture(LPCWSTR textureFilePath);

public:
	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT maxParticles);
	void Update(float dt, float gameTime, XMMATRIX ViewProj, XMFLOAT3 CameraPosition);
	void Render();
	void Reset();

private:
	std::vector<XLParticleEffect*> m_Effects;

private:
	float mGameTime;
	float mTimeStep;
	float mAge;
	XMFLOAT3 mEyePosW;
	XMFLOAT3 mEmitPosW;
	XMFLOAT3 mEmitDirW;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

public:
	static UINT GetMaxParticles();
};

