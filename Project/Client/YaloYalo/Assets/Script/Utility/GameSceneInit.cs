using UnityEngine;

public class GameSceneInit : MonoBehaviour
{
    //Game Scene 에 진입했을 때 호출되는 함수입니다
    //Game Scene 과 관련된 초기화는 이곳에서 하시면 됩니다
    void Awake()
    {
        FindObjectOfType<PacketProcessor>().OnGameSceneEnter();

        EffectManager.DisplayPlayersName(GameManager.opponentName, GameManager.myName, GameManager.playerSide);

        var pkt = new Packets.PACKET_CLIENT_GAME_READY_REQ();
        pkt._gameReady = (GameManager.playerSide == PlayerSide.TOP ? true : false);
        
        NetworkTcpIp.GetInstance().SendPacket(pkt, Packets.PacketId.ID_PACKET_CLIENT_GAME_READY_REQ);
    }
}