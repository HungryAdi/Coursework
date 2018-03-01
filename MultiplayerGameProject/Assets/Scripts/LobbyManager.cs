using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class PlayerInfo {
    public string name;
    public string status;

    public PlayerInfo(string name, string status) {
        this.name = name;
        this.status = status;
    }
}

public class ServerInfo {
    public string name;
    public string hostIP;
    public List<PlayerInfo> players;
    public int sessionId;
    public int numPlayers; // Used to see how many players there are before joining.

    public ServerInfo(string name, int sessionId) {
        this.name = name;
        this.sessionId = sessionId;
        players = new List<PlayerInfo>();
    }

    public void AddPlayer(PlayerInfo newPi) {
        foreach (PlayerInfo pi in players)
        {
            if (pi.name == newPi.name)
            {
                return;
            }
        }

        players.Add(newPi);
        LobbyManager.Instance.RenderServerList(this);
    }
}

public class LobbyManager : MonoBehaviour {

    public static LobbyManager Instance;

    public const int MAX_SERVERS = 5;
    public List<ServerInfo> serverList = new List<ServerInfo>();
    public GameObject playerPrefab;
    public GameObject serverPrefab;
    public Color evenColor;
    public Color oddColor;
    private GameObject content;
    private Text statusText;

    public GameObject hostObj;
    public GameObject clientObj;
    public GameObject hostButton;
    public GameObject joinButton;
    public GameObject backButton;
    // Use this for initialization
    void Start() {
        Instance = this;

        content = GameObject.Find("Content");
        statusText = GameObject.Find("Status Text").GetComponent<Text>();
        backButton.SetActive(false);
    }

    // Update is called once per frame
    void Update() {

    }

    public void CreateHostList() {
        RenderServerList(CreateServer());
        hostObj.SetActive(true);
    }

    public void RenderServerList(ServerInfo si) {
        ClearContent();
        statusText.text = "Hosting...";
        int count = 0;
        foreach (PlayerInfo pi in si.players) {
            if (pi != null) {
                GameObject hostItem = Instantiate(playerPrefab, content.transform);
                if (count % 2 == 0) {
                    hostItem.GetComponent<Image>().color = evenColor;
                } else {
                    hostItem.GetComponent<Image>().color = oddColor;
                }
                hostItem.transform.Find("Name Text").GetComponent<Text>().text = pi.name;
                hostItem.transform.Find("Status Text").GetComponent<Text>().text = pi.status;
                GameObject button = hostItem.transform.Find("Ready Button").gameObject;
                if (pi.status == "Hosting") {
                    button.GetComponentInChildren<Text>().text = "Start Game";
                    button.GetComponent<Button>().onClick.AddListener(delegate { GameHost.Instance.OnJoinGame(); });
                } else {
                    button.SetActive(false);
                }

                count++;
            }

        }
    }

    public ServerInfo CreateServer() {
        string username = LoginManager.Instance.username;
        int sessionID = Util.CreateGameSession(username);
        ServerInfo si = new ServerInfo(username, sessionID);
        si.AddPlayer(new PlayerInfo(username, "Hosting"));
        serverList.Add(si);
        HideButtons();
        return si;
    }

    public ServerInfo GetServerByName(string name) {
        foreach (ServerInfo si in serverList) {
            if (si != null) {
                if (si.name == name) {
                    return si;
                }
            }
        }
        return null;
    }

    public void CreateJoinList() {
        ClearContent();
        statusText.text = "Joining...";
        serverList = Util.GetServerList();
        Debug.Log(serverList.Count);
        int count = 0;
        foreach (ServerInfo si in serverList) {
            if (si != null) {
                GameObject joinItem = Instantiate(serverPrefab, content.transform);
                if (count % 2 == 0) {
                    joinItem.GetComponent<Image>().color = evenColor;
                } else {
                    joinItem.GetComponent<Image>().color = oddColor;
                }
                joinItem.transform.Find("Name Text").GetComponent<Text>().text = si.name;
                joinItem.transform.Find("Num Players Text").GetComponent<Text>().text = "" + si.numPlayers;
                joinItem.transform.Find("Join Button").GetComponent<Button>().onClick.AddListener(delegate { JoinGame(si.name); });
                count++;
            }
        }
        HideButtons();

    }

    public void ClearContent() {
        if(SceneManager.GetActiveScene() == SceneManager.GetSceneByName("Lobby")) {
            foreach (Transform t in content.transform) {
                if (t) {
                    Destroy(t.gameObject);
                }
            }
        }
    }

    public void JoinGame(string name) {
        ServerInfo si = GetServerByName(name);
        si.AddPlayer(new PlayerInfo(LoginManager.Instance.username, "Joining"));
        RenderServerList(si);
        statusText.text = "Joined...";
        clientObj.SetActive(true);
        clientObj.GetComponent<GameClient>().hostIP = GetHostIP(name);
    }

    void HideButtons() {
        hostButton.SetActive(false);
        joinButton.SetActive(false);
        backButton.SetActive(true);
    }

    public void ShowButtons() {
        hostButton.SetActive(true);
        joinButton.SetActive(true);
        backButton.SetActive(false);
        ClearContent();
    }

    public void DeleteServer() {
        LoginManager.Instance.database.DeleteGameSession(GetServerByName(LoginManager.Instance.username).sessionId);
    }

    public string GetHostIP(string name) {
        return GetServerByName(name).hostIP;
    }
}
