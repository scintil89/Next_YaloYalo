using UnityEngine;
using System.Collections.Generic;

public class ResourceManager : ScriptableObject
{
    public static Dictionary<string, GameObject> unitPrefabDic;
    public static List<GameObject> cardPrefabList;

    static bool isLoaded = false;

    static ResourceManager()
    {
        unitPrefabDic = new Dictionary<string, GameObject>();
        cardPrefabList = new List<GameObject>();    
    }

    static public void LoadData()
    {
        if (isLoaded == true)
        {
            return;
        }

        Utility.Load("Prefab/Card", cardPrefabList);
        Utility.Load("Prefab/Unit/", unitPrefabDic);

        isLoaded = true;
    }
}