using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.SceneManagement;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using UnityEngine.UI;
using System.Linq;

public class GameClient : MonoBehaviour {

    public int tcpID;
    public int udpID;
    public int hostSocketID; //Host socket id
    public int hostConnectionID;

    public RuntimeAnimatorController[] anList;

    public int socketPort = 8888;
    public string hostIP;

    private CameraFollow cam;
    private bool hostReady = false;
    private bool Ready = false;
    private bool EndGameBool = false;

    [SerializeField]
    public GameObject playerPrefab;
    public Sprite localIndicatorSprite;
    public Sprite nonlocalIndicatorSprite;
    private GameObject currPlayer;
    private Player currPlayerScript;
    private bool spectating = false;
    public GameObject winnerTextPrefab;
    private bool GoToLobby = false;

    private bool gameStarted = false;
    private List<int> playerAnims = new List<int>();

    int numPlayers = 2;
    int numPlayersInGame = 0;

    [HideInInspector]
    public static GameClient Instance;
    public static string username = "";

    //Local player information
    public bool dead = false;
    public bool Dying = false;

    //Information taken from host
    public int playerNum = 1; //Set by host
    public List<GameObject> players = new List<GameObject>();

    float lastHoriz = 0;
    Text pingText;
    public long ping;

    private System.Diagnostics.Stopwatch stopwatch;
    private long lastGameStateTimestamp = -1; //Used to keep track of the most recent gamestate processed.

    private bool sessionEnd = false;

    void Start() {
        if (Instance == null) {
            Instance = this;
        } else if (Instance != this) {
            Destroy(gameObject);
        }
        DontDestroyOnLoad(gameObject);

        SceneManager.activeSceneChanged += OnSceneChange;
        NetworkTransport.Init();
        ConnectionConfig config = new ConnectionConfig();

        tcpID = config.AddChannel(QosType.Reliable);
        udpID = config.AddChannel(QosType.Unreliable);

        int maxConnections = 10;
        HostTopology topology = new HostTopology(config, maxConnections); // Allow 2 connections at a time
        hostSocketID = NetworkTransport.AddHost(topology, socketPort); // Creates a host

        Debug.Log("Our socket ID is: " + hostSocketID);

        stopwatch = System.Diagnostics.Stopwatch.StartNew();

        Connect();
    }

    public void Connect() {
        byte error;
        hostConnectionID = NetworkTransport.Connect(hostSocketID, hostIP, socketPort, 0, out error);
        Debug.Log("Connected to server. ConnectionId: " + hostConnectionID);
    }

