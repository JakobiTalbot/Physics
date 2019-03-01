using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour
{
    public List<Rigidbody> m_rigidbodies = new List<Rigidbody>();
    public bool m_bRagdollOn
    {
        get { return !m_animator.enabled; }
        set
        {
            m_animator.enabled = !value;
            foreach (Rigidbody rb in m_rigidbodies)
                rb.isKinematic = !value;
        }
    }
    private Animator m_animator;

	// Use this for initialization
	void Start()
    {
        m_animator = GetComponent<Animator>();
        foreach (Rigidbody rb in m_rigidbodies)
            rb.isKinematic = true;

        m_bRagdollOn = false;
	}
	
	// Update is called once per frame
	void Update()
    {
        if (m_bRagdollOn)
            return;
        Vector3 v3SeekPos = new Vector3(GameObject.FindGameObjectWithTag("Player").transform.position.x, transform.position.y, GameObject.FindGameObjectWithTag("Player").transform.position.z);
        transform.LookAt(v3SeekPos);
	}
}