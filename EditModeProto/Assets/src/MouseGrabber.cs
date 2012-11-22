using UnityEngine;
using System.Collections;

public class MouseGrabber : MonoBehaviour 
{
    public Transform m_pointOfView;
    public int m_castToLayer=8;
    public Transform m_currentGrabbed = null;

    private Ray ray=new Ray();

    public enum Mode
    {
        FREE,
        GRABBING
    };
    public Mode m_mode;

	// Use this for initialization
	void Start () 
    {
        m_mode = Mode.FREE;
	}
	
	// Update is called once per frame
	void Update () 
    {
        // Get mouse/gamepad input
        bool grabButton = Input.GetAxis("Fire1")>0.0f;

        // Set up the ray
        var layermask = 1 << m_castToLayer;
        if (m_mode == Mode.FREE)                              // ray if free
            ray.origin = transform.position;
        else if (m_mode == Mode.GRABBING && m_currentGrabbed) // ray if something is grabbed
        {
            layermask = ~layermask;
            ray.origin = m_currentGrabbed.position;
        }

        ray.direction = (transform.position-m_pointOfView.position).normalized;
        RaycastHit hit;

        // Cast ray
        if (Physics.Raycast(ray, out hit, 40.0f, layermask))
        {
            Debug.DrawLine(ray.origin, hit.point, Color.yellow);
            if (m_mode == Mode.FREE)                              // ray hit(block) if free
            {
                if (grabButton) // if button pressed, grab currently hit object
                {
                    m_currentGrabbed = hit.transform;
                    m_mode = Mode.GRABBING;
                }
                else // if button released, release currently grabbed object
                {
                    m_currentGrabbed = null;
                    m_mode = Mode.FREE;
                }
            }
            if (m_mode == Mode.GRABBING && m_currentGrabbed) // ray hit(connector) if something is grabbed
            {
                if (!grabButton) // if button released, attach on connector
                {
                    m_currentGrabbed.position = hit.point;
                    m_currentGrabbed = null;
                    m_mode = Mode.FREE;
                }
            }
        }
        else
        {
            Debug.DrawRay(ray.origin, ray.direction * 40.0f, Color.magenta);

            // Release object if button is released
            if (m_mode == Mode.GRABBING && m_currentGrabbed) // ray if something is grabbed
            {
                if (!grabButton) // if button released, release currently grabbed object
                {
                    m_currentGrabbed = null;
                    m_mode = Mode.FREE;
                }
            }
        }

        // Make grabbed block follow the mouse
        if (m_mode == Mode.GRABBING && m_currentGrabbed)
        {
            m_currentGrabbed.position = Vector3.Slerp(m_currentGrabbed.position, transform.position+transform.forward*2.0f, 20.0f*Time.deltaTime);
        }
	}
}
