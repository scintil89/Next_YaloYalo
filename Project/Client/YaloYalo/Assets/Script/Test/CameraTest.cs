using UnityEngine;
using System.Collections;

public class CameraTest : MonoBehaviour
{
    public Camera mc;

    void Start()
    {
        LeanTween.value(mc.gameObject, cb, 0.3f, 9.0f, 0.5f).setEase(LeanTweenType.easeOutBack);
    }

    void Update()
    {
        if(Input.GetKey(KeyCode.A))
        {
            //이걸로!
            LeanTween.value(mc.gameObject, cb, 0.3f, 9.0f, 0.5f).setEase(LeanTweenType.easeOutSine);
        }
        if (Input.GetKey(KeyCode.B))
        {
            LeanTween.value(mc.gameObject, cb, 0.3f, 9.0f, 0.5f).setEase(LeanTweenType.easeSpring);
        }
    }
    void cb(float val, float ratio)
    {
        mc.orthographicSize = val * ratio;
    }
}