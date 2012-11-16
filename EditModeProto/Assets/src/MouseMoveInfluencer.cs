using UnityEngine;
using System.Collections;

///
/// Plane movement using the mouse
/// Distance attenuated from eye
/// 
public class MouseMoveInfluencer : PlaneMoveInfluencer
{
    public bool m_useJoystick=true;
    public float m_mouseSpeed = 0.5f;
    public float m_joySpeed = 0.1f;
	// Use this for initialization
	void Start () 
    {
	
	}
	
	// Update is called once per frame
	void Update () 
    {
        Vector3 t = new Vector3(Input.GetAxis("Mouse X") * m_mod.x,
                                Input.GetAxis("Mouse Y") * m_mod.y,
                                0.0f) * m_mouseSpeed;
        if (m_useJoystick)
        {
            t += new Vector3(Input.GetAxis("Horizontal") * m_mod.x,
                             Input.GetAxis("Vertical") * m_mod.y,
                             0.0f) * m_joySpeed;
        }
        m_move.x = t.x;
        m_move.y = t.y;
	}
}
