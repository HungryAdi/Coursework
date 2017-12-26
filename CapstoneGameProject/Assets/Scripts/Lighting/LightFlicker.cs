using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LightFlicker : MonoBehaviour {

    public float Range = 1f;
    public float CycleLength = 1f;

    float minIntensity;
    float maxIntensity;
    Light dirLight;

    bool increasing = true;

	void Start () {
        dirLight = GetComponent<Light>();

        minIntensity = dirLight.intensity;
        maxIntensity = dirLight.intensity + Mathf.Abs(Range);
	}

    void Update()
    {
        if (increasing)
        {
            dirLight.intensity += (Range * Time.deltaTime) / CycleLength;

            if (dirLight.intensity >= maxIntensity)
            {
                increasing = false;
            }
        }
        else
        {
            dirLight.intensity -= (Range * Time.deltaTime) / CycleLength;

            if (dirLight.intensity <= minIntensity)
            {
                increasing = true;
            }
        }
    }

}
