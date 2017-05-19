using UnityEngine;
using System.Collections;

public class HpTest : MonoBehaviour
{
    const float HP = 100.0f;
    public float nowHP = 100.0f;
    float BackHPPercent = 1.0f; //
    public GameObject HPBar;
    public GameObject HPBarBack;

    //TODO : 오브젝트의 Transform에 맞춰서 따라가기
    //  void Awake()
    //  {
    //      
    //  }

    public float tempTimer;

    // Update is called once per frame
    void Update()
    {
        //////////////////////////////
        ////HP가 감소하는 시뮬레이션
        //tempTimer += Time.deltaTime;
        //
        //if(tempTimer > 5.0f)
        //{
        //    tempTimer = 0.0f;
        //    nowHP -= 10.0f;
        //}
        //////////////////////////////

        if (nowHP > 0)
        {
            float percent = nowHP / HP;
            
            HPBarCtrl(percent);
            StartCoroutine(HPBarBackCtrl(percent));
        }
        else
        {
            HPBar.transform.localScale = new Vector3(0.0f, 1.0f, 1.0f);
            HPBarBack.transform.localScale = new Vector3(0.0f, 1.0f, 1.0f);
        }
    }

    void HPBarCtrl(float hpPercent)
    {
        HPBar.transform.localScale = new Vector3(hpPercent, 1.0f, 1.0f);
    }

    IEnumerator HPBarBackCtrl(float hpPercent)
    {
        yield return new WaitForSeconds(0.1f);

        while (BackHPPercent > hpPercent)
        {
            BackHPPercent -= 0.01f;
            HPBarBack.transform.localScale = new Vector3(BackHPPercent, 1.0f, 1.0f);
            yield return new WaitForSeconds(1.0f);
        }
    }
}
