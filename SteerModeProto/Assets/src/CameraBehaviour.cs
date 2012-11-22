using UnityEngine;
using System.Collections;
using System.Threading;

// Camera smooth movement and collision behaviour
public class CameraBehaviour : MonoBehaviour 
{
    public Transform m_target; ///< Camera lookat-target
    public Transform m_origin; ///< Camera moveto-goal

    // Movement and rotation values
    public float m_rotationSpd; ///< Speed of rotation towards goal
    public float m_moveSpd; ///< Speed of movement towards goal    
    private float origDist; ///< Start distance between camera and lookat-target

    // Collision values
    public float hitDist=0.0f; ///< Current raycast hit distance that is being used
    private int numHits = 0; ///< Number of ray hits last collision 
    public float collLerp = 5.0f; ///< Speed of collision correction transform lerp
    private float hitOccuredCooldown = 0.0f; ///< Grace period ticker after collision occurred to minimize camera twitching
    public float minCamHitAdjustDistance = 30.0f; ///< Minimum distance from camera to ship, when adjusting after collision
    private float closestHitDist = 999999.0f; ///< Hit distance closest to ship

    // Ray values
    public int rayGridBorderDim = 2; ///< Border dimension of ray grid, size of whole grid = (rayGridBorderDim * 2 + 1)^2
    public float rayGridStepVal=3.0f; ///< Distance step between rays in ray grid
    public float minRaysHitPercentageAccepted = 0.125f; ///< The minimum percentage of rays that must hit before collision is applied
    private RaycastHit hit = new RaycastHit(); ///< Hit info



	// Use this for initialization
	void Start () {
        origDist = (m_target.position - m_origin.position).magnitude;
	}
	
	// Update is called once per frame
	void Update () 
    {
        // pre-collision translation and rotation
        transform.position = Vector3.Lerp(transform.position, m_origin.position, m_moveSpd * Time.deltaTime);
        transform.rotation = Quaternion.Slerp(transform.rotation, m_origin.rotation, m_rotationSpd * Time.deltaTime);

        // only listen to collisions that triggers more than n% of the rays
        int rayGridAxisSz = (rayGridBorderDim * 2 + 1);
        int minRaysToHit = (int)(rayGridAxisSz * rayGridAxisSz * minRaysHitPercentageAccepted + 0.5f); // (does not need to be per-frame)
        
        // Smooth changing between camera offset distance
        if (hitOccuredCooldown>0.5f && numHits > minRaysToHit )  
            hitDist = Mathf.Lerp(hitDist, closestHitDist, collLerp * Time.deltaTime); // when hit...
        else
        {
            hitDist = Mathf.Lerp(hitDist, origDist, collLerp * Time.deltaTime); // ...and when free
            closestHitDist = 999999.0f; // reset to "max"
        }    
        // cooldown tick
        hitOccuredCooldown -= Time.deltaTime; 

        // final translation
        Vector3 offset = -(transform.forward * hitDist);
        transform.position = m_target.position + offset; // offset by collision correction vector

        // final rotation
        Quaternion newRotation = Quaternion.LookRotation((m_target.position - transform.position).normalized, transform.up); // look at target
        transform.rotation = Quaternion.Slerp(transform.rotation, newRotation, m_rotationSpd * Time.deltaTime);
	}

    void FixedUpdate()
    {
        // Camera collision
        Vector3 d = transform.position - m_target.position;
        Ray ray = new Ray();
        float rayDist = d.magnitude;
        ray.direction = d.normalized; // all rays share direction
        // Cast several rays to avoid clipping in corners
        // and to be able to determine ignore-cases(such as small debree)
        numHits = 0;
        for (int x=-rayGridBorderDim;x<rayGridBorderDim+1;x++)
        for (int y=-rayGridBorderDim;y<rayGridBorderDim+1;y++)
        {
            // Create a ray on the grid
            ray.origin = m_target.position + transform.TransformDirection(new Vector3(x,y,0.0f)) * rayGridStepVal;
            if (Physics.Raycast(ray, out hit, rayDist))
            {
                if (hit.distance < closestHitDist)
                {
                    closestHitDist = Mathf.Max(minCamHitAdjustDistance, hit.distance); // camera can not move further than the minimum adjust distance
                    hitOccuredCooldown = 1.0f; // reset cooldown tick
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
