using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckMarkScript : MonoBehaviour {

    public GameObject[] selectables;
    void OnEnable()
    {
        foreach(GameObject g in selectables)
            g.gameObject.SetActive(false);
    }

    void OnDisable()
    {
        foreach (GameObject g in selectables)
            g.gameObject.SetActive(true);
    }
}
