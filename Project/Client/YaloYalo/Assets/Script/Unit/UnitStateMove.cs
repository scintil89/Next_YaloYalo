using UnityEngine;

public class UnitStateMove : UnitState
{
    public override void Start(Unit unit)
    {
        Debug.LogFormat("unit[{0}({1})] starts to move toward unit[{2}({3})]",
            unit.unitName, unit.unitId, 
            unit.target.unitName, unit.target.unitId);        
    }

    public override void Update(Unit unit)
    {
        if (unit.target == null)
        {
            return;
        }

        unit.MoveTowardTarget();
    }

    public override void End(Unit unit)
    {
        Debug.LogFormat("unit[{0}({1})] ends approaching", unit.unitName, unit.unitId);
    }
}