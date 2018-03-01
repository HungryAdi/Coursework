using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LevelGenerator : MonoBehaviour {

    public static int Seed = 0;

    public GameObject platformPrefab;
    public GameObject backgroundPrefab;
    public int backgroundHeight;
    public int platformsPerGeneration;
    public int harderGen; // the amount of times it needs to generate before it gets harder
    int height; // half of the screen height
    float width; // half of the screen width
    int count = 0; // how many times Generate() has been called
    Vector3 lastGeneratedPosition;
    Vector3 startingPosition;
    Transform parent;
    Transform backgroundParent;

    // Use this for initialization
    void Start() {
        parent = new GameObject("Platforms").transform;
        backgroundParent = new GameObject("Backgrounds").transform;
        lastGeneratedPosition = transform.position;
        transform.position = new Vector3(transform.position.x, Camera.main.ScreenToWorldPoint(new Vector3(Screen.height, 1)).y, 0);
        height = (int)-transform.position.y / 2;
        // starting platform
        Platform p = Instantiate(platformPrefab, transform.position, Quaternion.identity).GetComponent<Platform>();
        p.transform.SetParent(parent, true);
        p.transform.localScale = new Vector3(transform.localScale.x*100, transform.localScale.y, transform.localScale.z);
        width = Camera.main.ScreenToWorldPoint(new Vector3(Screen.width, 1)).x - p.bc.bounds.extents.x;
        startingPosition = transform.position;
        Random.InitState(Seed);
        Generate();
        Generate();
    }

    // Update is called once per frame
    void Update() {
        if (transform.position.y - lastGeneratedPosition.y > height * platformsPerGeneration) {
            Generate();
            lastGeneratedPosition = transform.position;
        }
    }

    void LateUpdate() {
        // Move to the bottom of the level
        Player winning = null;

        if (GameClient.Instance != null) {
            winning = GameClient.Instance.GetWinningPlayer();
        } else if (GameHost.Instance != null) {
            winning = GameHost.Instance.GetWinningPlayer();
        }

        if (winning != null)
            transform.position = new Vector3(transform.position.x, Mathf.Max(winning.transform.position.y - height, transform.position.y), 0);
    }

    void Generate() {
        // Generates platforms up to the height times the platforms per generation
        GameObject background = Instantiate(backgroundPrefab, new Vector3(0, backgroundHeight * count, 0), Quaternion.identity);
        background.transform.SetParent(backgroundParent, true);
        for (int i = 0; i < height * platformsPerGeneration; i += height) {
            Platform p = Instantiate(platformPrefab, startingPosition + new Vector3(0, height * platformsPerGeneration * count, 0) + new Vector3(Random.Range(-width, width), i, 0), Quaternion.identity).GetComponent<Platform>();
            Platform p2 = Instantiate(platformPrefab, startingPosition + new Vector3(0, height * platformsPerGeneration * count, 0) + new Vector3(Random.Range(-width, width), i + height / 2, 0), Quaternion.identity).GetComponent<Platform>();
            if (count >= harderGen) {
                p2.type = Platform.GetRandomPlatformType();
            }
            if (count >= harderGen * 2) {
                p.type = Platform.GetRandomPlatformType();
            }
            if (count >= harderGen * 4) {
                while (p.type == Platform.Type.NORMAL) {
                    p.type = Platform.GetRandomPlatformType();
                }
                while (p2.type == Platform.Type.NORMAL) {
                    p2.type = Platform.GetRandomPlatformType();
                }
            }
            p.transform.SetParent(parent, true);
            p2.transform.SetParent(parent, true);
        }
        count++;
    }
}
