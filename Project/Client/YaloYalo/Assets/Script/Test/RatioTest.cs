using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class RatioTest : MonoBehaviour
{
    public Camera mainCamera;
    public Text whText;
    float cameraWidth;
    float cameraHeight;

    void Start()
    {
        cameraWidth = mainCamera.pixelWidth;
        cameraHeight = mainCamera.pixelHeight;
    }

	void Update()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            cameraWidth += 0.05f;
        }
        if(Input.GetKeyDown(KeyCode.D))
        {
            cameraHeight += 0.05f;
        }

        mainCamera.pixelRect = new Rect(0.0f, 0.0f, cameraWidth, cameraHeight);

        whText.text = Screen.width + ", " + Screen.height + "C" + mainCamera.pixelWidth + "," + mainCamera.pixelHeight;
    }
}