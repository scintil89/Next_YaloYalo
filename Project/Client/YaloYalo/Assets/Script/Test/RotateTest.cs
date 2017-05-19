using UnityEngine;
using System.Collections;

public class RotateTest : MonoBehaviour
{
    public GameObject w;
    public GameObject u1;
    public GameObject u2;
    Unit us1;

    void Start()
    {
        us1 = u1.GetComponent<Unit>();
    }

    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Z))
        {
            us1.transform.eulerAngles = new Vector3(
                us1.transform.eulerAngles.x,
                us1.transform.eulerAngles.y + 180.0f,
                us1.transform.eulerAngles.z);
            us1.transform.Rotate(Vector3.up, 179.0f);
        }
        if(Input.GetKeyDown(KeyCode.A))
        {
            MoveTowardWaypoint();
        }
        if(Input.GetKeyDown(KeyCode.S))
        {
            MoveToUnit();
        }
        if(Input.GetKeyDown(KeyCode.D))
        {
            AttackUnit();
        }
        if (Input.GetKeyDown(KeyCode.F))
        {
            u1.transform.position = new Vector3(
                u1.transform.position.x + 1,
                u1.transform.position.y,
                u1.transform.position.z + 1
                );
        }
    }

    void MoveTowardWaypoint()
    {
        LeanTween.pause(u1);
        var originalRotation = u1.transform.eulerAngles;
        u1.transform.LookAt(w.transform);
        var rotateVal = u1.transform.eulerAngles;
        u1.transform.eulerAngles = originalRotation;
        LeanTween.rotate(u1, rotateVal, 0.3f);
    }

    void MoveToUnit()
    {
        LeanTween.pause(u1);
        var originalRotation = u1.transform.eulerAngles;
        u1.transform.LookAt(u2.transform);
        var rotateVal = u1.transform.eulerAngles;
        u1.transform.eulerAngles = originalRotation;
        LeanTween.rotateLocal(u1, rotateVal, 0.3f);
    }

    void AttackUnit()
    {
        LeanTween.pause(u1);
        var originalRotation = u1.transform.eulerAngles;
        u1.transform.LookAt(u2.transform);
        var rotateVal = u1.transform.eulerAngles;
        u1.transform.eulerAngles = originalRotation;
        LeanTween.rotateLocal(u1, rotateVal, 0.3f);
    }
}