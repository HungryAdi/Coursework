using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerSpawner : MonoBehaviour {
    public GameObject playerPrefab;
    public GameObject AIPrefab;
    public GameObject grappleShooter;
    private int numPlayers;
    public static PlayerSpawner instance;
	// Use this for initialization
	void Start () {
        instance = this;
        if(SceneManager.GetActiveScene() == SceneManager.GetSceneByName("Main") || SceneManager.GetActiveScene() == SceneManager.GetSceneByName("TrainingScene")) {
            SpawnStartingPlayers();
        }

    }

    public GameObject SpawnPlayer(Vector3 position, int i) {
        GameObject player;
        if (PlayerPrefs.GetInt("IsAI" + i) == 1) {
            player = Instantiate(AIPrefab, position, Quaternion.identity);
        } else {
            player = Instantiate(playerPrefab, position, Quaternion.identity);
        }
        GameObject gsGO = Instantiate(grappleShooter, position, Quaternion.identity);
        PlayerInfo pi = player.GetComponent<PlayerInfo>();
        pi.PlayerNumber = i;
        GrappleShooter gs = player.GetComponent<GrappleShooter>();
        gs.grappleGO = gsGO;
        Grapple g = gsGO.GetComponent<Grapple>();
        g.grappleShooter = gs;
        return player;
    }

    public void SpawnStartingPlayers() {
        numPlayers = PlayerPrefs.GetInt("NumberOfPlayers");
        for (int i = 0; i < numPlayers; ++i) {
            SpawnPlayer(GetSpawnPosition(i, false), (i + 1));
        }
    }

    public Vector3 GetSpawnPosition(int index, bool onScreen) {
        Vector3 pos = Camera.main.ViewportToWorldPoint(new Vector3((float)(index + 1) / (numPlayers + 1), onScreen ? 1.05f : 0.95f, 0));
        pos.z = -1f;
        return pos;
    }
}
