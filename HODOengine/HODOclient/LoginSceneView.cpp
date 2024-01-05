#include "LoginSceneView.h"

LoginSceneView::LoginSceneView()
{
}

LoginSceneView::~LoginSceneView()
{

}

void LoginSceneView::Initialize()
{
	_scene = API::CreateScene("Login");

	// 캔버스 생성

	// 아이디, 비밀번호 텍스트 박스

	// 로그인, 회원가입 버튼



	API::LoadScene(_scene);
}
