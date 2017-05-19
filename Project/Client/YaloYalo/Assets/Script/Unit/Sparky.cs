using UnityEngine;
using Packets;
using System.Collections;

public class Sparky : Unit
{
    public SkinnedMeshRenderer hatSmr;
    public Animator animator;
    public Material enemyMat;
    public Material myMat;
    ParticleSystem chargeEffect;

    float chargeElapsedTime = 0.0f;
    float chargeLevelUpTime;
    float chargeCompleteTime;
    const int chargeLevelNum = 3;
    bool startCharge = false;

    enum SparkyState
    {
        NONE, LEVEL_ONE, LEVEL_TWO, LEVEL_THREE, COMPLETE
    }
    SparkyState sparkyState = SparkyState.NONE;

    public override void ManualUpdate()
    {
        chargeEffect.transform.position = transform.position;

        bool isChargeComplete = (sparkyState == SparkyState.COMPLETE) ? true : false;
        if (startCharge && !isChargeComplete)
        {
            Charge();
        }

        base.ManualUpdate();
    }

    public override void Kill()
    {
        //주의: 순서 바꾸지 말 것
        base.Kill();
        chargeEffect.transform.position = transform.position;
    }

    public override void OnSummoned(PACKET_UNIT_SUMMON_NTF pkt)
    {
        base.OnSummoned(pkt);
        if (chargeEffect == null) chargeEffect = particlePlayer.GetParticle(PARTICLE_TYPE.SparkyCharge);
        hatSmr.material = (playerSide == GameManager.playerSide) ? myMat : enemyMat;
    }

    public override void SummonedEffect()
    {
        particlePlayer.Play(PARTICLE_TYPE.Summon, gameObject.transform.position);
        SoundManager.GetInstance().Play(EFFECT_TYPE.UnitSummoned);
    }

    public override void OnSpecialNtf(PACKET_UNIT_SPECIAL_NTF pkt)
    {
        switch (pkt._ival)
        {
            case 0:
                chargeCompleteTime = pkt._fval1;
                chargeLevelUpTime = chargeCompleteTime / chargeLevelNum;
                sparkyState = SparkyState.LEVEL_ONE;
                startCharge = true;
                break;
            case 1:
                animator.SetTrigger(STAND_TRIGGER);
                ChangeState<UnitStateDoNothing>();
                break;
        }
    }

    public override void OnAttackEnter(Unit target, float attackDelay)
    {
        base.OnAttackEnter(target, attackDelay);

        StartCoroutine("ChangeToLevelOne", attackDelay);
    }
    
    IEnumerator ChangeToLevelOne(float delay)
    {
        yield return new WaitForSeconds(delay);

        sparkyState = SparkyState.LEVEL_ONE;
    }

    public override void AttackOnce()
    {
        StartCoroutine("AttackEffect");
    }

    IEnumerator AttackEffect()
    {
        yield return new WaitForSeconds(attackDelay - 0.55f);

        SoundManager.GetInstance().Play(EFFECT_TYPE.SparkyAttack);
        particlePlayer.Play(PARTICLE_TYPE.SparkyAttack, target.transform.position);
        animator.SetTrigger(ATTACK_TRIGGER);
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

    void Charge()
    {
        chargeElapsedTime += Time.deltaTime;
        if(chargeElapsedTime >= chargeLevelUpTime)
        {
            chargeElapsedTime = 0.0f;
            LevelUpState();
        }
    }

    void LevelUpState()
    {
        switch (sparkyState)
        {
            case SparkyState.LEVEL_ONE:
                sparkyState = SparkyState.LEVEL_TWO;
                SoundManager.GetInstance().Play(EFFECT_TYPE.SparkyCharge, 0.0f, 1.0f);
                particlePlayer.Play(PARTICLE_TYPE.SparkyLevelUp, transform.position);
                break;
            case SparkyState.LEVEL_TWO:
                sparkyState = SparkyState.LEVEL_THREE;
                SoundManager.GetInstance().Play(EFFECT_TYPE.SparkyCharge, 0.0f, 1.5f);
                particlePlayer.Play(PARTICLE_TYPE.SparkyLevelUp, transform.position);
                break;
            case SparkyState.LEVEL_THREE:
                sparkyState = SparkyState.COMPLETE;
                SoundManager.GetInstance().Play(EFFECT_TYPE.SparkyCharge, 0.0f, 3.0f);
                particlePlayer.Play(PARTICLE_TYPE.ChargeComplete, transform.position);
                particlePlayer.Play(PARTICLE_TYPE.SparkyLevelUp, transform.position);
                break;
        }
    }
}