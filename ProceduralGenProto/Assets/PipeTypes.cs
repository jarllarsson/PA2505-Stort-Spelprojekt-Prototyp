using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class PipeTypes : MonoBehaviour {
    public Transform pipesPrefab;

    //private Dictionary<Transform, Transform[]> pipeTypes;

	// Use this for initialization
	void Start () {
        if (pipesPrefab)
        {
            //for (int i = 0; i < pipesPrefab.childCount; i++)
            //{
            //    Transform pipeType          = pipesPrefab.GetChild(i);
            //    Transform[] pipeOpenings    = new Transform[pipeType.childCount];

            //    for (int j = 0; j < pipeType.childCount; j++)
            //        pipeOpenings[j] = pipeType.GetChild(j);


            //}
            //Transform pipeA = InstantiateRandomPipeType(new Vector3(0, 0, 0));
            //Transform pipeB = InstantiateRandomPipeType(new Vector3(0, 0, 0));

            Transform[] pipes = new Transform[7];
            pipes[0] = InstantiatePipeType(new Vector3(10, 0, 0), "pipe_6way");
            pipes[0].RotateAround(Vector3.up, Mathf.PI / 5.0f);
            pipes[0].RotateAround(Vector3.right, Mathf.PI / 3.0f);

            for (int i = 1; i < pipes.Length; i++)
            {
                //pipes[i] = InstantiateRandomPipeType(Vector3.zero);//InstantiatePipeType(Vector3.zero, "pipe_no_end_left");
                pipes[i] = InstantiatePipeType(Vector3.zero, "pipe_curved1");
                //pipes[i] = InstantiatePipeType(Vector3.zero, "pipe_no_end");
                ConnectPipes(pipes[0].GetChild(i-1), pipes[i].GetChild(0));
            }
            //ConnectPipes(pipeA.GetChild(0), pipeB.GetChild(0));
            //InstantiatePipeOnPipe(pipeChunk.GetChild(0));
            //InstantiateRandomPipeTypes(pipeChunk);
        }
	}

    Transform GetRandomPipeType()
    {
        return pipesPrefab.GetChild(UnityEngine.Random.Range(0, pipesPrefab.childCount));
    }

    Transform GetRandomChild(Transform parent)
    {
        return parent.GetChild(UnityEngine.Random.Range(0, pipesPrefab.childCount));
    }

    Transform InstantiateRandomPipeType(Vector3 position)
    {
        Transform pipeType = pipesPrefab.GetChild(UnityEngine.Random.Range(0, pipesPrefab.childCount));
        return (Transform)Instantiate(pipeType, position, Quaternion.identity);
    }

    Transform InstantiatePipeType(Vector3 position, string type)
    {
        Transform pipeType = pipesPrefab.FindChild(type);
        return (Transform)Instantiate(pipeType, position, Quaternion.identity);
    }

    Transform InstantiatePipeOnPipe(Transform attachment)
    {
        // Instantiate a pipe type chunk
        Transform pipeChunk = (Transform)Instantiate(GetRandomPipeType(), attachment.position, attachment.rotation);
        // Attach the new pipe to the attachment parent 
        pipeChunk.parent = attachment;
        // Get distance between attachment point and the pipeChunk attachment point.
        //float dist = Vector3.Distance(pipeChunk.GetChild(0), 


        return null;
    }

    public static Nullable<Quaternion> GetRotation(Matrix4x4 matrix)
    {

        var qw = Mathf.Sqrt(1f + matrix.m00 + matrix.m11 + matrix.m22) / 2;

        if (qw == 0)
        {
            Debug.Log("Warning: qw is zero!");
            return null;
        }
        var w = 4 * qw;

        var qx = (matrix.m21 - matrix.m12) / w;

        var qy = (matrix.m02 - matrix.m20) / w;

        var qz = (matrix.m10 - matrix.m01) / w;



        return new Quaternion(qx, qy, qz, qw);
    }

    void ConnectPipes(Transform pipeDockA, Transform pipeDockB)
    {
        Transform pipeA = pipeDockA.parent;
        Transform pipeB = pipeDockB.parent;

        Debug.Log("Old: " + pipeB.forward + " " + Vector3.forward);
        Debug.Log(pipeDockB.name);

        //Matrix4x4 pipeBMat = new Matrix4x4();
        //pipeBMat.SetTRS(Vector3.zero, Quaternion.identity, Vector3.one);

        // Local dock B
        Matrix4x4 pipeDockBMat = new Matrix4x4();
        pipeDockBMat.SetTRS(Vector3.zero, pipeDockB.localRotation, Vector3.one);

        // Global dock A
        Matrix4x4 pipeDockAMat = new Matrix4x4();

        pipeDockAMat.SetTRS(Vector3.zero, pipeDockA.rotation, Vector3.one);

        Matrix4x4 newPipeBMat = pipeDockAMat * Matrix4x4.Inverse(pipeDockBMat);
        Nullable<Quaternion> q = GetRotation(newPipeBMat);

        if (q.HasValue)
        {
            pipeB.rotation = q.Value;
            pipeB.RotateAround(pipeDockB.right, Mathf.PI);
        }
        // Rotation
        // ---
        // Rotate dockB using the inverse of pipeDockB rotation
        //var quart = Quaternion.Inverse(pipeDockB.rotation);
        //pipeB.rotation = Quaternion.Inverse(pipeDockB.localRotation);
        ////pipeB.rotation *= pipeA.rotation;
        //Matrix4x4 m = new Matrix4x4();
        //m.SetTRS(Vector3.zero, pipeDockB.localRotation, Vector3.one);

        //Quaternion q = GetRotation(m);
        //Debug.Log(pipeDockB.localRotation + " " + q);

        // NEJ!
        //pipeB.RotateAroundLocal(Vector3.right, Mathf.PI * 0.5f);
        
        //Debug.Log("Old: " + pipeB.forward + " " + Vector3.forward);
        //pipeB.RotateAroundLocal(Vector3.forward, Mathf.PI * 0.5f);

        //Debug.Log("New: " + pipeB.forward + " " + Vector3.forward);
        //pipeB.rotation *= Quaternion.LookRotation(pipeB.forward, pipeB.up);
        //pipeB.rotation = Quaternion.Inverse(pipeB.localRotation);
        //pipeB.rotation = pipeA.rotation;

        //var quart = Quaternion.Inverse(pipeDockA.rotation * pipeDockB.rotation);
        //pipeB.rotation *= quart;


        //pipeB.rotation *= Quaternion.Inverse(pipeB.rotation);
        //pipeB.rotation *= pipeDockA.rotation;
        //pipeB.RotateAroundLocal(Vector3.forward, 90);

        // ---
        // Positioning
        // ---
        // Get the distance between pipeDockB and pipeB.
        // Since pipeDockB is a child of pipeB, then the distance
        // is the magnitude of pipeDockB.localPosition
        float dist = pipeDockB.localPosition.magnitude;

        // Move pipeB using the forward direction of pipeDockA
        pipeB.position = pipeDockA.position + pipeDockA.forward * dist;

        // ---
        // Attach dockB to pipeDockA
        //pipeB.parent = pipeA;
    }


    Transform[] InstantiateRandomPipeTypes(Transform dockPipe)
    {
        Transform[] pipeTypes = new Transform[dockPipe.childCount];
        for (int j = 0; j < dockPipe.childCount; j++)
            pipeTypes[j] = dockPipe.GetChild(j);

        Transform[] pipeChunks = new Transform[dockPipe.childCount];
        for (int j = 0; j < dockPipe.childCount; j++)
            pipeChunks[j] = (Transform)Instantiate(pipesPrefab.GetChild(UnityEngine.Random.Range(0, pipesPrefab.childCount)), dockPipe.GetChild(j).position, dockPipe.GetChild(j).rotation);

        return pipeChunks;
    }

	// Update is called once per frame
	void Update () {
	
	}
}
