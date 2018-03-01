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

public class GameHost : MonoBehaviour {
    public int tcpID;
    private int udpID;

    private int socketID;
    public int winningHeight;
    public int socketPort = 8888;
    private bool clientReady = false;
    private bool GoToLobby = false;
    private bool inGameReady = false;
    private bool Ready = false;
    private bool AllReady = false;
    private bool EndGameBool = false;
    private GameObject dialogBox;
    private Button yesButton;
    private Button noButton;
    public GameObject winnerTextPrefab;
    public Sprite localIndicatorSprite;
    public Sprite nonlocalIndicatorSprite;
    public RuntimeAnimatorController[] anList;
    public bool dead = false;
    public bool Dying = false;

    private CameraFollow cam;

    [SerializeField]
    public GameObject playerPrefab;
    [HideInInspector]
    public static GameHost Instance;
    public static string username = "";

    private List<GamePlayerInfo> playerInfos = new List<GamePlayerInfo>();
    private int inGamePlayersCount = 0;
    private bool gameStarted = false;

    private int seed;

    //Only for update function
    int recHostId;
    int connectionId;
    int channelId;
    byte[] recBuffer = new byte[1024];
    int bufferSize = 1024;
    int dataSize;
    byte error;
    private int[] test;
    private string playerAnims;
    private List<int> playerAnimators = new List<int>();

    System.Diagnostics.Stopwatch stopwatch;

    void Start() {
        if (Instance == null) {
            Instance = this;
        } else if (Instance != this) {
            Destroy(gameObject);
            Destroy(this);
            return;
        }
        DontDestroyOnLoad(gameObject);
        SceneManager.activeSceneChanged += OnSceneChange;

        NetworkTransport.Init();
        ConnectionConfig config = new ConnectionConfig();

        tcpID = config.AddChannel(QosType.Reliable);
        udpID = config.AddChannel(QosType.Unreliable);

        int maxConnections = 10;
        HostTopology topology = new HostTopology(config, maxConnections);
        socketID = NetworkTransport.AddHost(topology, socketPort); // Creates a socket

        seed = System.DateTime.Now.Millisecond; //Creating level generating seed.
        LevelGenerator.Seed = seed;

        GamePlayerInfo hostGPI = new GamePlayerInfo(-1);
        hostGPI.username = username;
        playerInfos.Add(hostGPI); //Add host player

        stopwatch = System.Diagnostics.Stopwatch.StartNew();

        StartCoroutine(PingGameSession()); //Start pinging game session.

        Debug.Log("Our socket ID is: " + socketID);
    }

