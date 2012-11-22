using UnityEngine;
using System.Collections;

public class Connect : MonoBehaviour 
{
    public Transform myConnector;
    public Transform otherConnector;
    
    public int      currentDepth;
    public PipeGen  pipeGen;

    //public bool initalised = false;

	// Use this for initialization
	void Start () 
    {
        Debug.Log("Connection started");
        // First fix so the connection is the pivot:
        myConnector.parent = null;                  // set connector to world space temporarily
        myConnector.position -= transform.position; // reposition connection to world origo + offset from parent
        transform.position = Vector3.zero;          // set this to origo
        transform.parent = myConnector;             // set this to be child to connector

        // Reposition on other connection
        myConnector.position = otherConnector.position;

        // Now rotate:
        myConnector.localRotation = Quaternion.LookRotation(-otherConnector.forward);
        myConnector.RotateAround(myConnector.forward, Random.Range(0.0f, 2 * Mathf.PI)); // optional, rotate random along forward

        // reset parent
        transform.parent = null;
        myConnector.parent = transform;
        //transform.parent = otherConnector;
        transform.parent = otherConnector.parent; // alternative

        myConnector.tag     = "connected";
        otherConnector.tag  = "connected";
        // Done! :)

        //initalised = true;
        //if (currentDepth > 0)
        //{
        //    pipeGen.GeneratePipesOnPipe(transform, currentDepth - 1);
        //}
	}

	// Update is called once per frame
	void Update () 
    {
        //if (initalised)
	}
}
