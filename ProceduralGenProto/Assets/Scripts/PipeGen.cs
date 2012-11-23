using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class PipeGen : MonoBehaviour {
    public Transform pipeTypes;
    public Transform cubeChamber;

	// Use this for initialization
	void Start () {
        if (pipeTypes && cubeChamber)
        {
            pipeColor = renderer.material.color;
            GeneratePipeNetwork(3);
        }
        else
        {
            Debug.LogWarning("Warning: PipeType or CubeChamber not set!");
        }
	}

    public Transform GetRandomPipeType()
    {
        return pipeTypes.GetChild(Random.Range(0, pipeTypes.childCount));
    }

    public int CountAvailablePipeAttachments(Transform pipe)
    {
        int count = 0;
        for (int i = 0; i < pipe.childCount; i++)
        {
            // If a pipe attachment doesn't have a tag that says "connected", then it is free
            Transform attachment = pipe.GetChild(i);
            if (attachment.tag != "connected")
                count++;
        }
        return count;
    }

    Transform GetRandomFreePipeAttachment(Transform pipe)
    {
        // All children occupied?
        List<Transform> freeAttachments = new List<Transform>();
        for (int i = 0; i < pipe.childCount; i++)
        {
            // If a pipe attachment doesn't have a tag that says "connected", then it is free
            Transform attachment = pipe.GetChild(i);
            if (attachment.tag != "connected")
                freeAttachments.Add(attachment);
        }
        if (freeAttachments.Count > 0)
            return freeAttachments[Random.Range(0, freeAttachments.Count)];
        else
        {
            Debug.Log("Warning: No free attachments on " + pipe.name);
            return null;
        }
    }

    List<Transform> GetFreePipeAttachments(Transform pipe)
    {
        // All children occupied?
        List<Transform> freeAttachments = new List<Transform>();
        for (int i = 0; i < pipe.childCount; i++)
        {
            // If a pipe attachment doesn't have a tag that says "connected", then it is free
            Transform attachment = pipe.GetChild(i);
            if (attachment.tag != "connected")
            {
                freeAttachments.Add(attachment);
                Debug.Log("Attachment found: " + attachment.name);
            }
        }
        if (freeAttachments.Count == 0)
            Debug.Log("Warning: No free attachments on " + pipe.name);

        return freeAttachments;
    }

    Color pipeColor;

    void GeneratePipeNetwork(int maxDepth)
    {
        pipeColor += new Color(0.3f, 0, 0);
        Transform[] pipes = GeneratePipesOnCube();

        for (int currentDepth = 0; currentDepth < maxDepth; currentDepth++)
        {
            pipeColor += new Color(0.1f, 0, 0);
            List<Transform> temps = new List<Transform>();
            foreach (Transform pipe in pipes)
                temps.AddRange(GeneratePipesOnPipe(pipe));

            pipes = temps.ToArray();
        }
    }

    Transform[] GeneratePipesOnCube()
    {
        Transform[] pipes = new Transform[6];
        for (int i = 0; i < pipes.Length; i++)
        {
            Transform pipe = ((Transform)Instantiate(GetRandomPipeType(), Vector3.zero, Quaternion.identity));
            ConnectPipes(GetRandomFreePipeAttachment(pipe), cubeChamber.GetChild(i), pipe);
            pipe.renderer.material.color = pipeColor;

            pipes[i] = pipe;
        }
        return pipes;
    }

    //public void ShuffleTransformList(List<Transform> list)
    //{
    //    for (int i = 0; i < list.Count - 1; i++)
    //        list[i] = list[Random.Range(i, list.Count)];
    //}
    public Transform PopTransform(List<Transform> list)
    {
        Transform transform = null;
        if (list.Count > 0)
        {
            transform = list[list.Count - 1];
            list.RemoveAt(list.Count - 1);
        }
        return transform;
    }

    public List<Transform> GeneratePipesOnPipe(Transform targetPipe)
    {
        List<Transform> freeAttachments = GetFreePipeAttachments(targetPipe);
        List<Transform> generatedPipes = new List<Transform>();

        // Check so that there actually are available attachments
        if (freeAttachments.Count > 0)
        {
            for (int i = 0; i < freeAttachments.Count; i++)
            {
                Transform pipe = ((Transform)Instantiate(GetRandomPipeType(), Vector3.zero, Quaternion.identity));
                pipe.renderer.material.color = pipeColor;
                generatedPipes.Add(pipe);
            }

            foreach (Transform pipe in generatedPipes)
                ConnectPipes(pipe.GetChild(0), PopTransform(freeAttachments), pipe);
     
        }
        else
            Debug.Log("Pipe " + targetPipe.name + " has no free attachments.");

        return generatedPipes;
    }

    void ConnectPipes(Transform myConnector, Transform otherConnector, Transform myPipe)
    {
        Debug.Log("Connection started");
        // First fix so the connection is the pivot:
        myConnector.parent = null;                  // set connector to world space temporarily
        myConnector.position -= myPipe.position; // reposition connection to world origo + offset from parent
        myPipe.position = Vector3.zero;          // set this to origo
        myPipe.parent = myConnector;             // set this to be child to connector

        // Reposition on other connection
        myConnector.position = otherConnector.position;

        // Now rotate:
        myConnector.localRotation = Quaternion.LookRotation(-otherConnector.forward);
        // myConnector.RotateAround(myConnector.forward, Random.Range(0.0f, 2 * Mathf.PI)); // optional, rotate random along forward

        // reset parent
        myPipe.parent = null;
        myConnector.parent = myPipe;
        myPipe.parent = otherConnector;
        //myPipe.parent = otherConnector.parent; // alternative

        Debug.Log("Connected " + myConnector.name + " to " + otherConnector.name);

        myConnector.tag = "connected";
        //myConnector.name += " - parent: " +otherConnector.name;
        otherConnector.tag = "connected";
        //otherConnector.name += " - child: " + myConnector.name;
        // Done! :)

    }
}