    void Update() {
        int recHostId;
        int connectionId;
        int channelId;
        byte[] recBuffer = new byte[1024];
        int bufferSize = 1024;
        int dataSize;
        byte error;

        if (hostReady && Ready) {
            Ready = false;
            //gameStarted = true;
            SceneManager.LoadScene("Game");
        }


        if (dead) {
            dead = false;
            Debug.Log("Dying");
            Dying = true;
            Util.SendSocketMessage(hostSocketID, hostConnectionID, tcpID, "dead: " + playerNum);
            //StartCoroutine(checkIfCamMove());

        }

        NetworkEventType recData = NetworkEventType.DataEvent;
        while (recData != NetworkEventType.Nothing) {
            recData = NetworkTransport.Receive(out recHostId, out connectionId, out channelId, recBuffer, bufferSize, out dataSize, out error);
            switch (recData) {
                case NetworkEventType.Nothing:
                    break;
                case NetworkEventType.ConnectEvent:
                    Debug.Log("incoming connection event received");
                    OnJoinGame();
                    break;
                case NetworkEventType.DataEvent:
                    Stream stream = new MemoryStream(recBuffer);
                    BinaryFormatter formatter = new BinaryFormatter();
                    string message = formatter.Deserialize(stream) as string;

                    //Debug.Log(message);

                    if (message.Contains("pnum: ")) {
                        Debug.Log("incoming message event received: " + message);
                        playerNum = Int32.Parse(message.Replace("pnum: ", ""));
                    } else if (message.Contains("seed: ")) {
                        LevelGenerator.Seed = Int32.Parse(message.Replace("seed: ", ""));
                    } else if (message.Contains("Ready")) {
                        hostReady = true;
                    } else if (message.Contains("pl")) {
                        ParseGameState(message);
                    } else if (message.Contains("anim: ")) {
                        string pa = message.Replace("anim: ", "");
                        playerAnims = pa.Split(',').Select(int.Parse).ToList();
                    } else if (message.Contains("numi"))
                    {
                        numPlayersInGame = int.Parse(message.Replace("numi: ", ""));
                        
                    } else if (message.Contains("nump: ")) {
                        numPlayers = int.Parse(message.Replace("nump: ", ""));
                        Util.SendSocketMessage(hostSocketID, hostConnectionID, tcpID, "name: " + username);
                    } else if (message.Contains("pnames: "))
                    {
                        Debug.Log(message);
                        string[] playerNames = message.Replace("pnames: ", "").Split(',');
                        foreach (string pName in playerNames) {
                            if (pName.Length > 0) // No phantom players
                                LobbyManager.Instance.GetServerByName(playerNames[0].ToLower()).AddPlayer(new PlayerInfo(pName, "Joined"));
                        }
                    } else if (message.Contains("dead: ")) {
                        int deadPlayerNum = Int32.Parse(message.Replace("dead: ", ""));
                        if (deadPlayerNum != playerNum) {
                            players[deadPlayerNum].SetActive(false);
                            //Play animation for other player
                            //Tell everyone they died(Broadcast death)
                        }

                    } else if (message.Contains("start: ")) {
                        gameStarted = true;
                        StartCoroutine(checkIfCamMove());
                    } else if (message.Contains("reset: ")) {
                        dead = false;
                        LevelGenerator.Seed = int.Parse(message.Replace("reset: ", ""));
                        Debug.Log("reset dying");
                        Dying = false;
                        currPlayerScript.gameOver = true;
                        EndGameBool = true;
                    } else if (message.Contains("endgame: ")) {
                        string winningPlayer = message.Replace("endgame: ", "");
                        GameObject winText = Instantiate(winnerTextPrefab, GameObject.Find("Canvas").transform);
                        winText.GetComponent<Text>().text = "The winner is " + winningPlayer;
                    } else if (message.Contains("dc: ")) {
                        GameObject p = players[int.Parse(message.Replace("dc: ", ""))];
                        players.Remove(p);
                        p.SetActive(false);
                    } else if (message.Contains("spectate: ")) {
                        string pa = message.Replace("spectate: ", "");
                        playerAnims = pa.Split(',').Select(int.Parse).ToList();
                        spectating = true;
                        SceneManager.LoadScene("Game");

                    }

                    if (message.Contains("msg:")) {

                        string actualMsg = message.Replace("msg:", "");
                        Chatbox.Instance.ShowMessage(actualMsg);
                    }

                    if (message.Contains("sess_end"))
                    {
                        sessionEnd = true;
                    }

                    break;
                case NetworkEventType.DisconnectEvent:
                    Debug.Log("remote client event disconnected");
                    break;
            }
        }

        if (gameStarted && !spectating) {
            if (lastHoriz != Input.GetAxis("Horizontal")) {
                lastHoriz = Input.GetAxis("Horizontal");
                Util.SendSocketMessage(hostSocketID, hostConnectionID, udpID, "h: " + stopwatch.ElapsedMilliseconds + ":" + lastHoriz);
                //Added timestamp to input.
            }

            int ping = NetworkTransport.GetCurrentRTT(hostSocketID, hostConnectionID, out error);
            if (pingText)
            {
                pingText.text = "Ping: " + ping;
            }
            

            StartCoroutine(DelayInput(Input.GetAxis("Horizontal"), ping / 2 / 1000f));
        }


    }


    private void LateUpdate() {

        if (EndGameBool) {
            EndGame();
        } else if (sessionEnd)
        {
            gameStarted = false;
            sessionEnd = false;
            SceneManager.LoadScene("Lobby");
        }
    }

    void OnSceneChange(Scene oldScene, Scene newScene) {
        if (newScene == SceneManager.GetSceneByName("Game")) {
            pingText = GameObject.Find("PingText").GetComponent<Text>();
            players = new List<GameObject>();
            Debug.Log(numPlayersInGame);

            Player[] playersInScene = FindObjectsOfType<Player>();
            foreach (Player pl in playersInScene)
                DestroyObject(pl.gameObject);

            for (int i = 0; i < numPlayersInGame; i++)
            {
                GameObject PlayerGO = Instantiate(playerPrefab);
                Player p = PlayerGO.GetComponent<Player>();
                if (playerAnims == null)
                {
                    Debug.Log("null anims");
                }
                PlayerGO.GetComponentInChildren<Animator>().runtimeAnimatorController = anList[playerAnims[i]];
                if (!spectating)
                {
                    if (i == playerNum)
                    {
                        currPlayerScript = p;
                        cam = Camera.main.GetComponent<CameraFollow>();
                        cam.player = PlayerGO;
                        p.SetIndicator(localIndicatorSprite);
                    }
                    else
                    {
                        p.IsLocalPlayer = false;
                        p.SetIndicator(nonlocalIndicatorSprite);
                    }
                }
                players.Add(PlayerGO);

            }
            Debug.Log("scene change dying");
            Dying = false;
            Debug.Log("Send start");
            Util.SendSocketMessage(hostSocketID, hostConnectionID, tcpID, "start: " + playerNum);

            //StartCoroutine(Countdown(5));
        } else if (newScene == SceneManager.GetSceneByName("Lobby"))
        {
            byte error;
            NetworkTransport.Disconnect(hostSocketID, hostConnectionID, out error);
            NetworkTransport.RemoveHost(hostSocketID);
            Destroy(gameObject);
            Destroy(this);
        }
    }

