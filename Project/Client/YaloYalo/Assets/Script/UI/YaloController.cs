using UnityEngine;
using UnityEngine.UI;

public class YaloController : MonoBehaviour
{
    public Vector2 yaloBarPosition;
    public Text yaloNumberText;

    GameObject yaloDefaultBarObj;
    GameObject yaloCurrentBarObj;
    SpriteRenderer yaloCurrentBarSpr;

    float yaloBarScaleDelta;
    float maxScaleX;

    void Start()
    {
        yaloDefaultBarObj = Resources.Load<GameObject>(ResourceConstants.YALO_DEFAULT_BAR_PREFAB);
        yaloCurrentBarObj = Resources.Load<GameObject>(ResourceConstants.YALO_CHARGE_BAR_PREFAB);

        yaloDefaultBarObj = Instantiate(yaloDefaultBarObj);
        yaloCurrentBarObj = Instantiate(yaloCurrentBarObj);
        yaloCurrentBarSpr = yaloCurrentBarObj.GetComponent<SpriteRenderer>();

        yaloDefaultBarObj.transform.position = new Vector3(yaloBarPosition.x, 0.0f, yaloBarPosition.y);
        yaloCurrentBarObj.transform.position = new Vector3(yaloBarPosition.x, 0.1f, yaloBarPosition.y);

        yaloBarScaleDelta = 1.0f / (GameManager.yaloChargeTime * GameManager.maxYaloNumber); //1초에 얼마큼씩 커질 것인지

        yaloNumberText.text = GameManager.currentYaloNumber.ToString();

        yaloCurrentBarSpr.transform.localScale = 
            new Vector3(GameManager.currentYaloNumber / (float)GameManager.maxYaloNumber, 1.0f, 1.0f);

        maxScaleX = 1.0f;
    }

    void Update()
    {
        if (yaloCurrentBarSpr.transform.localScale.x <= maxScaleX)
        {
            yaloCurrentBarSpr.transform.localScale += new Vector3(
                yaloBarScaleDelta * Time.deltaTime, 0.0f, 0.0f);
        }
    }

    public void SetYalo(int nextYalo)
    {
        GameManager.currentYaloNumber = nextYalo;
        yaloNumberText.text = nextYalo.ToString();

        float scaleVal = GameManager.currentYaloNumber / (float)GameManager.maxYaloNumber;
        LeanTween.cancel(yaloCurrentBarObj);
        LeanTween.scaleX(yaloCurrentBarObj, scaleVal, GameManager.yaloChargeTime * 0.3f).setEase(LeanTweenType.easeOutBounce);
    }

    public void DecreaseYalo(int decreaseAmount)
    {
        GameManager.currentYaloNumber -= decreaseAmount;
        yaloNumberText.text = GameManager.currentYaloNumber.ToString();

        float scaleVal = GameManager.currentYaloNumber / (float)GameManager.maxYaloNumber;
        LeanTween.cancel(yaloCurrentBarObj);
        LeanTween.scaleX(yaloCurrentBarObj, scaleVal, GameManager.yaloChargeTime * 0.3f).setEase(LeanTweenType.easeInBounce);
    }
}
