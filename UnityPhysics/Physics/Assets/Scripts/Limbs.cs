using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Limbs : MonoBehaviour
{
    public GameObject m_dismemberParticle;
    public float m_fCollisionForceToRagdoll = 1000f;

    // Use this for initialization
    void Start()
    {
		
	}
	
	// Update is called once per frame
	void Update()
    {
	}

    private void OnCollisionEnter(Collision collision)
    {
        float fRagdollThreshold = m_fCollisionForceToRagdoll;
        if (collision.transform.CompareTag("Bullet"))
            fRagdollThreshold /= 4f;
        // check if we aren't already ragdolling
        // also check if the collision impulse is over the threshold to force ragdoll
        if (!GetComponentInParent<Ragdoll>().m_bRagdollOn
            && collision.impulse.sqrMagnitude > fRagdollThreshold)
        {
            GetComponentInParent<Ragdoll>().m_bRagdollOn = true;
        }
    }

    private void OnJointBreak(float breakForce)
    {
        Instantiate(m_dismemberParticle, transform.position, transform.rotation);
    }
}
