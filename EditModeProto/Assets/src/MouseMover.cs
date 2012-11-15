using UnityEngine;
using System.Collections;

// Plane movement using the mouse
// Distance attenuated from eye
public class MouseMover : PlaneMover
{
    public Transform m_eye;
	// Use this for initialization
	void Start () 
    {
	
	}
	
	// Update is called once per frame
	void Update () 
    {
        m_move.x = Input.GetAxis("Mouse X") * m_mod.x;
        m_move.y = Input.GetAxis("Mouse Y") * m_mod.y;
	}
}
