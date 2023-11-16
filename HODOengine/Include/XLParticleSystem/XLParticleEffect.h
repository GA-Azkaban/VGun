#pragma once

using namespace DirectX;

struct Particle
{
	XMFLOAT3 position;
	XMFLOAT3 velocity;
	XMFLOAT2 size;
	float age;
	unsigned int type;
};

class XLParticleSystem;

class XLParticleEffect
{
public:
	XLParticleEffect();
	~XLParticleEffect();

public:
	void Initialize(ID3DX11Effect* _pFX, ID3D11ShaderResourceView* _pTexSRV);
	virtual void Initialize();
	void Start();
	void Update();

public:
	// 쉐이더로 넘길 값들 Setter
	void SetViewProj(XMMATRIX VP) { m_VP = VP; }
	void SetGameTime(const float& GameTime) { m_GameTime = GameTime; }
	void SetTimeStep(const float& TimeStep) { m_TimeStep = TimeStep; }
	void SetEyePosW(const XMFLOAT3& EyePosW) { m_EyePosW = EyePosW; }
	void SetEmitPosW(const XMFLOAT3& EmitPosW) { m_EmitPosW = EmitPosW; }
	void SetEmitDirW(const XMFLOAT3& EmitDirW) { m_EmitDirW = EmitDirW; }
	void SetTexArray(ID3D11ShaderResourceView* TexSRV) { m_pTexSRV = TexSRV; }

	void SetParticleInterval(const float& ParticleInterval) { m_ParticleInterval = ParticleInterval; }
	void SetParticleSize(const float& ParticleSize) { m_ParticleSize = ParticleSize; }
	void SetParticleLife(const float& ParticleLife) { m_ParticleLife = ParticleLife; }
	void SetRandomVectorIntensity(const float& RandomVectorIntensity) { m_RandomVectorIntensity = RandomVectorIntensity; }
	void SetRandomVectorRange(const float& RandomVectorRange) { m_RandomVectorRange = RandomVectorRange; }
	void SetAccelIntensity(const float& AccelIntensity) { m_AccelIntensity = AccelIntensity; }

protected:
	// 쉐이더 내부로 전달할 값들

	XMMATRIX m_VP;
	float m_GameTime;
	float m_TimeStep;
	XMFLOAT3 m_EyePosW;
	XMFLOAT3 m_EmitPosW;
	XMFLOAT3 m_EmitDirW;
	ID3D11ShaderResourceView* m_pTexSRV;

	float m_ParticleInterval;
	float m_ParticleSize;
	float m_ParticleLife;
	float m_RandomVectorIntensity;
	float m_RandomVectorRange;
	float m_AccelIntensity;

public:
	XLParticleSystem* m_ParticleSystem;
	bool mFirstRun;

private:
	void SendValuesToFX();

public:
	void BuildVB();

private:
	void SetShaderVariable(LPCSTR variableName, const float& value);
	void SetShaderVariable(LPCSTR variableName, const XMMATRIX& value);
	void SetShaderVariable(LPCSTR variableName, const XMFLOAT3& value);
	void SetShaderVariable(LPCSTR variableName, ID3D11ShaderResourceView* value);

public:
	// 버텍스 버퍼들
	ID3D11Buffer* mInitVB; // Emitter Particle을 위한 InitVB
	ID3D11Buffer* mDrawVB;
	ID3D11Buffer* mStreamOutVB;

public:
	// FX 프레임워크 관련
	ID3DX11Effect* m_pFX;
	ID3DX11EffectTechnique* StreamOutTech;
	ID3DX11EffectTechnique* DrawTech;
	D3DX11_TECHNIQUE_DESC SOtechDesc;
	D3DX11_TECHNIQUE_DESC DrawtechDesc;


public:
	ID3D11InputLayout* ParticleInputLayout;

public:
	// Device, Device Context
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
};

class FireParticleEffect : public XLParticleEffect
{
public:
	FireParticleEffect() {};
	~FireParticleEffect() {};

	virtual void Initialize() override;
};

class SmokeParticleEffect : public XLParticleEffect
{
public:
	SmokeParticleEffect() {};
	~SmokeParticleEffect() {};

	virtual void Initialize() override;
};

class AshParticleEffect : public XLParticleEffect
{
public:
	AshParticleEffect() {};
	~AshParticleEffect() {};

	virtual void Initialize() override;
};

class RainParticleEffect : public XLParticleEffect
{
public:
	RainParticleEffect() {};
	~RainParticleEffect() {};

	virtual void Initialize() override;
};