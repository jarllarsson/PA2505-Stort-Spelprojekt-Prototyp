using UnityEngine;
using System.Collections;
using System.Threading;

public class CameraBehaviour : MonoBehaviour 
{
    public Transform m_target;
    public Transform m_origin;
    public float m_rotationSpd;
    public float m_moveSpd;
    public float hitDist=0.0f;
    private Vector3 hitCloseOffset = Vector3.zero;
    public float collLerp = 5.0f;
    private float origDist;
    private Vector3 origDir;
    private float hitOccured = 0.0f;

    private RaycastHit hit = new RaycastHit();
    private float minDist = 999999.0f;

	// Use this for initialization
	void Start () {
        origDir = m_target.position - m_origin.position;
        origDist = origDir.magnitude;
        origDir = origDir.normalized;
	}
	
	// Update is called once per frame
	void Update () 
    {
        // pre-collision translation and rotation
        transform.position = Vector3.Lerp(transform.position, m_origin.position, m_moveSpd * Time.deltaTime);
        transform.rotation = Quaternion.Slerp(transform.rotation, m_origin.rotation, m_rotationSpd * Time.deltaTime);


        // transform based on collision result
        if (hitOccured>0.5f)
        {
            hitDist = Mathf.Lerp(hitDist, minDist, collLerp * Time.deltaTime);
            // Debug.Log((rayDist - hitDist));
            hitCloseOffset = Vector3.Lerp(hitCloseOffset, transform.up * 30.0f, collLerp * Time.deltaTime); // to avoid camera moving into ship
            Debug.DrawLine(m_target.position, m_target.position + hitCloseOffset, Color.blue);       
        }
        else
        {
            hitDist = Mathf.Lerp(hitDist, origDist, collLerp * Time.deltaTime);
            hitCloseOffset = Vector3.Lerp(hitCloseOffset, Vector3.zero, collLerp * Time.deltaTime);
            minDist = 999999.0f;
        }    
        hitOccured -= Time.deltaTime;    

        // final translation
        Vector3 offset = -(transform.forward * hitDist) + hitCloseOffset;
        float offDist = offset.magnitude;
        Ray offRay = new Ray(m_target.position,offset.normalized);
        RaycastHit offHit = new RaycastHit();
        if (Physics.Raycast(offRay, out offHit, offDist))
        {
            if (hit.distance>5.0f)
                offset = offset.normalized * hit.distance;
        }

        transform.position = m_target.position + offset;

        // final rotation
        Quaternion newRotation = Quaternion.LookRotation((m_target.position - transform.position).normalized, transform.up);
        transform.rotation = Quaternion.Slerp(transform.rotation, newRotation, m_rotationSpd * Time.deltaTime);
        Debug.Log(hitOccured);
	}

    void FixedUpdate()
    {
        // Camera collision
        Vector3 d = transform.position - m_target.position;
        Ray ray = new Ray();
        float rayDist = d.magnitude;
        ray.direction = d.normalized;
        // Cast several rays to avoid clipping in corners
        // and to be able to determine ignore-cases(such as small debree)
        //
        // An ignore case should be:
        // * No further away from camera than half the distance (TODO: may need to fine tune this)
        // * Result in a ray miss on most rays.
        int lim = 5;
        float step=1.0f;
        for (int x=-lim;x<lim+1;x++)
        for (int y=-lim;y<lim+1;y++)
        {
            ray.origin = m_target.position + transform.TransformDirection(new Vector3(x,y,0.0f)) * step;
            if (Physics.Raycast(ray, out hit, rayDist))
            {
                if (hit.distance < minDist)
                {
                    minDist = hit.distance;
                    hitOccured = 1.0f;
                    Debug.DrawLine(ray.origin, ray.origin + ray.direction * hit.distance, Color.white);   // considered ray
                }
                else
                    Debug.DrawLine(ray.origin, ray.origin + ray.direction * hit.distance, Color.yellow);  // non-considered hit ray
            }
            else
                Debug.DrawLine(ray.origin, ray.origin + ray.direction * rayDist, Color.red);              // non hit ray
        }
    }


}
