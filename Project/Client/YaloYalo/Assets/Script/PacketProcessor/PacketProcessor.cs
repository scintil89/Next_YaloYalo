using UnityEngine;
using UnityEngine.SceneManagement;
using System.Collections.Generic;
using Packets;

/* PacketProcessor
     - NetworkTcpIp로부터 Packet을 받아서 처리한다.
     - 처리는 ProcessLobby, ProcessGame 두 개의 파일에 구현한다.
     * 서버 종료/게임 시작: 하단에 구현
 */

public partial class PacketProcessor : MonoBehaviour
{
    delegate void PktProcessFunc(string jsonData);
    Dictionary<int, PktProcessFunc> m_pktFuncDictionary;

    UnitManager m_unitManager;
    UIManager m_uiManager;

    public void ProcessPacket(Packet packet)
    {
        if (m_pktFuncDictionary.ContainsKey(packet.pktId) == false)
        {
            return;
        }

        m_pktFuncDictionary[packet.pktId](packet.data);
    }

    void Start()
    {
        RegisterPacketFunction();
    }

    void RegisterPacketFunction()
    {
        m_pktFuncDictionary = new Dictionary<int, PktProcessFunc>();

        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_MATCH_JOIN_RES, MatchRes);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_MATCH_START_NTF, MatchStartNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_UNIT_SUMMON_NTF, UnitSummonNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_YALO_CHANGED_NTF, YaloChangedNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_UNIT_DIE_NTF, UnitDieNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_UNIT_MOVE_TO_TARGET_NTF, UnitMoveToTargetNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_UNIT_MOVE_TO_WAYPOINT_NTF, UnitMoveToWaypointNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_UNIT_ATTACK_NTF, UnitAttackNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_UNIT_HP_CHANGED, UnitHpChangedNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_GAME_RESULT_NTF, GameResultNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_CONNECT_CLOSE_NTF, ConnectCloseNtf);
        m_pktFuncDictionary.Add((int)PacketId.ID_PACKET_UNIT_SPECIAL_NTF, UnitSpecialNtf);
    }

    //게임 시작
    public void OnGameSceneEnter()
    {
        //Load Scripts
        var gameScripts = GameObject.Find(ResourceConstants.GAME_SCRIPTS);
        m_uiManager = gameScripts.GetComponent<UIManager>();
        m_unitManager = gameScripts.GetComponent<UnitManager>();

        //Set ValidSummonArea
        MapManager.OnGameSceneInit();

        //Set camera for top player
        if(GameManager.playerSide == PlayerSide.TOP)
        {
            var cam = Camera.main;
            cam.transform.Rotate(0, 0, 180);
            cam.transform.localPosition = new Vector3(0.0f, 10.0f, 1.0f);
        }
    }

    //서버 종료
    void ConnectCloseNtf(string dummy)
    {
        SceneManager.LoadScene(ResourceConstants.CONNECT_SCENE);

        //로직 정리
        if(m_unitManager != null)
        {
            Destroy(m_unitManager);
        }
        if(m_uiManager != null)
        {
            Destroy(m_uiManager);
        }

        NetworkTcpIp.GetInstance().CloseConnection();

        SoundManager.GetInstance().StopMusic();
    }
}