    void Update() {
        if (!dialogBox && SceneManager.GetActiveScene() == SceneManager.GetSceneByName("Game")) {
            dialogBox = GameObject.Find("Canvas").transform.Find("DialogBox").gameObject;
            yesButton = GameObject.Find("Canvas").transform.Find("DialogBox/YesButton").GetComponent<Button>();
            noButton = GameObject.Find("Canvas").transform.Find("DialogBox/NoButton").GetComponent<Button>();
            yesButton.onClick.AddListener(SetEndGameBool);
            noButton.onClick.AddListener(ChangeToLobby);
        }
        if (AllReady) {
            AllReady = false;
            //Debug.Log(AllReady);
            gameStarted = true;
            StartCoroutine(checkIfCamMove());
        }

        if (inGameReady) {
            bool allReady = true;
            //Debug.Log(inGameReady);
            foreach (GamePlayerInfo p in playerInfos) {
                if (!p.GetReady()) {
                    allReady = false;
                    break;
                }
            }
            if (allReady) {
                inGameReady = false;
                SendToClients("start: ", tcpID);
                AllReady = true;
                Dying = false;
                
            }

        }

        if (clientReady && Ready) {
            Ready = false;

            CreateAnimatorList();
            Debug.Log(playerAnims);
            playerAnimators = playerAnims.Split(',').Select(int.Parse).ToList();
            SendToClients("anim: " + playerAnims, tcpID);
            SceneManager.LoadScene("Game");
        }

        if (gameStarted)
        {
            CheckIfWinner();
        }

        NetworkEventType recData = NetworkEventType.DataEvent;
        while (recData != NetworkEventType.Nothing) {

            recData = NetworkTransport.Receive(out recHostId, out connectionId, out channelId, recBuffer, bufferSize, out dataSize, out error);

            switch (recData) {
                case NetworkEventType.Nothing:
                    break;
                case NetworkEventType.ConnectEvent:
                    Debug.Log("incoming connection event received");

                    GamePlayerInfo gpi = new GamePlayerInfo(connectionId);
                    playerInfos.Add(gpi);
                    if (gameStarted)
                    {
                        gpi.SetInGame(false);
                        Debug.Log("specator");
                    }
                    else
                    {
                        gpi.SetInGame(true);
                        Debug.Log("joiner");
                    }

                    Util.SendSocketMessage(socketID, connectionId, tcpID, "pnum: " + (playerInfos.Count - 1)); // Send player which player they are.
                    Util.SendSocketMessage(socketID, connectionId, tcpID, "seed: " + seed); // Random seed for generation. TODO

                    StartCoroutine(SetNumPlayers());

                    // Update clients with how many players there are.
                    Debug.Log(playerInfos.Count + "PinfoCount");
                    inGamePlayersCount = 0;
                    foreach(GamePlayerInfo p in playerInfos)
                    {
                        if (p.GetInGame()) {
                            inGamePlayersCount++;
                        }
                    }
                    Debug.Log("inGamePlayersCount" + inGamePlayersCount);
                    SendToClients("nump: " + playerInfos.Count, tcpID);
                    SendToClients("numi: " + inGamePlayersCount, tcpID);

                    if (gameStarted)
                    {
                        Util.SendSocketMessage(socketID, connectionId, tcpID, "spectate: " + playerAnims);

                    }


                    break;
                case NetworkEventType.DataEvent:
                    Stream stream = new MemoryStream(recBuffer);
                    BinaryFormatter formatter = new BinaryFormatter();
                    string message = formatter.Deserialize(stream) as string;
                    //Debug.Log("incoming message event received: " + message);

                    if (message.Contains("h: ") && gameStarted) {
                        long horizTimestamp = long.Parse(message.Split(':')[1].Trim()); // Timestamp associated with this horizontal input msg.
                        float remoteHoriz = float.Parse(message.Split(':')[2].Trim());

                        GamePlayerInfo clientGPI = getGamePlayerInfo(connectionId);
                        if (horizTimestamp > clientGPI.lastHorizTimestamp) {
                            clientGPI.lastHorizTimestamp = horizTimestamp;
                            if (clientGPI.GetPlayer())
                            {
                                clientGPI.GetPlayer().RemotePlayerHorizontal = remoteHoriz;
                            }
                            
                        }
                    } else if (message.Contains("Ready")) {
                        clientReady = true;
                    } else if (message.Contains("name: ")) // Username of client
                    {
                        string playerNames = "pnames: ";
                        for (int i = 0; i < playerInfos.Count; i++) {
                            if (playerInfos[i].GetConnectionID() == connectionId) {
                                playerInfos[i].username = message.Replace("name: ", "");
                            }

                            playerNames += playerInfos[i].username;

                            if (playerInfos[i].username.Length > 0) // No phantom players
                                LobbyManager.Instance.GetServerByName(GameHost.username).AddPlayer(new PlayerInfo(playerInfos[i].username, "Joined"));

                            if (i != playerInfos.Count - 1)
                                playerNames += ",";
                        }

                        SendToClients(playerNames, tcpID);
                    } else if (message.Contains("dead: ")) {
                        if (gameStarted) {
                            int deadPlayerNum = Int32.Parse(message.Replace("dead: ", ""));
                            if (deadPlayerNum != 0) {
                                //Debug.Log(deadPlayerNum);
                                playerInfos[deadPlayerNum].SetDead(true);
                                SendToClients("dead: " + deadPlayerNum, tcpID);
                                CheckIfWinner();
                            }

                        }

                    } else if (message.Contains("start: ")) {
                        Debug.Log("received start");
                        playerInfos[int.Parse(message.Replace("start: ", ""))].SetReady(true);
                    }

                    if (message.Contains("msg:")) {
                        SendToClients(message, tcpID);

                        string actualMsg = message.Replace("msg:", "");
                        Chatbox.Instance.ShowMessage(actualMsg);
                    }

                    break;
                case NetworkEventType.DisconnectEvent:
                    Debug.Log("remote client event disconnected");
                    int pnum = 0;
                    for (int i = 0; i < playerInfos.Count; i++) {
                        if (playerInfos[i].GetConnectionID() == connectionId) {
                            pnum = i;
                            playerInfos.RemoveAt(i); // Removes client connection ID
                            break;
                        }
                    }

                    SendToClients("dc: " + pnum, tcpID);
                    break;
            }
        }

        SendGameState();

        if (gameStarted) {
            if (playerInfos[0].GetPlayer())
            {
                playerInfos[0].GetPlayer().LocalPlayerHorizontal = Input.GetAxis("Horizontal");
            }
             //TODO artificial lag.
            if (dead) {
                dead = false;
                Debug.Log("Dying");
                Dying = true;
                playerInfos[0].SetDead(true);
                //MoveCamOnDeath();
                CheckIfWinner();
            }
        }
    }


