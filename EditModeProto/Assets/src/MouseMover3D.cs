using UnityEngine;
using System.Collections;

///
/// Movement of orbiting 3d mouse cursor
/// 
public class MouseMover3D : PlaneMover
{
    public Orbiter m_cursorOrbiter;
    public Vector3 transformToCursorStart;
	// Use this for initialization
	void Start () 
    {
        // won't work if radius is changed during gameplay,
        // look into solution for this if it is needed
        transformToCursorStart = m_view.InverseTransformPoint(m_cursorOrbiter.transform.position);
	}
	
	// Update is called once per frame
	void Update () 
    {
        Vector3 transformToCursor = m_view.InverseTransformPoint(m_cursorOrbiter.transform.position) - transformToCursorStart;
        Debug.Log(transformToCursor);
	}
}
