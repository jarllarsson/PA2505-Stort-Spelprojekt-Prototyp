using UnityEngine;
using System.Collections;

public class FovEffect : MonoBehaviour {
    public Camera m_cam;
    public float m_multiplier=1.0f;

    private Vector3 m_oldPos;
    private float m_startFov;
	// Use this for initialization
	void Start () 
    {
        m_oldPos = transform.position;
        m_startFov = m_cam.fov;
	}
	
	// Update is called once per frame
	void Update () 
    {
        m_cam.fieldOfView = Mathf.Lerp(m_cam.fieldOfView,Mathf.Clamp(m_startFov * (1.0f+Vector3.SqrMagnitude(transform.position - m_oldPos) * m_multiplier),1.0f,170.0f),Time.deltaTime);
        m_oldPos = transform.position;
	}
}
