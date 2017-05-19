using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class AddTextTest : MonoBehaviour
{
	void Start()
    {
        var textObj = GameObject.Find("PvPTextObject");
        var text1 = textObj.GetComponent<Text>();
        text1.text = "abc";
    }
}