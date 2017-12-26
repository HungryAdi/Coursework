using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAI : MonoBehaviour {
    private Transform Ground;
    public AimGrapple aimGrapple;
    public GrappleShooter qHook;
    public float SideWallDist = 2;
    public float WallDist = 10;
    [HideInInspector]
    public bool hooked = false;
    [HideInInspector]
    public bool shot = false;
    private Transform target;
    // Use this for initialization
    void Start() {
        Ground = GameObject.Find("Ground").transform;
    }

    // Update is called once per frame
    void Update() {
        if (!hooked && !shot && (Game.instance && !Game.instance.GameOver())) {
            if (transform.position.y - Ground.position.y < WallDist && shot == false) {
                shot = true;
                GameObject[] Platforms = GameObject.FindGameObjectsWithTag("Platform");
                if (Platforms.Length > 0) {
                    GameObject closestPlatform = null;
                    float closestDist = Mathf.Infinity;
                    foreach (GameObject Platform in Platforms) {
                        if (Platform.transform.position.y >= transform.position.y && Vector2.Distance(transform.position, Platform.transform.position) < closestDist) {
                            if(Vector2.Distance(Platform.transform.position,transform.position) > 2f && Platform.transform != target)
                            {
                                closestPlatform = Platform;
                                closestDist = Vector2.Distance(transform.position, Platform.transform.position);
                            }

                        }
                    }
                    if(closestPlatform == null)
                    {
                        Debug.Log("No plat found");
                        return;
                    }
                    aimGrapple.SetAimDirection(closestPlatform.transform.position);
                    target = closestPlatform.transform;
                    qHook.Shoot();
                }
            }
        }
        if (hooked)
        {
            if (target)
            {
                if (Vector2.Distance(target.position, transform.position) < 3)
                    qHook.Detach();
            }

        }
    }
}
