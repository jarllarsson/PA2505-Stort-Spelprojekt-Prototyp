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
    private int numHits = 0;
    public int rayGridBorderDim = 2; // border dimension of ray grid, size of whole grid= (rayGridBorderDim * 2 + 1)^2
    public float minRaysHitPercentageAccepted = 0.125f;
    private Vector3 hitCloseOffset = Vector3.zero;
    public float collLerp = 5.0f;
    private float origDist;
    private Vector3 origDir;
    private float hitOccured = 0.0f;
    public float minCamHitAdjustDistance = 30.0f; // minimum distance from camera to ship

    private RaycastHit hit = new RaycastHit();
    private float closestHitDist = 999999.0f; // shipward closest


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


        // Ray-based camera collision

        Debug.Log(numHits + " > " + (int)(((rayGridBorderDim * 2 * rayGridBorderDim * 2) / 8.0f) + 0.5f) + " == " + (numHits > (int)(((rayGridBorderDim*2*rayGridBorderDim*2)/8.0f)+0.5f)) );

        // only listen to collisions that triggers more than x% of the rays
        int minRaysToHit = (int)((((rayGridBorderDim * 2 + 1) * (rayGridBorderDim * 2 + 1)) * minRaysHitPercentageAccepted) + 0.5f); // does not need to be per-frame
        
        if (hitOccured>0.5f && 
            numHits > minRaysToHit ) 
        {
            hitDist = Mathf.Lerp(hitDist, closestHitDist, collLerp * Time.deltaTime);
            // Debug.Log((rayDist - hitDist));
            // hitCloseOffset = Vector3.Lerp(hitCloseOffset, transform.up * 30.0f, collLerp * Time.deltaTime); // to avoid camera moving into ship
            Debug.DrawLine(m_target.position, m_target.position + hitCloseOffset, Color.blue);       
        }
        else
        {
            hitDist = Mathf.Lerp(hitDist, origDist, collLerp * Time.deltaTime);
            // hitCloseOffset = Vector3.Lerp(hitCloseOffset, Vector3.zero, collLerp * Time.deltaTime);
            closestHitDist = 999999.0f;
        }    
        hitOccured -= Time.deltaTime;    

        // final translation
        Vector3 offset = -(transform.forward * hitDist)/* + hitCloseOffset*/;
        /*float offDist = offset.magnitude;
        Ray offRay = new Ray(m_target.position,offset.normalized);
        RaycastHit offHit = new RaycastHit();
        if (Physics.Raycast(offRay, out offHit, offDist))
        {
            if (hit.distance>5.0f)
                offset = offset.normalized * hit.distance;
        }*/

        transform.position = m_target.position + offset;

        // final rotation
        Quaternion newRotation = Quaternion.LookRotation((m_target.position - transform.position).normalized, transform.up);
        transform.rotation = Quaternion.Slerp(transform.rotation, newRotation, m_rotationSpd * Time.deltaTime);
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
        numHits = 0;
        float step=3.0f;
        for (int x=-rayGridBorderDim;x<rayGridBorderDim+1;x++)
        for (int y=-rayGridBorderDim;y<rayGridBorderDim+1;y++)
        {
            ray.origin = m_target.position + transform.TransformDirection(new Vector3(x,y,0.0f)) * step;
            if (Physics.Raycast(ray, out hit, rayDist))
            {
                if (hit.distance < closestHitDist)
                {
                    closestHitDist = Mathf.Max(minCamHitAdjustDistance, hit.distance);
                    hitOccured = 1.0f;
                    numHits++;
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
