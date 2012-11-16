using UnityEngine;
using System.Collections;

///
/// Move and rotate a transform along the surface of
/// a sphere. Supports trackball- and turntable-type rotations.
/// 
public class Orbiter : MonoBehaviour 
{
    public enum ROTATEMODE
    {
        TRACKBALL,
        TURNTABLE
    };
    public Transform m_target; ///< Lookat target (center of sphere).
    public float m_radius; ///< Radius of sphere surface.
    public ROTATEMODE m_rotateMode = ROTATEMODE.TRACKBALL;

    public PlaneMoveInfluencer m_planeMoveInfluencer; ///< Movement influence.
    private Quaternion m_startRotation; ///< Initial rotation.
    private Quaternion m_rotation; ///< Current rotation. 

    private Vector2 m_movement; ///< Movement vector in 2d space.

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
        if (m_rotateMode == ROTATEMODE.TURNTABLE)
            m_movement += m_planeMoveInfluencer.m_move; // For turntable; accumulate movement (in 2d space)
        else if (m_rotateMode == ROTATEMODE.TRACKBALL)
            m_movement = m_planeMoveInfluencer.m_move; // For trackball; set relative movement (in 2d space)

        // Transform quaternion with movement vector
        m_rotation *= Quaternion.Euler(m_movement.y, m_movement.x, 0.0f);

        // Translate and rotate
        transform.position = m_target.position + m_rotation * Vector3.forward * -m_radius;
        transform.rotation = m_rotation;

        // Reset quaternion if turntable (move vector is accumulated instead)
        if (m_rotateMode == ROTATEMODE.TURNTABLE)
            m_rotation = m_startRotation;
	}
}
