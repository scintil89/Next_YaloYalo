using UnityEngine;
using UnityEngine.UI;

//EffectManager
//게임 내 여러 연출을 관리합니다

public class EffectManager : MonoBehaviour
{
    //매치 시작 시, 플레이어들의 이름을 보여줍니다.
    //음악재생, 카메라연출 추가함
	static public void DisplayPlayersName(string oppentName, string myName, PlayerSide mySide)
    {
        var UpperNickName   = GameObject.Find("UpperNickName").GetComponent<Text>();
        var VS              = GameObject.Find("VS").GetComponent<Text>();
        var LowerNickName   = GameObject.Find("LowerNickName").GetComponent<Text>();
        var bgCamera        = GameObject.Find("Background Camera").GetComponent<Camera>();

        if (mySide == PlayerSide.TOP)
        {
            UpperNickName.text = myName;
            LowerNickName.text = oppentName;
        }
        else
        {
            UpperNickName.text = oppentName;
            LowerNickName.text = myName;
        }

        //Upper Nick Name Move        
        LeanTween.moveLocal(UpperNickName.gameObject, new Vector2(-120.0f, 45.0f), 1.5f).setEase(LeanTweenType.easeInCubic);
        LeanTween.moveLocal(UpperNickName.gameObject, new Vector2(1000.0f, 45.0f), 1.5f).setEase(LeanTweenType.easeInOutBack).setDelay(4.5f);
        
        //Lower Nick Name Move
        LeanTween.moveLocal(LowerNickName.gameObject, new Vector2(120.0f, -45.0f), 1.5f).setEase(LeanTweenType.easeInCubic);
        LeanTween.moveLocal(LowerNickName.gameObject, new Vector2(-1000.0f, -45.0f), 1.5f).setEase(LeanTweenType.easeInOutBack).setDelay(4.5f);

        //VS Move 
        VS.gameObject.SetActive(true); //씬에서 디폴트로 꺼져있음
        LeanTween.moveLocal(VS.gameObject, new Vector2(0.0f, 1000.0f), 0.5f).setDelay(4.0f);

        //Camera Move
        System.Action<float, float> cameraMove = (float val, float ratio) => { bgCamera.orthographicSize = val * ratio; };
        LeanTween.value(bgCamera.gameObject, cameraMove, 0.3f, 9.0f, 0.6f).setEase(LeanTweenType.easeOutSine);

        //Play VS SFX
        SoundManager.GetInstance().Play(EFFECT_TYPE.VS, 1.4f);

        //Play music with delay
        SoundManager.GetInstance().Play(MUSIC_TYPE.GameScene, 4.25f);
    }

    //매치 종료 시, 게임 결과를 보여줍니다.
    //음악재생 추가함
    static public void DisplayGameResult(bool winnerSide)
    {
        SoundManager.GetInstance().StopMusic();

        //돌아가기 버튼 위치시키기
        GameObject.Find("GoToLobby").transform.localPosition = new Vector3(13.6f, -82.8f);

        //VS를 표시하는데 사용했던 텍스트 재활용
        var textObj = GameObject.Find("VS").GetComponent<Text>();

        //승리함
        if(GameManager.playerSide == GameManager.GetPlayerSide(winnerSide))
        {
            SoundManager.GetInstance().Play(EFFECT_TYPE.Win);
            textObj.text = "You Win";
        }
        //패배함
        else
        {
            SoundManager.GetInstance().Play(EFFECT_TYPE.Lose);
            textObj.text = "You Lose";
        }

        textObj.gameObject.transform.localPosition = new Vector2(0.0f, 0.0f);
    }
}