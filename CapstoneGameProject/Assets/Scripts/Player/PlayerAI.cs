using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAI : MonoBehaviour {
    private Transform Ground;
    public Aim aimGrapple;
    public GrappleShooter qHook;
    public float SideWallDist = 2;
    public float WallDist = 10;
    [HideInInspector]
    public bool hooked = false;
    [HideInInspector]
    public bool shot = false;
    public int levelOfDifficulty = 1;

    public GameObject targetPlatform;
    private List<PlayerInfo> allPlayers;
    private PunchShooter pShooter;
    private PlayerInfo playerInfo;
    private Transform target;
    private Rigidbody2D AIrb2d;
    private PlayerInfo punchTarget = null;
    // Use this for initialization
    void Start() {
        Ground = GameObject.Find("Ground").transform;
        playerInfo = GetComponent<PlayerInfo>();
        playerInfo.isAI = true;
        allPlayers = new List<PlayerInfo>();
        foreach (PlayerInfo p in FindObjectsOfType<PlayerInfo>())
        {
            if (p != playerInfo)
                allPlayers.Add(p);
        }

        pShooter = GetComponent<PunchShooter>();
        AIrb2d = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        if (playerInfo.CanAct())
        {
            if (levelOfDifficulty >= 2)
            {
                if (punchTarget)
                    pShooter.aimPunch.AimAt(punchTarget.transform.position);
                if (!pShooter.isPunchLoaded)
                {
                    if (Random.Range(0.0f, 1000.0f) < 10)
                    {
                        pShooter.LoadPunch();
                        if (levelOfDifficulty >= 3)
                            foreach (PlayerInfo p in allPlayers)
                            {
                                if (p.transform.position.y < playerInfo.transform.position.y)
                                {
                                    punchTarget = p;
                                }

                            }
                        else
                            punchTarget = allPlayers[Random.Range(0, allPlayers.Count)];

                        if (punchTarget == null)
                        {
                            punchTarget = allPlayers[Random.Range(0, allPlayers.Count)];
                        }
                        pShooter.aimPunch.SetAimDirection(punchTarget.transform.position);
                    }
                }

                if (pShooter.isPunchLoaded)
                {
                    if (Random.Range(0.0f, 1000.0f) < 10)
                        pShooter.ReleasePunch();
                    else
                    {
                        pShooter.PunchHeld();
                    }
                }
            }


            if (!hooked && !shot && (Game.instance && !Game.instance.GameOver()))
            {
                if (transform.position.y - Ground.position.y < WallDist && shot == false)
                {
                    shot = true;
                    GameObject[] Platforms = GameObject.FindGameObjectsWithTag("Platform");
                    if (Platforms.Length > 0)
                    {
                        GameObject closestPlatform = null;
                        float closestDist = Mathf.Infinity;
                        foreach (GameObject Platform in Platforms)
                        {
                            if (Platform.transform.position.y >= transform.position.y && Vector2.Distance(transform.position, Platform.transform.position) < closestDist)
                            {
                                if (Vector2.Distance(Platform.transform.position, transform.position) > 2 && Platform.transform != target)
                                {
                                    closestPlatform = Platform;
                                    closestDist = Vector2.Distance(transform.position, Platform.transform.position);
                                }

                            }
                        }
                        if (closestPlatform == null)
                        {
                            Debug.Log("No plat found");
                            return;
                        }
                        aimGrapple.AimAt(closestPlatform.transform.position);
                        target = closestPlatform.transform;
                        targetPlatform = target.gameObject;
                        qHook.Shoot();
                    }
                }
            }
            if (Camera.main.WorldToViewportPoint(qHook.transform.position).y > 1)
            {
                qHook.Detach();
            }
            if (hooked)
            {
                if (target)
                {
                    if (Vector2.Distance(target.position, transform.position) < 3 || AIrb2d.velocity.y < 0)
                        qHook.Detach();
                }

            }
        }
    }
}
