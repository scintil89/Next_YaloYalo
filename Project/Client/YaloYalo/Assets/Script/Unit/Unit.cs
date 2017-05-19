using UnityEngine;

public class Unit : MonoBehaviour
{
    public HPBar hpBar;

    public PlayerSide playerSide { get; set; }
    public ParticleManager particlePlayer { get; set; }

    public Unit target { get; set; }

    public int unitId { get; set; }
    public string unitName { get; set; }

    public float attackDelay { get; set; }
    public int hp { get; set; }

    public float moveSpeed { get; set; }   //방향은 적용되지 않은 값
    public Vector2 moveValue { get; set; } //방향이 적용된 값

    protected UnitState unitState;

    protected static string MOVE_TRIGGER = "Move";
    protected static string STAND_TRIGGER = "Stand";
    protected static string ATTACK_TRIGGER = "Attack";

    public void ManualInit()
    {
        if(hpBar) hpBar.ManualInit();
    }

    public virtual void ManualUpdate()
    {
        if (unitState == null)
        {
            return;
        }

        unitState.Update(this);
    }

    public virtual void OnSummoned(Packets.PACKET_UNIT_SUMMON_NTF pkt)
    {
        playerSide = GameManager.GetPlayerSide(pkt._playerSide);
        unitName = pkt._unitName;
        unitId = pkt._unitNum;
        hp = pkt._hp;

        //다른 편 유닛인 경우 = 위쪽에 있는 유닛인 경우
        if(playerSide != GameManager.playerSide)
        {
            transform.eulerAngles = new Vector3(
                transform.eulerAngles.x,
                transform.eulerAngles.y + 180.0f,
                transform.eulerAngles.z);
        }

        gameObject.transform.position = new Vector3(
            pkt._posX * CalculateManager.toClientValue,
            0.0f,
           pkt._posY * CalculateManager.toClientValue);

        //unit의 위치를 정하고나서 호출한다
        if (hpBar) hpBar.OnSummoned(hp);

        ChangeState<UnitStateSummoning>();
    }

    public virtual void OnMoveToWaypointEnter(Vector2 moveValue, Vector2 waypoint)
    {
        this.moveValue = moveValue;
        moveSpeed = moveValue.magnitude;

        Utility.Rotate(gameObject, new Vector3(waypoint.x, 0.0f, waypoint.y), 0.3f);
        ChangeState<UnitStateWalkAndSeek>();
    }

    public virtual void OnMoveToTargetEnter(Unit target, float moveSpeed, Vector2 serverPos)
    {
        this.target = target;
        this.moveSpeed = moveSpeed;

        gameObject.transform.position = new Vector3(
            serverPos.x * CalculateManager.toClientValue,
            0.0f,
            serverPos.y * CalculateManager.toClientValue);

        Utility.Rotate(gameObject, target.gameObject, 0.3f);
        ChangeState<UnitStateMove>();
    }

    public virtual void OnAttackEnter(Unit target, float attackDelay)
    {
        this.attackDelay = attackDelay;
        this.target = target;

        Utility.Rotate(gameObject, target.gameObject, 0.3f);
        ChangeState<UnitStateAttack>();
    }

    public virtual void MoveTowardWaypoint()
    {
        gameObject.transform.position +=
            new Vector3(moveValue.x, 0.0f, moveValue.y) * Time.deltaTime;
    }

    public virtual void MoveTowardTarget()
    {
        if (target == null || target.gameObject.activeSelf == false)
        {
            return;
        }

        var lookVector = target.gameObject.transform.position - gameObject.transform.position;
        var normalizedLookVector = lookVector.normalized;
        var moveDelta = new Vector3(
            normalizedLookVector.x * moveSpeed,
            0.0f,
            normalizedLookVector.z * moveSpeed);

        gameObject.transform.position += moveDelta * Time.deltaTime;
    }

    public virtual void ChangeHp(int chageHp)
    {
        hp = chageHp;
        if(hpBar) hpBar.OnChangeHP(hp);
    }

    public virtual void Kill()
    {
        particlePlayer.Play(PARTICLE_TYPE.UnitDead, transform.position);

        gameObject.transform.position = LogicConstants.UNIT_DISAPPEAR_POS;
        SoundManager.GetInstance().Play(EFFECT_TYPE.UnitDead);
        ChangeState<UnitStateDoNothing>();
    }

    public void ChangeState<STATE_TYPE>() where STATE_TYPE : UnitState, new()
    {
        if (unitState != null)
        {
            unitState.End(this);
        }

        unitState = new STATE_TYPE();
        unitState.Start(this);
    }

    public virtual void OnSpecialNtf(Packets.PACKET_UNIT_SPECIAL_NTF pkt)
    {
    }

    public virtual void AttackOnce()
    {
    }

    public virtual void SummonedEffect()
    {
    }
}