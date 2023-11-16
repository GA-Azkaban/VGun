#include <SimpleMath.h>
#include <vector>

class CAnimation
{
public:
	CAnimation();

	void Update(float timeElapsed);
	void AccumulateQuaternion();

	int GetCurrentFrame() { return m_currentFrame; }
	bool IsFinished() {
		return m_isPosAnimFinished && m_isRotAnimFinished;
	}
	void SetFinished(bool isFinished) { 
		m_isPosAnimFinished = isFinished; 
		m_isRotAnimFinished = isFinished;
	}

	std::vector<int> posFrame;
	std::vector<int> rotFrame;
	std::vector<DirectX::SimpleMath::Vector3> posInfo;
	std::vector<DirectX::SimpleMath::Vector4> rotInfo;

	std::vector<DirectX::SimpleMath::Vector4> quaternionList;

private:
	int m_currentFrame = 0;
	//bool m_isFinished = false;
	bool m_isPosAnimFinished = false;
	bool m_isRotAnimFinished = false;
};
