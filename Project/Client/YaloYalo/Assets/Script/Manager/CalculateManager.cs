using UnityEngine;

//CalculateManager
//서버-클라 간 값 변환을 위한 변수를 관리합니다 

public class CalculateManager : MonoBehaviour
{
    //용도 : 게임좌표를 서버좌표로 변환할 때 곱해주는 값
    public static float toServerValue { get; set; }

    //용도 : 서버좌표를 게임좌표로 변환할 때 곱해주는 값
    public static float toClientValue { get; set; }

    //용도 : 게임좌표에서 스케일 값의 크기
    public const float scaleInGameWorld = 10.0f;

    //매치가 시작해줄 때 호출합니다.
    static public void OnMatchStart(Packets.PACKET_MATCH_START_NTF pkt)
    {
        toServerValue = pkt._mapSizeX / scaleInGameWorld;
        toClientValue = 1.0f / toServerValue;
    }
}
