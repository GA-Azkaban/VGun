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
	auto canvas = API::CreateObject(_scene);

	// 아이디, 비밀번호 텍스트 박스
	// 비밀번호를 받을 때 텍스트를 한글자씩 받고 바로 별모양으로 변경해야 한다
	auto idBox = API::CreateTextbox(_scene);
	auto passwordBox = API::CreateTextbox(_scene);
	auto nicknameBox = API::CreateTextbox(_scene);

	auto loginButton = API::CreateButton(_scene);
	auto joinButton = API::CreateButton(_scene);

	// 로그인, 회원가입 버튼


	API::LoadScene(_scene);
}
