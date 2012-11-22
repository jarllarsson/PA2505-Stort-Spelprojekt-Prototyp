using UnityEngine;
using System.Collections;

// Spawns bullet when fire-button is pressed
public class Shooter : MonoBehaviour {
    public Transform m_bulletPrefab;
    public Transform m_origin;
    public Controller m_owner;
    private float m_release;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetAxis("Fire1")>0.0f && m_release<0.0f)
        {
            Transform bullet = Instantiate(m_bulletPrefab, m_origin.position+m_origin.forward*30.0f, m_origin.rotation) as Transform;
            bullet.GetComponent<BulletBehaviour>().AddSpd(m_owner.m_thrustVec);
            m_release = 0.2f;
        }

        m_release -= Time.deltaTime;
	}
}