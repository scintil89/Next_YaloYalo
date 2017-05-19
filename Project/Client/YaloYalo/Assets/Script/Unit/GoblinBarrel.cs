using UnityEngine;
using Packets;
using System.Collections;

public class GoblinBarrel : Unit
{
    public GameObject model;
    public SkinnedMeshRenderer hatSmr;
    public Animator animator;
    public Material enemyMat;
    public Material myMat;
    public ParticleSystem flyingEffect;
    float kingPositionZ;
    float flyingTime;
    Vector3 flyingDesination;

    public override void OnSummoned(PACKET_UNIT_SUMMON_NTF pkt)
    {
        playerSide = GameManager.GetPlayerSide(pkt._playerSide);
        unitName = pkt._unitName;
        unitId = pkt._unitNum;
        hp = pkt._hp;

        //다른 편 유닛인 경우 = 위쪽에 있는 유닛인 경우
        if (playerSide != GameManager.playerSide)
        {
            transform.eulerAngles = new Vector3(
                transform.eulerAngles.x,
                transform.eulerAngles.y + 180.0f,
                transform.eulerAngles.z);
        }

        flyingDesination = new Vector3(
            pkt._posX * CalculateManager.toClientValue,
            0.0f,
            pkt._posY * CalculateManager.toClientValue);

        hpBar.OnSummoned(hp);
        hpBar.transform.position = LogicConstants.UNIT_DISAPPEAR_POS;
        hpBar.enabled = false;
        gameObject.transform.position = new Vector3(0.0f, -5.0f, 0.0f);
        model.SetActive(false);
        flyingEffect.Stop(true);

        hatSmr.material = (playerSide == GameManager.playerSide) ? myMat : enemyMat;
    }

    public override void OnSpecialNtf(PACKET_UNIT_SPECIAL_NTF pkt)
    {
        kingPositionZ = pkt._fval1 * CalculateManager.toClientValue;
        flyingTime = pkt._fval2;
        FlyToThePosition();
    }

    void FlyToThePosition()
    {
        SoundManager.GetInstance().Play(EFFECT_TYPE.FlyingStart);
        flyingEffect.Play(true);
        gameObject.transform.position = new Vector3(0.0f, 0.0f, kingPositionZ);

        LeanTween.move(gameObject, flyingDesination, flyingTime - 0.5f);

        StartCoroutine("OnFlyComplete", flyingTime - 0.5f);
        SoundManager.GetInstance().Play(EFFECT_TYPE.FlyingEnd, flyingTime);
    }

    IEnumerator OnFlyComplete(float flyingTime)
    {
        yield return new WaitForSeconds(flyingTime);

        flyingEffect.Stop(true);
        hpBar.enabled = true;
        model.SetActive(true);
        animator.SetTrigger(STAND_TRIGGER);
    }

    public override void OnMoveToWaypointEnter(Vector2 moveValue, Vector2 waypoint)
    {
        base.OnMoveToWaypointEnter(moveValue, waypoint);

        animator.SetTrigger(MOVE_TRIGGER);
    }

    public override void OnMoveToTargetEnter(Unit target, float moveSpeed, Vector2 serverPos)
    {
        base.OnMoveToTargetEnter(target, moveSpeed, serverPos);

        animator.SetTrigger(MOVE_TRIGGER);
    }

    public override void AttackOnce()
    {
        animator.SetTrigger(ATTACK_TRIGGER);

        SoundManager.GetInstance().Play(EFFECT_TYPE.SwordManAttack, attackDelay);
    }
}