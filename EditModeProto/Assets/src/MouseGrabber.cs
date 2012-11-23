using UnityEngine;
using System.Collections;

public class MouseGrabber : MonoBehaviour 
{
    public Transform m_pointOfView;
    public int m_blockLayer=8;
    public int m_connectionMinusLayer=9;
    public int m_connectionPlusLayer=10;
    public Transform m_currentGrabbed = null;
    public Transform m_currentGrabbedConnectionPlus = null;
    public Transform m_ship; // used to get ship orientation to correctly orient the blocks

    private Ray ray=new Ray();

    private float m_moduleCollSphereRad = 0.4f;
    private bool m_drawDbgSphere = false;
    private Vector3 m_dbgSpherePos;
    private Color m_dbgSphereCol;

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

        // Default move-to/rotate-to for possible grabbed block
        Vector3 blockMoveTo = transform.position+transform.forward*2.0f;
        Quaternion blockRotateTo = Quaternion.LookRotation(m_pointOfView.rotation * Vector3.forward, m_ship.up);

        // Set up the ray
        var layermask = 1 << m_blockLayer;
        if (m_mode == Mode.FREE)                              // ray if free
            ray.origin = transform.position;
        else if (m_mode == Mode.GRABBING && m_currentGrabbed) // ray if something is grabbed
        {
            layermask = 1 << m_connectionMinusLayer;
            ray.origin = m_currentGrabbed.position;
        }

        ray.direction = (transform.position-m_pointOfView.position).normalized;
        RaycastHit hit;

        // Cast ray
        m_drawDbgSphere = false;
        if (Physics.Raycast(ray, out hit, 40.0f, layermask))
        {
            Debug.DrawLine(ray.origin, hit.point, Color.yellow);
            if (m_mode == Mode.FREE)                              // ray hit(block) if free
            {
                if (grabButton) // if button pressed, grab currently hit object
                {
                    m_currentGrabbed = hit.collider.transform;
                    m_currentGrabbed.gameObject.layer = LayerMask.NameToLayer("Ignore Raycast");
                    m_currentGrabbedConnectionPlus = m_currentGrabbed.FindChild("connection+");
                    m_currentGrabbed.parent = null;
                    m_mode = Mode.GRABBING;
                }
                else // if button released, release currently grabbed object
                {
                    if (m_currentGrabbed!=null) m_currentGrabbed.gameObject.layer = m_blockLayer;
                    m_currentGrabbed = null;
                    m_currentGrabbedConnectionPlus = null;
                    m_mode = Mode.FREE;
                }
            }
            if (m_mode == Mode.GRABBING && m_currentGrabbed) // ray hit(connector) if something is grabbed
            {                        
                Transform other = hit.collider.transform;
                Vector3 newModulePos = other.position + other.rotation*m_currentGrabbedConnectionPlus.localPosition;
                if (!grabButton) // if button released, attach on connector
                {
                    // can't collide with anything in block layer(other modules)
                    if (Physics.CheckSphere(newModulePos, m_moduleCollSphereRad, 1 << m_blockLayer) == false) 
                    {
                        if (m_currentGrabbed != null) m_currentGrabbed.gameObject.layer = m_blockLayer;
                        m_currentGrabbed.position = newModulePos;
     
                        // Rotate using collider forward and up vector of socket owner(for example: the ship)
                        m_currentGrabbed.rotation = Quaternion.LookRotation(-other.forward, other.parent.up); 

                        // attach
                        m_currentGrabbed.parent = other.transform;
                        Debug.Log(m_currentGrabbed.name + " attached to " + other.name);
                        m_currentGrabbed = null;
                        m_mode = Mode.FREE;
                    }
                }
                else
                {
                    m_drawDbgSphere = true;
                    m_dbgSpherePos = newModulePos;
                    // hint what the final alignment will be if button
                    // is released (only for debug purposes right now)
                    if (Physics.CheckSphere(m_dbgSpherePos, m_moduleCollSphereRad, 1 << m_blockLayer))
                        m_dbgSphereCol = Color.red;
                    else
                        m_dbgSphereCol = Color.green;
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
                    if (m_currentGrabbed != null) m_currentGrabbed.gameObject.layer = m_blockLayer;
                    m_currentGrabbed.parent = null;
                    m_currentGrabbed = null;
                    m_currentGrabbedConnectionPlus = null;
                    m_mode = Mode.FREE;
                }
            }
        }

        // Make grabbed block follow the mouse
        if (m_mode == Mode.GRABBING && m_currentGrabbed)
        {
            m_currentGrabbed.position = Vector3.Slerp(m_currentGrabbed.position, blockMoveTo, 20.0f * Time.deltaTime);

            m_currentGrabbed.rotation = Quaternion.Slerp(m_currentGrabbed.rotation, 
                                                         blockRotateTo, 
                                                         20.0f * Time.deltaTime);
        }
	}

    void OnDrawGizmos()
    {
        if (m_drawDbgSphere)
        {
            Gizmos.color = m_dbgSphereCol;
            Gizmos.DrawWireSphere(m_dbgSpherePos, m_moduleCollSphereRad);
        }
    }
}
