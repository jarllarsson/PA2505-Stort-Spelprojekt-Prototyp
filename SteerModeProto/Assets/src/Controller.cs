using UnityEngine;
using System.Collections;

public class Controller : MonoBehaviour 
{
    private Quaternion m_startRotation; ///< Initial rotation.
    private Quaternion m_rotation; ///< Current rotation. 

    public float m_turnspeed;
    public float m_movespeed;

	// Use this for initialization
	void Start () 
    {
        m_startRotation = transform.rotation;
        m_rotation = m_startRotation;
	}
	
	// Update is called once per frame
	void Update () {

        Vector3 inputAngles = new Vector3(Input.GetAxis("Vertical"), Input.GetAxis("Horizontal"), 0.0f) * m_turnspeed * Time.deltaTime; // For trackball; set relative movement (in 2d space)

        // Transform quaternion with movement vector
        m_rotation *= Quaternion.Euler(inputAngles);

        // Translate and rotate
        transform.position += m_rotation * Vector3.forward * m_movespeed * Time.deltaTime;
        transform.rotation = m_rotation;

	}
}
