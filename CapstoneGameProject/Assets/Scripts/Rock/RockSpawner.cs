using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RockSpawner : MonoBehaviour {
    public GameObject rockPrefab; // rock prefab, has all of the sprite switching and state handling
    public GameObject lavaSpawnWarning; // prefab spawned before lava rocks
    public GameObject placementTextPrefab; // prefab for endgame placement text
    private int numBins; // number of bins (the size of binWeights)
    public float[] binWeights; // distribution weights for each bin
    public float lavaRockSpawnRate; // probability of spawning lava rock
    public float breakableRockSpawnRate; // probability of spawning breakable rock
    public bool redistribute; // whether or not to redistribute spawn weights over time
    public bool considerPlayerPosition; // whether or not to spawn based on where the players currently are
    public static RockSpawner instance; //singleton
    public float spawnFrequency; // how often a rock spawns
    public float kingSpawnFrequency; // how often a rock spawns in King of the Hill mode
    public float spawnDecrease; // how much spawnFrequency decreases each time a rock spawns
    public float minScale; // min value for the scale of the rock
    public float maxScale; // max value for the scale of the rock
    public int maxRockCount; // max number of rocks in the pool
    public float lavaLaunchVelMin; // how fast the lava rocks shoot up when they spawn
    public float lavaLaunchVelMax;
    public float kingRockSize; // how big the king rock is
    public float minGravScale; // min value for the gravity scale of the rock
    public float maxGravScale; // max value for the gravite scale of the rock
    public float warningDuration; // how long the warning appears before the lava rock spawns
    private float timer; // basic timer variable
    private Stack<GameObject> pool; // data structure used to store the rocks, only creates maxRockCount rocks and pools them so that they don't have to be reinstantiated
    private int numActiveRocks; // how many rocks are currently active (not pooled)
    private float binWidth;
    public static float lowestSpawn;
    public static float highestSpawn;
    // Use this for initialization
    void Start() {
        //initialize variables
        instance = this;
        timer = 0;
        numActiveRocks = 0;
        pool = new Stack<GameObject>();
        numBins = binWeights.Length;
        binWidth = 1.0f / numBins;
        lowestSpawn = -12.5f;
        highestSpawn = 17.5f;
        // populate pool with new rocks
        for (int i = 0; i < maxRockCount; ++i) {
            GameObject rock = Instantiate(rockPrefab, new Vector3(-100, -100, 0), Quaternion.identity); // spawn the rock initially offscreen
            ReturnRock(rock);
        }
        // draw some lines to show bins for debugging purposes (only shows up in scene view)
        for (int i = 1; i < numBins; ++i) {
            Vector3 start = Camera.main.ViewportToWorldPoint(new Vector3(i * binWidth, 1, 0));
            start.z = 0;
            Vector3 end = Camera.main.ViewportToWorldPoint(new Vector3(i * binWidth, 0, 0));
            end.z = 0;
            Debug.DrawLine(start, end, Color.red, Mathf.Infinity);
        }
        if(PlayerPrefs.GetString("GameMode") == "King of the Hill") {
            SpawnKingRock();
            spawnFrequency = kingSpawnFrequency;
            lavaRockSpawnRate = 0;
            spawnDecrease = 0;
        }
    }

    // Update is called once per frame
    void Update() {
        timer += Time.deltaTime;
        // if the timer is up
        if (timer > spawnFrequency && Game.instance && !Game.instance.GameOver()) {
            //spawn rock
            SpawnRock();
            if (considerPlayerPosition) {
                foreach (PlayerInfo pi in Game.instance.GetPlayers()) {
                    RedistributeWeights(GetBinFromPosition(pi.transform.position), false);
                }
            }
            timer = 0;
            spawnFrequency += Time.deltaTime * spawnDecrease;
            breakableRockSpawnRate += Time.deltaTime * spawnDecrease;
            breakableRockSpawnRate = Mathf.Clamp(breakableRockSpawnRate, 0, 1 - lavaRockSpawnRate);
        }
    }
    // returns the rock to the pool
    public void ReturnRock(GameObject rock) {
        rock.transform.parent = transform;
        rock.SetActive(false);
        rock.name = "Pooled Rock";
        pool.Push(rock);
        numActiveRocks--;
    }

    // gets the next rock from the pool
    private GameObject GetRock() {
        if (pool.Count > 0) {
            numActiveRocks++;
            return pool.Pop();
        }
        return null;
    }

    // used for endgame sequence
    public void SpawnEndGameRocks() {
        int count = Game.instance.GetDeadPlayers().Count;
        for (int i = 0; i < count; ++i) {
            GameObject rockGO = GetRock();
            if (rockGO) {
                rockGO.name = "Rock";
                rockGO.SetActive(true);
                Rock rock = rockGO.GetComponent<Rock>();
                Rigidbody2D rockRb = rockGO.GetComponent<Rigidbody2D>();
                rock.type = Rock.Type.Solid;
                rock.InitType();
                rockRb.constraints = RigidbodyConstraints2D.FreezeAll;
                rockRb.gravityScale = 0;
                PlayerInfo pi = Game.instance.GetDeadPlayers()[count - i - 1]; // last player in the list is the winner


                //pi.mr.enabled = true;
                //pi.col.enabled = true;

                pi.blob.Restart();
                Vector3 spawnPosition = PlayerSpawner.instance.GetSpawnPosition(pi.PlayerNumber - 1, false);
                spawnPosition.y = Camera.main.ViewportToWorldPoint(new Vector3(0, 0.5f - 0.1f * i, 0)).y;
                rockGO.transform.position = spawnPosition;
                GameObject textGo = Instantiate(placementTextPrefab, spawnPosition, Quaternion.identity);
                textGo.GetComponent<TextMesh>().text = GetPlacement(i);
                spawnPosition.y = Camera.main.ViewportToWorldPoint(Vector3.one).y;
                pi.transform.position = spawnPosition;
                pi.gameObject.SetActive(true);
            }
        }
    }
    // get the placement as a string given index
    public static string GetPlacement(int i) {
        switch (i) {
            case 0:
                return "1st";
            case 1:
                return "2nd";
            case 2:
                return "3rd";
            case 3:
                return "4th";
            default:
                return "";
        }
    }

    // spawns a rock somewhere at the top of the screen
    private void SpawnRock() {
        if (numActiveRocks < maxRockCount) {
            GameObject rockGO = GetRock();
            if (rockGO) {
                rockGO.name = "Rock";
                rockGO.SetActive(true);
                Rock rock = rockGO.GetComponent<Rock>();
                Rigidbody2D rockRb = rockGO.GetComponent<Rigidbody2D>();
                rock.type = GetRandomType();
                rock.InitType();
                // spawns position is at some x point at the top of the screen based on the bin distribution
                Vector3 spawnPosition = GetSpawnPosition(GetNextBin());
                if (spawnPosition.x < 0.1f || spawnPosition.x > Camera.main.ScreenToWorldPoint(new Vector3(Screen.width, 1, 1)).x * 0.9f) {
                    // if the rock is going to spawn offscreen, use a uniform distribution instead
                    spawnPosition = Camera.main.ViewportToWorldPoint(new Vector3(UnityEngine.Random.Range(0.1f, 0.9f), 1, 0));
                }
                if (rock.type != Rock.Type.Lava) {
                    // move the spawn up offscreen
                    spawnPosition.y = highestSpawn;
                    spawnPosition.y += rockGO.GetComponent<Collider2D>().bounds.size.y;

                } else {
                    rockGO.name = "Lava Rock";
                    spawnPosition.y = lowestSpawn; // move to bottom of the screen if lava rock

                }
                spawnPosition.z = 0;
                rockRb.bodyType = RigidbodyType2D.Dynamic;
                rockGO.transform.position = spawnPosition;
                rockRb.gravityScale = Random.Range(minGravScale, maxGravScale);
                float randomScale = Random.Range(minScale, maxScale);
                rockGO.transform.localScale = new Vector3(randomScale, randomScale, 1);
                if (rock.type == Rock.Type.Lava) {
                    StartCoroutine(LavaSpawn(spawnPosition, rockRb));
                }
            }
        }
    }

    // spawns a rock somewhere at the top of the screen
    private void SpawnKingRock() {
        if (numActiveRocks < maxRockCount) {
            GameObject rockGO = GetRock();
            if (rockGO) {
                rockGO.name = "King Rock";
                rockGO.SetActive(true);
                Rock rock = rockGO.GetComponent<Rock>();
                Rigidbody2D rockRb = rockGO.GetComponent<Rigidbody2D>();
                rock.type = Rock.Type.King;
                rock.InitType();
                Vector3 spawnPosition = new Vector3(0, 2, 0);
                rockGO.transform.position = spawnPosition;
                rockRb.bodyType = RigidbodyType2D.Static;
                float randomScale = Random.Range(minScale, maxScale);
                rockGO.transform.localScale = Vector3.one * kingRockSize;
                if (rock.type == Rock.Type.Lava) {
                    StartCoroutine(LavaSpawn(spawnPosition, rockRb));
                }
            }
        }
    }
    // coroutine for lava rock and its warning
    IEnumerator LavaSpawn(Vector3 spawnPosition, Rigidbody2D rockRb) {
        GameObject warning = Instantiate(lavaSpawnWarning, spawnPosition, Quaternion.identity);
        WarningAnim warningAnim = warning.GetComponent<WarningAnim>();
        int level = Random.Range(0, 3);
        warningAnim.level = level;
        Vector3 randomVel = Vector3.zero;
        switch (level) {
            case 0:
                randomVel = new Vector3(0, lavaLaunchVelMin);
                break;
            case 1:
                randomVel = new Vector3(0, (lavaLaunchVelMin + lavaLaunchVelMax) / 2);
                break;
            case 2:
                randomVel = new Vector3(0, lavaLaunchVelMax);
                break;
        }
        warning.GetComponent<Rigidbody2D>().velocity = randomVel;
        Destroy(warning, warningDuration); // spawn the warning for warningDuration seconds
        float gravScale = rockRb.gravityScale;
        rockRb.gravityScale = 0;
        yield return new WaitForSeconds(warningDuration);
        rockRb.velocity = randomVel;
        rockRb.gravityScale = gravScale;
    }

    // generates rock type based on lavaRockSpawnRate and breakableRockSpawnRate
    private Rock.Type GetRandomType() {
        float value = Random.value;
        if (value < lavaRockSpawnRate) {
            return Rock.Type.Lava;
        } else if (value < lavaRockSpawnRate + breakableRockSpawnRate) {
            return Rock.Type.Breakable;
        } else {
            return Rock.Type.Normal;
        }
    }

    // calculates where the rock should spawn given a bin
    private Vector3 GetSpawnPosition(int bin) {
        Vector3 spawnPos = Camera.main.ViewportToWorldPoint(new Vector3(Random.Range(bin * binWidth, (bin + 1) * binWidth), 1, 0));
        spawnPos.z = 0;
        return spawnPos;
    }

    // generates next bin index based on binWeights
    private int GetNextBin() {
        float r = Random.value;
        int i = 0;
        float binSum = 0f;
        while (r >= binSum) {
            binSum += binWeights[i];
            i++;
        }
        if (redistribute) {
            RedistributeWeights(i - 1, true); // the bin is the one right before the loop finishes
        }
        return i - 1;
    }

    // take 1/numBins^2 of the bin and redistribute it to or from the other bins
    private void RedistributeWeights(int bin, bool from) {
        float amount = binWeights[bin] / (numBins * numBins);
        binWeights[bin] -= amount * (from ? 1 : -1);
        for (int i = 0; i < numBins; ++i) {
            if (i != bin) {
                binWeights[i] += (amount / (numBins - 1)) * (from ? 1 : -1);
            }
        }
    }

    // calculates the bin from the given world position
    private int GetBinFromPosition(Vector3 position) {
        Vector3 scaledPos = Camera.main.WorldToViewportPoint(position);
        return (int)(scaledPos.x * numBins);
    }
}
