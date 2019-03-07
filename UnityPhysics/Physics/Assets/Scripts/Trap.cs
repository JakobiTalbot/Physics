using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Trap : MonoBehaviour
{
    public GameObject m_block;
    public float m_fTrapForce = 100f;

    private bool m_bTrapSprung = false;
	// Use this for initialization
	void Start()
    {
		
	}
	
	// Update is called once per frame
	void Update()
    {
		
	}

    public void ActivateTrap()
    {
        if (m_bTrapSprung)
            return;

        m_block.GetComponent<Rigidbody>().isKinematic = false;

        m_bTrapSprung = true;
    }
}