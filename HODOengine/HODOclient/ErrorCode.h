#pragma once

enum class ErrorCode : int32
{
	SIGNIN_FAIL = 1001,	// 로그인 실패


	SIGNUP_FAIL = 1004, // 회원가입 실패
	SIGNUP_ID_DUPLICATE = 1002, // ID 중복
	SIGNUP_NICKNAME_DUPLICATE = 1003, // ID 중복
	
	ROOM_ENTER_FAIL = 1005, // 방 입장 실패
	ROOM_LEAVE_FAIL = 1006, // 방 퇴장 실패
	ROOM_FULL = 1007, // 방이 꽉 참
	ROOM_NOT_FOUND = 1008, // 방을 찾을 수 없음
	ROOM_ALREADY_ENTERED = 1009, // 이미 방에 입장함
	ROOM_NOT_ENTERED = 1010, // 방에 입장하지 않음
	ROOM_PLAYING = 1011, // 방이 플레이 중임
	ROOM_NOT_PLAYING = 1012, // 방이 플레이 중이지 않음
};