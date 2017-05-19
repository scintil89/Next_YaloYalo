namespace SamplePacket
{
	// 테스트용 rpg파일입니다.
	// 예외처리 하나도 안해뒀으니 문법에 맞게 잘 쓰시길
	// 주석은 "single integer _count //어쩌고" 식의 주석은 안됨
	// 패킷이름은 여기에
	public class Person
	{
	    // 고유 아이디
		public int					_index;
		public string				_name;
		public uint				_age;
	    // 배열도 됩니다
		public string[]			_interests;
	}

	// 여러 클래스를 한 파일에
	public class MessageList
	{
		public uint				NumOfMessages;
		public string[]			Messages;
	}

	// 제일 밑에는 패킷 아이디 들어감
	public enum PacketId
	{
		ID_Person			= 101,
		ID_MessageList			= 102,
	};
}