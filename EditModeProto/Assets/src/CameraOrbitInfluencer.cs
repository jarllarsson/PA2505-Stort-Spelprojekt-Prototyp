using UnityEngine;
using System.Collections;

///
/// Movement of orbiting 3d mouse cursor
/// 
public class CameraOrbitInfluencer : PlaneMoveInfluencer
{
    public float m_sphereRadius = 4;
    public float m_speed = 0.2f;
    public float m_borderStartRadSqr = 0.25f;
    public OrbitMouseMover m_cursorMover;
    public Vector3 transformToCursorStart;
	// Use this for initialization
	void Start () 
    {
        // won't work if radius is changed during gameplay,
        // look into solution for this if it is needed
        transformToCursorStart = m_view.InverseTransformPoint(m_cursorMover.transform.position);
	}
	
	// Update is called once per frame
	void Update () 
    {
        Vector3 transformToCursor = m_view.InverseTransformPoint(m_cursorMover.transform.position) - transformToCursorStart;
        transformToCursor /= m_sphereRadius; // -1.0 -> 1.0 range
        float sqrDist = transformToCursor.sqrMagnitude;
        if (sqrDist > m_borderStartRadSqr)
        {
            // Movement within border, with falloff
            float m3x = transformToCursor.x;
            float m3y = transformToCursor.y;
            float fallof = sqrDist - m_borderStartRadSqr;
            m_move.x = m3x * m_speed * fallof * m_mod.x;
            m_move.y = m3y * m_speed * fallof * m_mod.y;
        }
        else
            m_move = Vector2.zero;
	}
}
