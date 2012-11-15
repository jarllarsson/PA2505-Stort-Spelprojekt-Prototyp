using UnityEngine;
using System.Collections;

///
/// Plane movement using the mouse
/// Distance attenuated from eye
/// 
public class MouseMover : PlaneMover
{
	// Use this for initialization
	void Start () 
    {
	
	}
	
	// Update is called once per frame
	void Update () 
    {
        Vector3 t = new Vector3(Input.GetAxis("Mouse X") * m_mod.x,
                                Input.GetAxis("Mouse Y") * m_mod.y,
                                0.0f);
        t = m_view.InverseTransformDirection(t); // transform movement by eye matrix
        m_move.x = t.x;
        m_move.y = t.y;
	}
}
