using UnityEngine;

//GameManager
// - 게임 진행 데이터를 저장한다
// - 초기화는 Match가 시작될 때 해준다

public class GameManager : MonoBehaviour
{
    //용도 : 플레이어 닉네임
    public static string myName { get; set; }
    public static string opponentName { get; set; }

    //용도 : 클라이어트 편
    public static PlayerSide playerSide { get; set; }
    
    //용도 : 현재 얄로 개수
    public static int currentYaloNumber { get; set; }
    public static float yaloChargeTime { get; set; }
    public static int maxYaloNumber { get; set; }

    //용도 : 현재 게임이 종료되었는가
    public static bool isGameOver = false;

    static public PlayerSide GetPlayerSide(bool playerSide)
    {
        return playerSide ? PlayerSide.TOP : PlayerSide.BOTTOM;
    }

    static public void OnMatchStart(Packets.PACKET_MATCH_START_NTF pkt)
    {
        //Set playerSide
        playerSide = GetPlayerSide(pkt._playerSide);

        //Set yalo
        currentYaloNumber = pkt._yaloStartNum;
        yaloChargeTime = pkt._yaloChargeTime;
        maxYaloNumber = pkt._maxYaloNum;
    }
}

public enum PlayerSide
{
    BOTTOM, TOP
}