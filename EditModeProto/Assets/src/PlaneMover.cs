using UnityEngine;
using System.Collections;
///
/// Base class for plane movement
/// 
public class PlaneMover : MonoBehaviour 
{
    public Transform m_view;
    public Vector2 m_move; ///< Plane movement
    public Vector2 m_mod = Vector2.one; ///< Modifier
	// Use this for initialization
	void Start () {
        if (m_view == null) m_view = transform;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
