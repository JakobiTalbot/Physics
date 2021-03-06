﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    public GameObject m_bullet;
    public float m_fMoveSpeed = 10f;
    public float m_fMouseSensitivity = 100f;
    public float m_fJumpForce = 300f;
    public int m_nBulletFirePower = 20;
    public float m_fBulletPowerIncreaseSpeed = 10f;
    public float m_fCameraZoomFOV = 20f;
    public float m_fCrouchHeight = 0.4f;

    private GameObject m_camera;
    private Rigidbody m_rb;
    private float m_fCameraFOV;
    private float m_fStartSensitivity;
    private bool m_bCanJump = true;
	// Use this for initialization
	void Start()
    {
        // get reference to camera
        m_camera = GameObject.FindGameObjectWithTag("MainCamera");
        // lock cursor
        Cursor.lockState = CursorLockMode.Locked;
        // get reference to rigidbody because it is much shorter
        m_rb = GetComponent<Rigidbody>();
        GameObject.FindGameObjectWithTag("UI").GetComponent<UI>().UpdateBulletPower(m_nBulletFirePower);
        // get camera FOV
        m_fCameraFOV = m_camera.GetComponent<Camera>().fieldOfView;
        m_fStartSensitivity = m_fMouseSensitivity;
    }

    // Update is called once per frame
    void Update()
    {
        // movement input
		if (Input.GetKey(KeyCode.W))
        {
            m_rb.velocity += transform.forward * m_fMoveSpeed * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.S))
        {
            m_rb.velocity -= transform.forward * m_fMoveSpeed * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.A))
        {
            m_rb.velocity -= transform.right * m_fMoveSpeed * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.D))
        {
            m_rb.velocity += transform.right * m_fMoveSpeed * Time.deltaTime;
        }

        if (!m_bCanJump && Physics.Raycast(transform.position, Vector3.down, 1f))
        {
            m_bCanJump = true;
        }

        // jump
        if (Input.GetKeyDown(KeyCode.Space) && m_bCanJump)
        {
            m_rb.velocity += transform.up * m_fJumpForce * Time.deltaTime;
            m_bCanJump = false;
        }

        // zoom
        if (Input.GetMouseButton(1))
        {
            m_camera.GetComponent<Camera>().fieldOfView = Mathf.Lerp(m_camera.GetComponent<Camera>().fieldOfView, m_fCameraZoomFOV, 0.5f);
            if (Input.GetMouseButtonDown(1))
                m_fMouseSensitivity = (m_fCameraFOV / m_fCameraZoomFOV) * 6f;
            GameObject.FindGameObjectWithTag("UI").GetComponent<UI>().SetZoom(true);
        }
        else
        {
            m_camera.GetComponent<Camera>().fieldOfView = Mathf.Lerp(m_camera.GetComponent<Camera>().fieldOfView, m_fCameraFOV, 0.5f);
            m_fMouseSensitivity = m_fStartSensitivity;
            GameObject.FindGameObjectWithTag("UI").GetComponent<UI>().SetZoom(false);
        }

        // crouch (dodgy)
        Vector3 v3Scale = transform.localScale;
        if (Input.GetKey(KeyCode.LeftControl))
        {
            v3Scale.y = Mathf.Lerp(v3Scale.y, m_fCrouchHeight, 0.4f);
        }
        else
        {
            v3Scale.y = Mathf.Lerp(v3Scale.y, 1f, 0.4f);
        }
        transform.localScale = v3Scale;

        // adjust bullet power with scroll wheel
        if (!m_camera.GetComponent<ClickMove>().m_selectedObject)
        {
            if (Input.mouseScrollDelta.y > 0)
            {
                // increase more if holding shift
                if (Input.GetKey(KeyCode.LeftShift))
                    m_nBulletFirePower += 10;
                else
                    m_nBulletFirePower += 1;

                GameObject.FindGameObjectWithTag("UI").GetComponent<UI>().UpdateBulletPower(m_nBulletFirePower);
            }
            else if (Input.mouseScrollDelta.y < 0)
            {
                // decrease more if holding shift
                if (Input.GetKey(KeyCode.LeftShift))
                    m_nBulletFirePower -= 10;
                else
                    m_nBulletFirePower -= 1;

                GameObject.FindGameObjectWithTag("UI").GetComponent<UI>().UpdateBulletPower(m_nBulletFirePower);
            }
        }

        // shoot bullet
        if (Input.GetMouseButtonDown(0))
        {
            GameObject bullet = Instantiate(m_bullet, m_camera.transform.position + m_camera.transform.forward, m_camera.transform.rotation);
            bullet.GetComponent<Rigidbody>().velocity += ((bullet.transform.position - m_camera.transform.position).normalized * m_nBulletFirePower) + GetComponent<Rigidbody>().velocity;
        }

        // rotate camera
        transform.Rotate(new Vector3(0, Input.GetAxis("Mouse X"), 0) * m_fMouseSensitivity * Time.deltaTime);
        m_camera.transform.Rotate(new Vector3(-Input.GetAxis("Mouse Y"), 0, 0) * m_fMouseSensitivity * Time.deltaTime);
    }
}