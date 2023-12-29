#pragma once
#include <vector>
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
///--------------------------------------------------
/// Animation Data를 저장하기 위한 클래스
///--------------------------------------------------

class CAnimation_pos
{
public:
    int m_time;
    Vector3	m_pos;
};

class CAnimation_rot
{
public:
    int				m_time;
    Vector3		m_rot;
    float			m_angle;
    Quaternion	m_rotQT_accumulation;		// 상대변량이기 때문에 이전 값들과 누적해서 가지고 있는다 (즉 지금까지의 절대량으로 변환한다)
};

class CAnimation_scl {

public:
    int				m_time;
    Vector3		m_scale;
    Vector3		m_scaleaxis;
    float			m_scaleaxisang;
    Quaternion	m_scalerotQT_accumulation;
};


///--------------------------------------------------
/// 위 클래스를 조합한 Animation 클래스
///--------------------------------------------------
class Animation {

public:

    // 애니메이션 데이터의 이름. 유일한 참조수단이다
    std::string						        m_nodename;
    std::vector<CAnimation_pos>		m_position;
    std::vector<CAnimation_rot>		m_rotation;
    std::vector<CAnimation_scl>		m_scale;

    int		m_ticksperFrame;	// 한 프레임의 틱 크기. 이것은 AnimationTable을 가지는 각각의 오브젝트마다 다를 수 있다.
    int		m_minKeyframe;		// 키 프레임간의 최소크기. 이것의 위의것과 꼭 일치하지는 않는다.
};