using UnityEngine;
using System.Collections;

public class CameraTest2 : MonoBehaviour
{
    public Camera main;
    public Camera ui;
    public Camera bg;

    void Update()
    {  
        if(Input.GetMouseButtonUp(0))
        {
            //Debug.Log("==================");

            //var mousePos = Input.mousePosition;
            //Debug.Log("Mouse: " + mousePos);

            //var mainPos = main.ScreenToWorldPoint(mousePos);
            //Debug.Log("Main: " + mainPos);

            //var uiPos = ui.ScreenToWorldPoint(mousePos);
            //Debug.Log("UI: " + uiPos);
        }
    }
}