using UnityEngine;
using System.Collections;

///
/// Plane movement using the mouse
/// Distance attenuated from eye
/// 
public class MouseMoveInfluencer : PlaneMoveInfluencer
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
        m_move.x = t.x;
        m_move.y = t.y;
	}
}
