using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClickMove : MonoBehaviour
{
    public float m_fMoveSpeed = 100f;
    public float m_fScrollMoveSpeed = 0.3f;

    private GameObject m_selectedObject;
    private Vector3 m_v3Destination;
    private float m_fDistance;
    private float m_fOriginalDrag;
	// Use this for initialization
	void Start()
    {
	}
	
	// Update is called once per frame
	void Update()
    {
        if (!m_selectedObject && Input.GetMouseButtonDown(1))
        {
            // create raycast variables
            RaycastHit raycastHit = new RaycastHit();
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            
            // if raycast hit a gameobject that isnt a plane
            if (Physics.Raycast(ray, out raycastHit)
                && !raycastHit.transform.gameObject.CompareTag("Plane"))
            {
                if (raycastHit.transform.gameObject.CompareTag("Player"))
                {
                    raycastHit.transform.gameObject.GetComponentInParent<Ragdoll>().m_bRagdollOn = true;
                }
                // make selected object the hit object
                m_selectedObject = raycastHit.transform.gameObject;
                m_fDistance = raycastHit.distance;
                m_fOriginalDrag = m_selectedObject.GetComponent<Rigidbody>().drag;
                m_selectedObject.GetComponent<Rigidbody>().drag = 10f;
            }
        }

        // there is an object selected
        if (m_selectedObject)
        {
            // add scroll wheel change to distance
            m_fDistance += Input.mouseScrollDelta.y * m_fScrollMoveSpeed;

            // create ray from mouse position
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            // get destination for selected object to seek
            m_v3Destination = ray.origin + (ray.direction * m_fDistance);

            // get direction to seek destination
            Vector3 m_v3Direction = m_v3Destination - m_selectedObject.transform.position;
            // add force to rigidbody in direction of destination
            m_selectedObject.GetComponent<Rigidbody>().AddForce(m_v3Direction * m_fMoveSpeed * m_selectedObject.GetComponent<Rigidbody>().mass);

            // set selectedObject to null if mouse button is released
            if (Input.GetMouseButtonUp(1))
            {
                m_selectedObject.GetComponent<Rigidbody>().drag = m_fOriginalDrag;
                m_selectedObject = null;
            }
        }
	}
}