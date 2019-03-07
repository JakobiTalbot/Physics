using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI : MonoBehaviour
{
    public GameObject m_bulletPowerText;
    public GameObject m_crosshair;
    public GameObject m_scope;
	// Use this for initialization
	void Start()
    {
	}
	
	// Update is called once per frame
	void Update()
    {
		
	}

    public void UpdateBulletPower(int nBulletPower)
    {
        m_bulletPowerText.GetComponent<Text>().text = nBulletPower.ToString();
    }

    public void SetZoom(bool bDoZoom)
    {
        m_scope.SetActive(bDoZoom);
        m_crosshair.SetActive(!bDoZoom);
    }
}
