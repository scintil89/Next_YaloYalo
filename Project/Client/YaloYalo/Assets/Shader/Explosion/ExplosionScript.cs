using UnityEngine;
using System.Collections;

public class ExplosionScript : MonoBehaviour
{
    float boomScale = 30.0f;
    float unboomScale = -70.0f;
    float loopduration = 0.6f;

    float nowlife = 0.0f;
    const float lifeCycle = 1.2f;
    const float destroy = 2.0f;

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    Renderer render;

    void Start()
    {
        render = GetComponent<Renderer>();
        transform.localScale = new Vector3(0.1f, 0.1f, 0.1f);
    }

    // Update is called once per frame
    void Update ()
    {
        nowlife += Time.deltaTime;

        transform.localScale += new Vector3(0.01f * boomScale, 0.01f * boomScale, 0.01f * boomScale);

        //if (nowlife < lifeCycle)
        //{
            r = Mathf.Sin((Time.time / loopduration) * (2 * Mathf.PI)) * 0.5f + 0.25f;
            g = Mathf.Sin((Time.time / loopduration + 0.33333333f) * 2 * Mathf.PI) * 0.5f + 0.25f;
            b = Mathf.Sin((Time.time / loopduration + 0.66666667f) * 2 * Mathf.PI) * 0.5f + 0.25f;
        //SKA
        //}

        //if (nowlife > destroy)
        //{
        //    //NTSC 표준 명도 변환 비율
        //    float gray = (0.299f * r + 0.587f * g + 0.114f * b);
        //
        //    r = gray;
        //    g = gray;
        //    b = gray;
        //}

        float correction = 1 / (r + g + b);
        r *= correction;
        g *= correction;
        b *= correction;


        //Debug.Log(nowlife);

        if (nowlife > lifeCycle)
            boomScale = unboomScale;

        if (nowlife > destroy)
            gameObject.SetActive(false);

        render.material.SetVector("_ChannelFactor", new Vector4(r, g, b, 0));
    }
}