    //Pings server that game session is active.
    IEnumerator PingGameSession() {
        while (true) {
            Util.PingGameSession();
            yield return new WaitForSeconds(15);
        }
    }

    IEnumerator checkIfCamMove() {
        while (gameStarted) {
            yield return new WaitForSeconds(1);
            if (Dying)
            {
                MoveCamOnDeath();
            }

        }
    }

    //Sets num players fam.
    IEnumerator SetNumPlayers() {
        Util.SetNumPlayersServer(playerInfos.Count);

        yield return null;
    }

    private void LateUpdate() {
        if (EndGameBool) {
            EndGame();
        }
        if (GoToLobby)
        {
            GoToLobby = false;
            gameStarted = false;
            foreach (GamePlayerInfo p in playerInfos)
            {
                p.SetReady(false);
                p.SetDead(false);
            }
            //byte er;
            //NetworkTransport.Disconnect(socketID, connectionId,out er);
            StartCoroutine(WaitForTime(2));

        }
        
    }

    IEnumerator WaitForTime(int time)
    {
        yield return new WaitForSeconds(time);
        SceneManager.LoadScene("Lobby");
    }

    void OnSceneChange(Scene oldScene, Scene newScene) {
        if (newScene == SceneManager.GetSceneByName("Game")) {
            int numPlayers = playerInfos.Count;
            Debug.Log(FindObjectsOfType<GameHost>().Count());
            Debug.Log(numPlayers + " Number of players");
            Player[] play = FindObjectsOfType<Player>();
            foreach (Player pl in play)
                DestroyObject(pl.gameObject);
            for (int i = 0; i < numPlayers; i++) {
                if (playerInfos[i].GetPlayer() == null)
                {
                    GameObject PlayerGO = Instantiate(playerPrefab);
                    Player p = PlayerGO.GetComponent<Player>();
                    PlayerGO.GetComponentInChildren<Animator>().runtimeAnimatorController = anList[playerAnimators[i]];

                    if (i == 0)
                    {
                        cam = Camera.main.GetComponent<CameraFollow>();
                        cam.player = PlayerGO;
                        p.SetIndicator(localIndicatorSprite);
                    }
                    else
                    {
                        p.IsLocalPlayer = false;
                        p.SetIndicator(nonlocalIndicatorSprite);
                    }

                    playerInfos[i].SetPlayer(p);
                    playerInfos[i].SetDead(false);
                    playerInfos[i].SetReady(true);
                }
            }

        }
        if (newScene == SceneManager.GetSceneByName("Lobby"))
        {
            NetworkTransport.RemoveHost(socketID);
            GameHost.Instance = null;
            Destroy(gameObject);
            Destroy(this);

        }
        //Debug.Log("Set to true");
        inGameReady = true;
        playerInfos[0].SetReady(true);
    }

    void SendGameState() {
        if (gameStarted) {
            string gameString = "gs: " + stopwatch.ElapsedMilliseconds + "\n"; // Added timestamp to game state string.

            for (int i = 0; i < inGamePlayersCount; i++) {
                if (playerInfos[i].GetPlayer() != null && playerInfos[i].GetInGame()) {
                    gameString += "pl" + i + ": ;" + Util.TrimTuple(playerInfos[i].GetPlayer().transform.position.ToString()) +
                            ";" + playerInfos[i].GetPlayer().transform.rotation.eulerAngles.z +
                            ";" + Util.TrimTuple(playerInfos[i].GetPlayer().GetComponent<Rigidbody2D>().velocity.ToString()) +
                            ";" + playerInfos[i].GetPlayer().GetComponent<Rigidbody2D>().angularVelocity.ToString();

                    if (i != playerInfos.Count - 1)
                        gameString += "\n";
                }
            }

            SendToClients(gameString, udpID);
        }
    }

