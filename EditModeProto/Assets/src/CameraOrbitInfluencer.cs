using UnityEngine;
using System.Collections;

///
/// Movement of orbiting 3d mouse cursor
/// 
public class CameraOrbitInfluencer : PlaneMoveInfluencer
{
    public float m_sphereRadius = 4;
    public float m_speed = 0.2f;
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
        float m3x = transformToCursor.x * transformToCursor.x * transformToCursor.x; // cubed to get a nice
        float m3y = transformToCursor.y * transformToCursor.y * transformToCursor.y; // falloff (uneven for correct sign)
        // Calc the movement amount
        float sphx = (1.0f - Mathf.Abs(Mathf.Sin(m3x)));
        float sphy = (1.0f - Mathf.Abs(Mathf.Sin(m3y)));
        m_move.x = m3x * sphx * sphx * m_speed * m_mod.x;
        m_move.y = m3y * sphy * sphy * m_speed * m_mod.y;
	}
}
