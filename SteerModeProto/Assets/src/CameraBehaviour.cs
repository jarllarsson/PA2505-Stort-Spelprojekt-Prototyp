using UnityEngine;
using System.Collections;
using System.Threading;

public class CameraBehaviour : MonoBehaviour {
    public Transform m_target;
    public Transform m_origin;
    public float m_rotationSpd;
    public float m_moveSpd;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        transform.position = Vector3.Lerp(transform.position, m_origin.position, m_moveSpd * Time.deltaTime);
        // transform.LookAt(m_target, m_target.up);
        transform.rotation = Quaternion.Slerp(transform.rotation, m_origin.rotation, m_rotationSpd * Time.deltaTime);
        // Thread.Sleep(100);
	}


}
