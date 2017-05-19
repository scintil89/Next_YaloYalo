using UnityEditor;
using UnityEngine;

[CustomEditor(typeof(UnitAdd))]
public class UnitAddEditor : Editor
{
    UnitAdd ua;
    GameObject unitPrefab;
    GameObject prevPrefab;
    GameObject cardPrefab;

    void OnSceneGUI()
    {
        ua = (UnitAdd)target;
       
        Handles.BeginGUI();

        if (GUI.Button(new Rect(10, 10, 100, 50), "Make"))
        {
            if(ua.unitPrefab == null ||
                ua.unitPrevSprite == null ||
                ua.cardSprite == null ||
                ua.unitName.Length == 0 ||
                ua.yaloCost <= 0 || ua.yaloCost > 5)
            {
                EditorUtility.DisplayDialog("Fill all inspector items", "If you want to know conditions of item, open \"UnitAdd.cs\" and read comments", "ok");
            }
            else
            {
                string prefabDir = "Assets/Resources/Prefab/";

                prevPrefab = CreatePrevPrefab();
                PrefabUtility.CreatePrefab(prefabDir + "Prev/" + prevPrefab.name + ".prefab", prevPrefab);

                cardPrefab = CreateCardPrefab();
                PrefabUtility.CreatePrefab(prefabDir + "Card/" + cardPrefab.name + ".prefab", cardPrefab);

                unitPrefab = CreateUnitPrefab();
                PrefabUtility.CreatePrefab(prefabDir + "Unit/" + unitPrefab.name + ".prefab", unitPrefab);

                AssetDatabase.Refresh(ImportAssetOptions.ForceUpdate);

                EditorUtility.DisplayDialog("Done", "Check if result is good", "thank you");
            }
        }

        if(GUI.Button(new Rect(10, 60, 100, 50), "Clear scene"))
        {
            if(unitPrefab) DestroyImmediate(unitPrefab, false);
            if(cardPrefab) DestroyImmediate(cardPrefab, false);
            if(prevPrefab) DestroyImmediate(prevPrefab, false);

            EditorUtility.DisplayDialog("Done", "Scene is now clear", "ok");
        }

        Handles.EndGUI();
    }

    GameObject CreateCardPrefab()
    {
        GameObject cardPrefab = new GameObject();
        cardPrefab.name = ua.unitName;
        cardPrefab.layer = 5; //5 = UI

        cardPrefab.transform.position = 
            new Vector3(0.0f, 1.0f, 0.0f);
        cardPrefab.transform.eulerAngles = 
            new Vector3(90.0f, 0.0f, 0.0f);
        cardPrefab.transform.localScale = 
            new Vector3(0.25f, 0.25f, 1.0f);

        var sprRenderer = cardPrefab.AddComponent<SpriteRenderer>();
        sprRenderer.sprite = ua.cardSprite;
        sprRenderer.sortingOrder = 1;

        var cc = cardPrefab.AddComponent<CardController>();
        cc.m_unitPrev = Resources.Load<GameObject>("Prefab/Prev/" + ua.unitName);
        cc.m_unitName = ua.unitName;
        cc.m_cardSprite = sprRenderer;
        cc.m_isMagic = ua.isMagic;
        cc.m_yaloCost = ua.yaloCost;

        cardPrefab.AddComponent<BoxCollider>();

        string numStr = ua.yaloCost.ToString();
        var numObj = Instantiate(Resources.Load<GameObject>("Prefab/Number/" + numStr));
        numObj.name = numStr;
        numObj.layer = 5; //5 = layer
        numObj.GetComponent<SpriteRenderer>().sortingOrder = 2;
        numObj.transform.SetParent(cardPrefab.transform);

        cc.m_numberSprite = numObj.GetComponent<SpriteRenderer>();

        return cardPrefab;
    }

    GameObject CreatePrevPrefab()
    {
        GameObject prevPrefab = new GameObject();
        prevPrefab.name = ua.unitName;
        prevPrefab.layer = 5; //5 = layer

        prevPrefab.transform.position =
            new Vector3(0.0f, 1.0f, 0.0f);
        prevPrefab.transform.eulerAngles =
            new Vector3(90.0f, 0.0f, 0.0f);
        prevPrefab.transform.localScale =
            new Vector3(0.7f, 0.7f, 1.0f);

        var sr = prevPrefab.AddComponent<SpriteRenderer>();
        sr.sprite = ua.unitPrevSprite;

        return prevPrefab;
    }

    GameObject CreateUnitPrefab()
    {
        GameObject unitPrefab = Instantiate(ua.unitPrefab);
        unitPrefab.name = ua.unitName;

        return unitPrefab;
    }
}