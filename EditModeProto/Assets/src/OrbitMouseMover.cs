using UnityEngine;
using System.Collections;

public class OrbitMouseMover : MonoBehaviour 
{
    public float m_circleBorderRadius = 4;
    public PlaneMoveInfluencer m_planeMoveInfluencer; ///< Movement influence.
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () 
    {
        // Store origin and dir
        Vector3 orig = new Vector3(transform.localPosition.x, transform.localPosition.y, 0.0f);
        Vector3 dir = new Vector3(m_planeMoveInfluencer.m_move.x, m_planeMoveInfluencer.m_move.y, 0.0f);
        // Update position
        transform.localPosition += dir;
        // alternative: transform.position += transform.parent.TransformDirection(v); where "parent" is the camera

        // Sliding collision against circle border ("edge" of sphere)
        Vector3 diff = (orig + dir);
        float dist = diff.sqrMagnitude;
        if (dist > m_circleBorderRadius * m_circleBorderRadius)
        {
            dist = Mathf.Sqrt(dist);
            diff /= dist;
            float offset = (m_circleBorderRadius - dist);
            transform.localPosition += diff*offset;
        }
        
        
        
	}
}
