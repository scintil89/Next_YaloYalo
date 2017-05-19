//Network에서 가져다 쓰는 상수값입니다.

static public class NetworkConstants
{
    static public string LOCAL_ADDRESS = "127.0.0.1";
    static public int SERVER_PORT = 34343;
    static public int PACKET_HEADER_SIZE = 8;
    static public int INT_SIZE = sizeof(int);
    static public int BUFFER_SIZE = 131072;
    static public System.Text.Encoding ENCODING = System.Text.Encoding.ASCII;
}