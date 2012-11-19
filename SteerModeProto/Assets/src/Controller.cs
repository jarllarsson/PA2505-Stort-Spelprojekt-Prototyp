using UnityEngine;
using System.Collections;

public class Controller : MonoBehaviour 
{
    private Quaternion m_startRotation; ///< Initial rotation.
    private Quaternion m_rotation; ///< Current rotation. 

    public float m_turnspeed;
    public float m_movespeed;
    public float m_power;
    public Vector3 m_thrustVec;
    public Vector3 inputAngles;
    public ParticleSystem m_engineParticles;
    public ParticleSystem m_engineParticleSecond;

	// Use this for initialization
	void Start () 
    {
        m_startRotation = transform.rotation;
        m_rotation = m_startRotation;
        m_engineParticles.enableEmission = false;
        m_engineParticleSecond.enableEmission = false;
	}
	
	// Update is called once per frame
	void FixedUpdate () 
    {
        float xangle = Input.GetAxis("Vertical") - Input.GetAxis("Mouse Y");
        float yangle = Input.GetAxis("Horizontal") + Input.GetAxis("Mouse X");
        float zangle = Input.GetAxis("Roll");

        m_rotation = Quaternion.identity;
        inputAngles = new Vector3(xangle, 
                                   yangle, 
                                   zangle) * m_turnspeed /* Time.deltaTime*/; // For trackball; set relative movement (in 2d space)
        // inputAngles = Vector3.Slerp(inputAngles, Vector3.zero, Time.deltaTime);

        // Transform quaternion with movement vector
        m_rotation *= Quaternion.Euler(inputAngles);

        // Rotate
        // transform.rotation = m_rotation;

        // Translate
        m_thrustVec = Vector3.zero;
        m_thrustVec += transform.forward * Input.GetAxis("Thrust") * m_power; // forward is dependent on the previous rotation. alt is: m_rotation*vec3(0,0,1)

        m_thrustVec += transform.right * Input.GetAxis("StrafeHorizontal") * m_power;
        m_thrustVec += transform.up * Input.GetAxis("StrafeVertical") * m_power;

        // m_thrustVec = Vector3.Lerp(m_thrustVec, Vector3.zero, Time.deltaTime*0.3f);
        // rigidbody.velocity = m_thrustVec;
        //+= /*m_rotation */ m_thrustVec * Time.deltaTime;

        rigidbody.AddForce(m_thrustVec,ForceMode.Acceleration);
        rigidbody.AddTorque(transform.rotation * inputAngles, ForceMode.Acceleration);
        // rigidbody.angularVelocity = transform.rotation * inputAngles;

        // particles
        if (Input.GetAxis("Thrust") > 0.1f)
        {
            m_engineParticles.enableEmission = true;
            m_engineParticleSecond.enableEmission = false;
        }
        else if (Input.GetAxis("Thrust") < -0.1f)
        {
            m_engineParticleSecond.enableEmission = true;
            m_engineParticles.enableEmission = false;
        }
        else
        {
            m_engineParticles.enableEmission = false;
            m_engineParticleSecond.enableEmission = false;
        }

        Debug.DrawLine(transform.position, transform.position + m_thrustVec*10.0f);
	}
    /*
    void FixedUpdate()
    {
        
        
    }*/
}
