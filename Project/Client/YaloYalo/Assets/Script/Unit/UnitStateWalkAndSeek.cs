using UnityEngine;

public class UnitStateWalkAndSeek : UnitState
{
    public override void Start(Unit unit)
    {
        Debug.LogFormat("unit [{0}({1})] starts seeking and walking", unit.unitName, unit.unitId);
    }

    public override void Update(Unit unit)
    {
        unit.MoveTowardWaypoint();
    }

    public override void End(Unit unit)
    {
    }
}