using UnityEngine;
using System.Collections;

public class Orbiter : MonoBehaviour {
    public Transform m_target;
    public float m_radius;
    public float m_speed = 0.02f;

    ///
	/// Use this for initialization
    /// 
	void Start () {

	}
	
	/// 
	/// Update is called once per frame
	/// 
	void Update () {
        // Get mouse pos delta, isolate this later
        float mx = Input.GetAxis("Mouse X") * m_speed;
        float my = -Input.GetAxis("Mouse Y") * m_speed;

        // transform.Rotate(m_rotation);
        transform.rotation *= Quaternion.Euler(my,mx,0.0f);
        transform.position = transform.forward*m_radius*-1;
        

	}
}
