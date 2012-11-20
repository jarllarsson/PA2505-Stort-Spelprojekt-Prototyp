using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class PipeTypes : MonoBehaviour {
    public Transform pipesPrefab;

    private Dictionary<Transform, Transform[]> pipeTypes;

	// Use this for initialization
	void Start () {
        if (pipesPrefab)
        {
            for (int i = 0; i < pipesPrefab.childCount; i++)
            {
                Transform pipeType          = pipesPrefab.GetChild(i);
                Transform[] pipeOpenings    = new Transform[pipeType.childCount];

                for (int j = 0; j < pipeType.childCount; j++)
                    pipeOpenings[j] = pipeType.GetChild(j);


            }
            Transform pipeChunk = InstantiateRandomPipeType(new Vector3(0, 0, 0));

            InstantiatePipeOnPipe(pipeChunk.GetChild(0));
            //InstantiateRandomPipeTypes(pipeChunk);
        }
	}

    Transform GetRandomPipeType()
    {
        return pipesPrefab.GetChild(Random.Range(0, pipesPrefab.childCount));
    }

    Transform GetRandomChild(Transform parent)
    {
        return parent.GetChild(Random.Range(0, pipesPrefab.childCount));
    }

    Transform InstantiateRandomPipeType(Vector3 position)
    {
        Transform pipeType = pipesPrefab.GetChild(Random.Range(0, pipesPrefab.childCount));
        return (Transform)Instantiate(pipeType, position, Quaternion.identity);
    }

    Transform InstantiatePipeOnPipe(Transform attachment)
    {
        // Instantiate a pipe type chunk
        Transform pipeChunk = (Transform)Instantiate(GetRandomPipeType(), attachment.position, attachment.rotation);
        // Attach the new pipe to the attachment parent 
        pipeChunk.parent = attachment;
        // Get distance between attachment point and the pipeChunk attachment point.
        //float dist = Vector3.Distance(pipeChunk.GetChild(0), pip

        return null;
    }

    Transform[] InstantiateRandomPipeTypes(Transform dockPipe)
    {
        Transform[] pipeTypes = new Transform[dockPipe.childCount];
        for (int j = 0; j < dockPipe.childCount; j++)
            pipeTypes[j] = dockPipe.GetChild(j);

        Transform[] pipeChunks = new Transform[dockPipe.childCount];
        for (int j = 0; j < dockPipe.childCount; j++)
            pipeChunks[j] = (Transform)Instantiate(pipesPrefab.GetChild(Random.Range(0, pipesPrefab.childCount)), dockPipe.GetChild(j).position, dockPipe.GetChild(j).rotation);

        return pipeChunks;
    }

	// Update is called once per frame
	void Update () {
	
	}
}
