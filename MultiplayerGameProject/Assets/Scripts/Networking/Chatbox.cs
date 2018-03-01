using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Chatbox : MonoBehaviour {

    public static Chatbox Instance;
    private ScrollRect scrollRect;
    public string chat;
    public InputField inputField;
    public Text text;

    void Start()
    {
        Instance = this;
        scrollRect = GetComponent<ScrollRect>();
    }

    void Update() {
        if (Input.GetKeyDown(KeyCode.Return)) {
            AddMessage();
        }
    }

    public void AddMessage() {
        if (inputField.text.Trim() != "")
        {
            if (GameHost.Instance != null)
            {
                GameHost.Instance.SendToClients("msg:" + LoginManager.Instance.username + ": " + inputField.text, GameHost.Instance.tcpID);
                ShowMessage(LoginManager.Instance.username + ": " + inputField.text);
            }
            else if (GameClient.Instance != null)
            {
                Util.SendSocketMessage(GameClient.Instance.hostSocketID, GameClient.Instance.hostConnectionID, GameClient.Instance.tcpID, "msg:" + LoginManager.Instance.username + ": " + inputField.text);
            }

            inputField.text = "";
        }
    }

    public void ShowMessage(string message)
    {
        chat += message + "\n";
        text.text = chat;
        scrollRect.verticalNormalizedPosition = 0;
    }
}