    public void SendToClients(string message, int channel) {
        foreach (GamePlayerInfo gpi in playerInfos) {
            Util.SendSocketMessage(socketID, gpi.GetConnectionID(), channel, message);
        }
    }

    public void OnJoinGame() {
        SendToClients("Ready", tcpID);
        Ready = true;
    }

    private void CreateAnimatorList() {
        List<int> temp = new List<int>();
        for (int i = 0; i < playerInfos.Count; i++) {
            if (temp.Count >= anList.Length) {
                temp = new List<int>();
            }

            int tempRan = UnityEngine.Random.Range(0, anList.Length);
            while (temp.Contains(tempRan)) {
                tempRan = UnityEngine.Random.Range(0, anList.Length);
            }
            temp.Add(tempRan);


            if (i == 0) {
                playerAnims += tempRan;
            } else {
                playerAnims += "," + tempRan;
            }

        }
    }

    private GamePlayerInfo getGamePlayerInfo(int connectionID) {
        foreach (GamePlayerInfo gpi in playerInfos) {
            if (gpi.GetConnectionID() == connectionID) {
                return gpi;
            }
        }

        return null;
    }

    public Player GetWinningPlayer() {
        if (!gameStarted) {
            return null;
        }
        Player winner = null;
        float height = float.MinValue;
        foreach (GamePlayerInfo pi in playerInfos) {
            Player player = pi.GetPlayer();

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

    public void SetEndGameBool() {
        EndGameBool = true;
        CloseDialogBox();
    }

    private void EndGame() {
        EndGameBool = false;
        //broadcast winner
        gameStarted = false;
        dead = false;
        Dying = false;
        foreach (GamePlayerInfo p in playerInfos) {
            p.SetReady(false);
            p.SetDead(false);
        }
        Debug.Log("reset");
        seed = DateTime.Now.Millisecond;
        LevelGenerator.Seed = seed;
        SendToClients("reset: " + seed, tcpID);
        SceneManager.LoadScene("Game");
    }

    IEnumerator Countdown(int waitTime) {
        yield return new WaitForSeconds(waitTime);
    }

    private void MoveCamOnDeath() {
        if (GetWinningPlayer() != null) {
            Camera.main.GetComponent<CameraFollow>().player = GetWinningPlayer().gameObject;
        }

    }
    public void CloseDialogBox() {
        if (dialogBox) {
            dialogBox.SetActive(false);
        }
    }

    public void ChangeToLobby()
    {
        if (dialogBox)
        {
            dialogBox.SetActive(false);
        }
        Debug.Log("sess_end");
        SendToClients("sess_end", tcpID);
        GoToLobby = true;
        
    }

    private bool CheckIfWinner() {
        if (gameStarted)
        {
            int count = 0;
            bool winner = false;
            int winnerNum = -1;
            int playerInt = 0;
            foreach (GamePlayerInfo player in playerInfos)
            {
                if (player.GetPlayer())
                {
                    if (player.GetPlayer().transform.position.y > winningHeight)
                    {
                        //winner is this player break out of everything

                        //this means someone won by winning height rather than one person left
                        count = -1;
                        winnerNum = playerInt;
                        break;
                    }
                }

                if (!player.GetDead())
                {
                    count++;

                }
                playerInt++;
            }
            //if there is one player left or if someone won by winning height
            if(count <= 1)
            {
                //if there is only one person left
                if (count == 1)
                {
                    for (int i = 0; i < playerInfos.Count; i++)
                    {
                        if (!playerInfos[i].GetDead())
                        {
                            //winner is this index
                            winnerNum = i;
                            break;
                        }
                    }
                }
                //Set dialog box to active
                if (!dialogBox.activeSelf)
                {
                    dialogBox.SetActive(true);
                    Debug.Log("endgame");
                    GameObject winText = Instantiate(winnerTextPrefab, GameObject.Find("Canvas").transform);
                    winText.GetComponent<Text>().text = "The winner is " + playerInfos[winnerNum].username;
                    SendToClients("endgame: " + playerInfos[winnerNum].username, tcpID);
                }
              winner = true;
              return winner;
              }


        }
        return false;
    }
}