    IEnumerator Countdown(int waitTime) {
        yield return new WaitForSeconds(waitTime);
    }

    IEnumerator checkIfCamMove() {
        while (gameStarted)
        {
            yield return new WaitForSeconds(1);
            if (Dying)
            {
                MoveCamOnDeath();
            }

        }
    }

    IEnumerator DelayInput(float horizontal, float seconds) {
        yield return new WaitForSeconds(seconds);
        currPlayerScript.LocalPlayerHorizontal = horizontal;
    }

    public void OnJoinGame() {
        Util.SendSocketMessage(hostSocketID, hostConnectionID, tcpID, "Ready");
        Ready = true;
    }

    private void ParseGameState(string gameStateString) {
        if (gameStarted) {
            if (players.Count > 0) {
                string[] lines = gameStateString.Split('\n');

                long gameStateTimestamp = long.Parse(lines[0].Replace("gs: ", "")); // Parses timestamp associated with game state.
                if (gameStateTimestamp > lastGameStateTimestamp) {
                    lastGameStateTimestamp = gameStateTimestamp;
                } else {
                    Debug.Log("OUT OF ORDER game state, dropping...");
                    return; //This is an out of order game state string and we should not process it.
                }

                for (int i = 1; i < lines.Count(); i++) {
                    string line = lines[i];

                    string gameState = line.Replace("pl", "");
                    int gameStatePlayerNum = Int32.Parse(gameState.Substring(0, 1));
                    string[] gameStateList = gameState.Split(';');

                    string[] pos = gameStateList[1].Split(',');

                    float xPos = float.Parse(pos[0]);
                    float yPos = float.Parse(pos[1]);
                    float zPos = float.Parse(pos[2]);

                    if (gameStatePlayerNum < numPlayersInGame && players[gameStatePlayerNum] != null) {
                        Vector3 lastP = players[gameStatePlayerNum].transform.position;
                        players[gameStatePlayerNum].transform.position = new Vector3(Util.BigEnough(lastP.x, xPos, 0.0025f), Util.BigEnough(lastP.y, yPos, 0.0025f), zPos);

                        players[gameStatePlayerNum].transform.rotation = Quaternion.Euler(0, 0, float.Parse(gameStateList[2]));

                        string[] vel = gameStateList[3].Split(',');
                        float xVel = float.Parse(vel[0]);
                        float yVel = float.Parse(vel[1]);
                        Vector2 oldV = players[gameStatePlayerNum].GetComponent<Rigidbody2D>().velocity;
                        players[gameStatePlayerNum].GetComponent<Rigidbody2D>().velocity = new Vector2(Util.BigEnough(oldV.x, xVel, 0.0025f), Util.BigEnough(oldV.y, yVel, 0.0025f));

                        players[gameStatePlayerNum].GetComponent<Rigidbody2D>().angularVelocity = float.Parse(gameStateList[4]);
                    }

                }
            }
        }

    }

    public Player GetWinningPlayer() {
        if (!gameStarted) {
            return null;
        }
        Player winner = null;
        float height = float.MinValue;
        foreach (GameObject player in players) {
            if (player == null) {
                winner = null;
                break;
            }
            if (player.transform.position.y > height) {
                height = player.transform.position.y;
                winner = player.GetComponent<Player>();
            }
        }
        return winner;
    }

    private void EndGame() {
        EndGameBool = false;
        //broadcast winner
        gameStarted = false;
        Debug.Log("EndGameDying");
        Dying = false;
        SceneManager.LoadScene("Game");
    }

    private void MoveCamOnDeath() {
        GameObject player = Camera.main.GetComponent<CameraFollow>().player;
        if (gameStarted && Camera.main != null && player != null) {
            if (GetWinningPlayer() != null) {
                Camera.main.GetComponent<CameraFollow>().player = GetWinningPlayer().gameObject;
            }

        } else if (player == null) {
            Debug.Log("null player");
        }

    }




}
