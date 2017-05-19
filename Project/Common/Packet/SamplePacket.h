#pragma once

#include<string>
#include<vector>

namespace SamplePacket
{
	// 테스트용 rpg파일입니다.
	// 예외처리 하나도 안해뒀으니 문법에 맞게 잘 쓰시길
	// 주석은 "single integer _count //어쩌고" 식의 주석은 안됨
	// 패킷이름은 여기에
	struct Person
	{
	    // 고유 아이디
		int							_index;
		std::string					_name;
		unsigned					_age;
	    // 배열도 됩니다
		std::vector<std::string>	_interests;
	};

	// 여러 클래스를 한 파일에
	struct MessageList
	{
		unsigned					NumOfMessages;
		std::vector<std::string>	Messages;
	};

	// 제일 밑에는 패킷 아이디 들어감
	enum PacketId
	{
		ID_Person			= 101,
		ID_MessageList			= 102,
	};
}