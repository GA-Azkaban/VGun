#pragma once

enum ErrorCode : int32
{
	SIGNIN_FAIL = 1001,	// 로그인 실패


	SIGNUP_FAIL = 1004, // 회원가입 실패
	SIGNUP_ID_DUPLICATE = 1002, // ID 중복
	SIGNUP_NICKNAME_DUPLICATE = 1003, // ID 중복
	
	
};