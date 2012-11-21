using UnityEngine;
using System.Collections;

// Physics based movement for ship
public class Controller : MonoBehaviour 
{
    // Rotation
    private Quaternion m_startRotation; ///< Initial rotation.
    private Quaternion m_rotation; ///< Current rotation.     
    public float m_turnspeed; ///< Rotation speed
    public Vector3 m_inputAngles; ///< Angles to use for rotation

    // Translation
    public float m_power; ///< Thrust power
    public Vector3 m_thrustVec; ///< Thrust direction & length
    

    // Special effects
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
	
	// Note! Fixed update
	void FixedUpdate () 
    {
        // Calc rotation from player input
        float xangle = Input.GetAxis("Vertical") - Input.GetAxis("Mouse Y");
        float yangle = Input.GetAxis("Horizontal") + Input.GetAxis("Mouse X");
        float zangle = Input.GetAxis("Roll");

        m_rotation = Quaternion.identity;
        m_inputAngles = new Vector3(xangle, 
                                   yangle, 
                                   zangle) * m_turnspeed;

        m_rotation *= Quaternion.Euler(m_inputAngles);



        // Calc translation from player input
        m_thrustVec = Vector3.zero;
        m_thrustVec += transform.forward * Input.GetAxis("Thrust") * m_power;
        m_thrustVec += transform.right * Input.GetAxis("StrafeHorizontal") * m_power;
        m_thrustVec += transform.up * Input.GetAxis("StrafeVertical") * m_power;

        // Apply force and torque
        rigidbody.AddForce(m_thrustVec,ForceMode.Acceleration);
        rigidbody.AddTorque(transform.rotation * m_inputAngles, ForceMode.Acceleration);

        //////////////////////////////////////////////////////////////////////////

        // Special effects
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

}
