using UnityEngine;
using System.Collections;

public class HPBar : MonoBehaviour
{
    public GameObject redBar; //현재체력
    public GameObject backBar; //데미지 연출용
    public GameObject maxHPBar; //최대체력
    public Unit unit;

    static Vector3 barScale = new Vector3(0.65f, 1.0f, 1.0f);

    public Vector3 barPosDelta = new Vector3(-0.33f, 1.0f, -0.68f);
    public Vector3 barPosDeltaOnUnitReverted = new Vector3(-0.32f, 1.0f, -0.29f);
    Vector3 barPosDeltaDefault; //Cache for barPosDelta

    float backHPPercent = 1.0f;
    int maxHP;
    Quaternion fixedRotation;
    
    public void ManualInit() //PoolManager에서 한번만 호출된다
    {
        barPosDeltaDefault = barPosDelta;

        var unitTransform = unit.transform;
        var unitRot = unitTransform.localEulerAngles;
        var unitScale = unitTransform.localScale;

        //부모의 스케일에 관계없이 일정한 크기를 갖게함
        transform.localScale = new Vector3(barScale.x / unitScale.x, barScale.y / unitScale.y, 1.0f);
        //항상 위를 바라보도록 조정
        transform.localEulerAngles = new Vector3(90.0f - unitRot.x, -unitRot.y, -unitRot.z);
        fixedRotation = transform.rotation;

        name = unit.name + " HPBar";
    }

    void LateUpdate()
    {
        transform.position = unit.transform.position + barPosDelta;
        transform.rotation = fixedRotation;
    }

    public void OnSummoned(int maxHP)
    {
        barPosDelta = (GameManager.playerSide != unit.playerSide) ? barPosDeltaOnUnitReverted : barPosDeltaDefault;
        
        //유닛의 머리 위에다가 위치시킨다
        transform.position = unit.transform.position + barPosDelta;

        this.maxHP = maxHP;

        backHPPercent = 1.0f;

        redBar.transform.localScale = Vector3.one;
        backBar.transform.localScale = Vector3.one;
        maxHPBar.transform.localScale = Vector3.one;
    }

    public void OnChangeHP(int currentHP)
    {
        if (currentHP > 0)
        {
            float percent = currentHP / (float)maxHP;

            //클라에서 보여주는것이 서버보다 조금 느림
            StartCoroutine(RedBarCtrl(percent));
            StartCoroutine(BackBarCtrl(percent));
        }
        else
        {
            maxHPBar.transform.localScale = new Vector3(0.0f, 1.0f, 1.0f);
            redBar.transform.localScale = new Vector3(0.0f, 1.0f, 1.0f);
            backBar.transform.localScale = new Vector3(0.0f, 1.0f, 1.0f);
        }
    }

    IEnumerator RedBarCtrl(float hpPercent)
    {
        yield return new WaitForSeconds(0.2f);

        redBar.transform.localScale = new Vector3(hpPercent, 1.0f, 1.0f);
    }

    IEnumerator BackBarCtrl(float hpPercent)
    {
        while (backHPPercent > hpPercent)
        {
            backHPPercent -= 0.01f;
            backBar.transform.localScale = new Vector3(backHPPercent, 1.0f, 1.0f);

            yield return new WaitForSeconds(0.05f);
        }
    }
}