/*
 클라이언트에서의 UnitState 구성
 -소환중(UnitStateSummoning)
 -탐색한다(UnitStateWalkAndSeek)
 -접근한다(UnitStateMove)
 -때린다(UnitStateAttack)
 -아무것도 안한다(UnitStateDoNothing)
*/

public abstract class UnitState
{
    public abstract void Start(Unit unit);
    public abstract void Update(Unit unit);
    public abstract void End(Unit unit);
}