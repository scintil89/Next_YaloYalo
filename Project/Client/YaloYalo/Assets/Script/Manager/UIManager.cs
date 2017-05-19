using UnityEngine;

/* UI Manager
  PacketProcessor가 UI들의 메서드를 호출할 수 있도록 
  인터페이스를 제공한다 
*/
public class UIManager : MonoBehaviour
{
    public YaloController m_yaloController; //인스펙터를 통해 등록한다

    public void YaloChanged(int number)
    {
        m_yaloController.SetYalo(number);
    }
}

