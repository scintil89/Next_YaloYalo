using UnityEngine;

public class UnitStateAttack : UnitState
{
    public override void Start(Unit unit)
    {
        Debug.LogFormat("unit[{0}({1})] starts attacking unit[{2}({3})]",
               unit.unitName, unit.unitId,
               unit.target.unitName, unit.target.unitId);
    }

    public override void Update(Unit unit)
    {
        if(unit.target == null)
        {
            Debug.Log("Attack: target is not available");
            return;
        }

        unit.AttackOnce();
        unit.ChangeState<UnitStateDoNothing>();
    }

    public override void End(Unit unit)
    {
    }
}