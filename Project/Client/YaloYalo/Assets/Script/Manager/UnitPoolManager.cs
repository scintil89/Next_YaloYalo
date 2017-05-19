using System.Collections.Generic;
using UnityEngine;

/*
    PoolManager
     - 유닛의 이름을 받아서 해당 유닛의 인스턴스를 리턴한다
     - 내부적으로 자원을 재활용한다 
*/

public class UnitPoolManager : MonoBehaviour
{
    Dictionary<string, GameObject> unitPrefabDict;
    List<GameObject> unitPool = new List<GameObject>();

    void Start()
    {
        unitPrefabDict = ResourceManager.unitPrefabDic;
    }

    public Unit GetUnit(string unitName)
    {
        if(unitPrefabDict.ContainsKey(unitName))
        {
            return GetThis(unitName, unitPrefabDict[unitName]);
        }
        else
        {
            return null;
        }
    }

    Unit GetThis(string unitName, GameObject prefab)
    {
        //우선 풀에 해당 객체가 있는지 확인한다 
        for (int i = 0; i < unitPool.Count; i++)
        {
            bool isUsable = unitPool[i].gameObject.transform.position.y < LogicConstants.UNIT_POOL_USABLE_LINE;
            if ( isUsable && unitPool[i].name == unitName)
            {
                return unitPool[i].GetComponent<Unit>();
            }
        }
        
        //풀에 해당 객체가 없으면 새로 만들어서
        GameObject newUnitObj = Instantiate(prefab);
        //이름 세팅을 하고
        newUnitObj.name = unitName;
        //풀에 추가한다
        unitPool.Add(newUnitObj);
        //스크립트 초기화
        var unitScript = newUnitObj.GetComponent<Unit>();
        unitScript.ManualInit();

        return unitScript;
    }
}