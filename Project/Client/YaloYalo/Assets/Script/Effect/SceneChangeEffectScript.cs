using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

public class SceneChangeEffectScript : MonoBehaviour
{
    //씬 이동시 Fade in / out을 위한 스크립트입니다.
    //이 스크립트를 가지는 오브젝트는 씬이 이동되어도 파괴되면 안됩니다.

    public Texture2D fadeTexture; //페이드 인아웃을 위한 택스쳐입니다.
    public float fadeSpeed = 2.0f;
    public float fadeTime = 2.0f;

    int drawDepth = -1000;
    int fadeDir = -1;
    float alpha = 1.0f;

    // Use this for initialization
    void Awake()
    {
        SceneManager.sceneLoaded += OnLevelLoadedEffect;
        DontDestroyOnLoad(gameObject);
    }

    void OnGUI()
    {
        //TODO : 상시적으로 OnGUI를 계속 호출하고 있음. if문으로 감싸서 수정할 필요가 있을듯
        alpha += fadeDir * fadeSpeed * Time.deltaTime;

        alpha = Mathf.Clamp01(alpha); //0~1 사이의 값으로 변환

        GUI.color = new Color(GUI.color.r, GUI.color.g, GUI.color.b, alpha);
        GUI.depth = drawDepth;
        GUI.DrawTexture(new Rect(0, 0, Screen.width, Screen.height), fadeTexture);
    }

    public float BeginFade(int direction)
    {
        fadeDir = direction;
        return fadeTime;
    }

    public IEnumerator FadeWaitAfterClick()
    {
        float _fadeTime = BeginFade(1);
        Debug.Log(_fadeTime);
        yield return new WaitForSeconds(_fadeTime);
    }

    void OnLevelLoadedEffect(Scene scene, LoadSceneMode mode)
    {
        //TODO : 레벨 로딩 단계적으로 하기
        GameObject.Find("SceneChangeEffect").GetComponent<SceneChangeEffectScript>().BeginFade(-1);
    }

    //     public enum SceneState
    //     {
    //         NONE,
    //         FADEOUT,
    //         FADEIN
    //     }


    //public GUITexture fadeTexture;
    //Color textureColor;

    /*    float nowTime = 0.0f;*/
    /*    public SceneState sceneState;*/

    //camera = Find("Start Scene Camera");

    //fadeTexture = gameObject.GetComponent<GUITexture>();
    //textureColor = fadeTexture.color;

    /*        sceneState = SceneState.NONE;*/
    //     void OnLevelWasLoaded()
    //     {
    //         BeginFade(-1);
    //     }



    // Update is called once per frame
    // 	void Update ()
    //     {
    //	    switch (sceneState)
    //      {
    //             case SceneState.NONE:
    //                 {
    //                     return;
    //                 }
    // 
    //             case SceneState.FADEOUT:
    //                 {
    //                     if(nowTime > fadeOut)
    //                     {
    //                         EndEffect();
    //                         nowTime = 0.0f;
    //                         return;
    //                     }
    // 
    //                     fadeTexture.color = Color.Lerp(new Color(0, 0, 0, 0), Color.black, nowTime / fadeOut);
    // 
    //                     nowTime += Time.deltaTime;
    //                 }
    //                 break;
    // 
    //             case SceneState.FADEIN:
    //                 {
    //                     if(nowTime > fadeIn)
    //                     {
    //                         sceneState = SceneState.NONE;
    //                         nowTime = 0.0f;
    //                         return;
    //                     }
    // 
    //                     fadeTexture.color = Color.Lerp(Color.black, textureColor, nowTime / fadeIn);
    // 
    //                     nowTime += Time.deltaTime;
    //                 }
    //                 break;
    //         }
    //      }
    //     public void StartEffect()
    //     {
    //         sceneState = SceneState.FADEOUT;
    //     }
    //    void EndEffect()
    //     {
    //         sceneState = SceneState.FADEIN;
    //    }

}
