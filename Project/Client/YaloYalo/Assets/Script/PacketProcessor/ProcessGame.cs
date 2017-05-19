using UnityEngine;
using Packets;

//ProcessGame
// - GameScene에서 일어나는 일들을 처리한다

public partial class PacketProcessor : MonoBehaviour
{
    void YaloChangedNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_YALO_CHANGED_NTF>(jsonData);

        if(GameManager.playerSide == GameManager.GetPlayerSide(pkt._playerSide))
        {
            m_uiManager.YaloChanged(pkt._currentYalo);
        }
    }

    void UnitSummonNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_UNIT_SUMMON_NTF>(jsonData);

        m_unitManager.SummonUnit(pkt);
    }

    void UnitMoveToWaypointNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_UNIT_MOVE_TO_WAYPOINT_NTF>(jsonData);
        
        var unit = m_unitManager.FindUnit(pkt._unitNum);
        if (unit == null)
        {
            Debug.LogWarning("MoveToWay : unit not exists");
            return;
        }
        
        unit.OnMoveToWaypointEnter(
            new Vector2(pkt._moveValueX, pkt._moveValueY) * CalculateManager.toClientValue,
            new Vector2(pkt._wayPointX, pkt._wayPointY) * CalculateManager.toClientValue);
    }

    void UnitMoveToTargetNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_UNIT_MOVE_TO_TARGET_NTF>(jsonData);
        
        var unit = m_unitManager.FindUnit(pkt._unitNum);
        if(unit == null)
        {
            Debug.LogWarning("MoveToTarget : unit not exists");
            return;
        }

        var target = m_unitManager.FindUnit(pkt._targetNum);
        if(target == null)
        {
            Debug.LogWarning("MoveToTarget : target not exists");
            return;
        }

        unit.OnMoveToTargetEnter(target,
            pkt._speed * CalculateManager.toClientValue,
            new Vector2(pkt._currentPosX, pkt._currentPosY));
    }

    void UnitAttackNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_UNIT_ATTACK_NTF>(jsonData);
        
        Unit attackUnit = m_unitManager.FindUnit(pkt._unitNum);
        if (attackUnit == null)
        {
            Debug.LogWarning("AttackNtf : attack unit not exists");
            return;
        }

        Unit targetUnit = m_unitManager.FindUnit(pkt._targetNum);
        if (targetUnit == null)
        {
            Debug.LogWarning("AttackNtf : target unit not exists");
            return;
        }

        attackUnit.OnAttackEnter(targetUnit, pkt._attackDelay);
    }

    void UnitHpChangedNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_UNIT_HP_CHANGED>(jsonData);

        Unit targetUnit = m_unitManager.FindUnit(pkt._unitNum);
        if(targetUnit == null)
        {
            Debug.LogWarning("UnitHpChanged: cannot find unit");
            return;
        }

        targetUnit.ChangeHp(pkt._hp);
    }

    void UnitDieNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_UNIT_DIE_NTF>(jsonData);

        var res = m_unitManager.KillUnit(pkt._unitNum);
        if (res == false)
        {
            Debug.LogWarning("UnitDieNtf: fail kill unit");
        }
    }

    void UnitSpecialNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_UNIT_SPECIAL_NTF>(jsonData);

        var unit = m_unitManager.FindUnit(pkt._unitNum);
        if(unit == null)
        {
            Debug.LogWarning("UnitStateNtf: fail find unit");
            return;
        }

        unit.OnSpecialNtf(pkt);
    }

    void GameResultNtf(string jsonData)
    {
        var pkt = JsonUtility.FromJson<PACKET_GAME_RESULT_NTF>(jsonData);
        //이미 게임 종료되었으면 처리하지 않는다.
        if (GameManager.isGameOver == true)
        {
            return;
        }

        //게임 종료 표시
        GameManager.isGameOver = true;

        //유닛 업데이트 정지
        m_unitManager.enabled = false;

        //승패 연출
        EffectManager.DisplayGameResult(pkt._winPlayerSide);
    }
}