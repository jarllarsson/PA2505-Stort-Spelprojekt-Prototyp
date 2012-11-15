using UnityEngine;
using System.Collections;

public class Orbiter : MonoBehaviour 
{
    public enum ROTATEMODE
    {
        TRACKBALL,
        TURNTABLE
    };
    public Transform m_target;
    public float m_radius;
    public float m_speed = 0.02f;
    public ROTATEMODE m_rotateMode = ROTATEMODE.TRACKBALL;

    public PlaneMover m_planeMover;
    private Quaternion m_startRotation;
    private Quaternion m_rotation;

    private Vector2 m_movement;

    ///
	/// Use this for initialization
    /// 
	void Start() 
    {
        m_startRotation = transform.rotation;
        m_rotation = m_startRotation;
	}
	
	/// 
	/// Update is called once per frame
	/// 
	void Update () 
    {
        // Accumulate movement (in plane space)
        if (m_rotateMode == ROTATEMODE.TURNTABLE)
            m_movement += m_planeMover.m_move * m_speed;
        else if (m_rotateMode == ROTATEMODE.TRACKBALL)
            m_movement = m_planeMover.m_move * m_speed;

        // Transform quaternion with movement vector
        m_rotation *= Quaternion.Euler(m_movement.y, m_movement.x, 0.0f);

        // Translate and rotate
        transform.position = m_rotation * Vector3.forward * -m_radius;
        transform.rotation = m_rotation;

        // Reset quaternion if turntable
        if (m_rotateMode == ROTATEMODE.TURNTABLE)
            m_rotation = m_startRotation;
	}
}
