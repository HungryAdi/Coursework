using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShakeEffect : MonoBehaviour
{
    public float ShakeAmount;

	void Update ()
    {
        transform.position += (Vector3) Random.insideUnitCircle * ShakeAmount;
	}
}
