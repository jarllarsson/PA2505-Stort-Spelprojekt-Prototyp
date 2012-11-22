using UnityEngine;
using System.Collections;

public class connect : MonoBehaviour 
{
    public Transform myConnector;
    public Transform otherConnector;
	// Use this for initialization
	void Start () 
    {
        // First fix so the connection is the pivot:
        myConnector.parent = null;                  // set connector to world space temporarily
        myConnector.position -= transform.position; // reposition connection to world origo + offset from parent
        transform.position = Vector3.zero;          // set this to origo
	    transform.parent = myConnector;             // set this to be child to connector

        // Reposition on other connection
        myConnector.position = otherConnector.position;

        // Now rotate:
        myConnector.localRotation = Quaternion.LookRotation(-otherConnector.forward);
        myConnector.RotateAround(myConnector.forward, Random.Range(0.0f, 360.0f)); // optional, rotate random along forward

        // reset parent
        transform.parent = null;
        myConnector.parent = transform;
        transform.parent = otherConnector;
        // transform.parent = otherConnector.parent; // alternative

        // Done! :)
	}
	
	// Update is called once per frame
	void Update () 
    {
        
	}
}
