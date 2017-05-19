using UnityEngine;
using System.Collections;

public class ShakeTest : MonoBehaviour
{
    public GameObject card;
    public float shakeAmount;
    public float shakeTime;
    public LeanTweenType tweenOption;

	void Update()
    {
        if(Input.GetMouseButtonDown(0))
        {
            LeanTween.pause(card);
            LeanTween.rotateY(card, shakeAmount, shakeTime).setEase(tweenOption).setEaseShake().setLoopCount(5);
            SoundManager.GetInstance().Play(EFFECT_TYPE.SummonFail);
        }
    }

    void Shake(float val)
    {
        shakeAmount = val;
    }
}