#pragma once

enum class ErrorCode : int32
{
	SIGNIN = 1000, // 로그인
	SIGNIN_FAIL,	// 로그인 실패
	SIGNIN_DUPLICATE, // 중복 로그인
	SIGNIN_AUTOLOIGN_FAIL, // 자동 로그인 실패
	SIGNIN_ALREADY, // 이미 로그인 중

	SIGNUP = 2000, // 회원가입
	SIGNUP_FAIL, // 회원가입 실패
	SIGNUP_ID_DUPLICATE, // ID 중복
	SIGNUP_NICKNAME_DUPLICATE, // ID 중복

	ROOM = 5000, // 방
	ROOM_ENTER_FAIL, // 방 입장 실패
	ROOM_LEAVE_FAIL, // 방 퇴장 실패
	ROOM_PASSWORD_INCORRECT, // 방 비밀번호 틀림
	ROOM_FULL, // 방이 꽉 참
	ROOM_NOT_FOUND, // 방을 찾을 수 없음
	ROOM_ALREADY_ENTERED, // 이미 방에 입장함
	ROOM_NOT_ENTERED, // 방에 입장하지 않음
	ROOM_PLAYING, // 방이 플레이 중임
	ROOM_NOT_PLAYING, // 방이 플레이 중이지 않음
	ROOM_
};
