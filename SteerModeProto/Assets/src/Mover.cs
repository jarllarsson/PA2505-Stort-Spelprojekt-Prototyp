using UnityEngine;
using System.Collections;

public class Mover : MonoBehaviour {
    public Vector3 m_speed=Vector3.zero;
    public float m_speedModifier = 300.0f;
    public float m_dieTick = 100.0f;
	// Use this for initialization
	void Start () {
       
	}
	
	// Update is called once per frame
	void Update () {
        transform.position += m_speed * Time.deltaTime;
        if (m_dieTick > 0.0f)
            m_dieTick -= Time.deltaTime;
        else
            Destroy(this.gameObject);
        Debug.DrawLine(transform.position, transform.position + m_speed * 10.0f);
	}

    public void AddSpd(Vector3 vec)
    {
        m_speed = transform.forward * m_speedModifier + vec;
        Debug.Log("! "+m_speed);
    }
}
