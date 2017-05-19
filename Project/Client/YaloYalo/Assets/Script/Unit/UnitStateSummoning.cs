using UnityEngine;

public class UnitStateSummoning : UnitState
{
    public override void Start(Unit unit)
    {
        Debug.LogFormat("unit[{0}({1})] starts summoning", unit.unitName, unit.unitId);

        unit.SummonedEffect();
    }

    public override void Update(Unit unit)
    {
    }

    public override void End(Unit unit)
    {
    }
}