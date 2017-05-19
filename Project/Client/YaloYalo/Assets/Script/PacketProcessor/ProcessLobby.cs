using UnityEngine;
using System.Collections;
using Packets;
using UnityEngine.SceneManagement;

 //ProcessLobby
 // - LobbyScene에서 일어나는 일들을 처리한다

public partial class PacketProcessor : MonoBehaviour
{
    void MatchRes(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_MATCH_JOIN_RES>(jsonData);

        if (pkt._isSuccess == false)
        {
            FindObjectOfType<MatchButton>().OnMatchFailed();
        }
        else
        {
            GameManager.opponentName = pkt._opponentNickname;
        }
    }

    void MatchStartNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_MATCH_START_NTF>(jsonData);

        //주의: 초기화 순서 바꾸지 말 것
        CalculateManager.OnMatchStart(pkt);
        GameManager.OnMatchStart(pkt);
               
        //Scene Fade in/out Script by Scintil
        StartCoroutine("LoadGameScene");
    }

    IEnumerator LoadGameScene()
    {
        float fadeTime = FindObjectOfType<SceneChangeEffectScript>().BeginFade(1);

        yield return new WaitForSeconds(fadeTime);

        SceneManager.LoadScene(ResourceConstants.GAME_SCENE);
    }
}
