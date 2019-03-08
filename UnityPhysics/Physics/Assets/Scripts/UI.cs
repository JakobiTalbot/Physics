using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI : MonoBehaviour
{
    public GameObject m_bulletPowerText;
    public GameObject m_crosshair;
    public GameObject m_scopeParent;
    public GameObject m_showControlsText;
    public GameObject m_controlsText;
    public GameObject m_scope;
    public GameObject[] m_blackBars;

	// Use this for initialization
	void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
		if (Input.GetKeyDown(KeyCode.F1))
        {
            ToggleControls();
        }

        float fMinDimension = Mathf.Min(Screen.height, Screen.width);
        m_scope.GetComponent<RectTransform>().sizeDelta = new Vector2(fMinDimension, fMinDimension);
        m_scope.GetComponent<RectTransform>().position = new Vector2(Screen.width / 2, Screen.height / 2);
        foreach (var bar in m_blackBars)
        {
            bar.GetComponent<RectTransform>().sizeDelta = new Vector2(fMinDimension * -0.5f, bar.GetComponent<RectTransform>().sizeDelta.y);
        }
    }

    public void UpdateBulletPower(int nBulletPower)
    {
        m_bulletPowerText.GetComponent<Text>().text = nBulletPower.ToString();
    }

    public void SetZoom(bool bDoZoom)
    {
        m_scopeParent.SetActive(bDoZoom);
        m_crosshair.SetActive(!bDoZoom);
    }

    public void ToggleControls()
    {
        m_showControlsText.SetActive(m_controlsText.activeSelf);
        m_controlsText.SetActive(!m_controlsText.activeSelf);
    }
}