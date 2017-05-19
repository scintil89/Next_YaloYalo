using System.Collections.Generic;
using UnityEngine;

//UnitManager  
//  - Unit을 검색, 삭제, 추가한다.
//  - Unit의 Update를 호출한다

public class UnitManager : MonoBehaviour
{
    UnitPoolManager m_poolManager;
    ParticleManager m_particleManager;
    SortedDictionary<int, Unit> m_units = new SortedDictionary<int, Unit>();

    void Start()
    {
        m_poolManager = gameObject.AddComponent<UnitPoolManager>();
        m_particleManager = gameObject.AddComponent<ParticleManager>();
    }

    public void Update()
    {
        var iter = m_units.GetEnumerator();

        while (iter.MoveNext())
        {
            var unit = iter.Current.Value;
            if(unit.gameObject.activeSelf == false)
            {
                continue;
            }

            unit.ManualUpdate();
        }
    }

    public Unit FindUnit(int unitId)
    {
        if (m_units.ContainsKey(unitId) == true &&
            m_units[unitId].gameObject.activeSelf == true)
        {
            return m_units[unitId];
        }
        else
        {
            return null;
        }
    }

    public bool KillUnit(int unitId)
    {
        if(m_units.ContainsKey(unitId) == true &&
            m_units[unitId].gameObject.activeSelf == true)
        {
            m_units[unitId].Kill();
            m_units.Remove(unitId);
            return true;
        }
        else
        {
            return false;
        }
    }

    public void SummonUnit(Packets.PACKET_UNIT_SUMMON_NTF pkt)
    {
        if(m_units.ContainsKey(pkt._unitNum) == true)
        {
            Debug.Log("Unit number is overlapped");
            return;
        }

        var unit = m_poolManager.GetUnit(pkt._unitName);
        if(unit == null)
        {
            Debug.Log("PoolManager create unit failed");
            return;
        }

        unit.particlePlayer = m_particleManager;
        unit.OnSummoned(pkt);
        
        m_units.Add(pkt._unitNum, unit);
    }
}