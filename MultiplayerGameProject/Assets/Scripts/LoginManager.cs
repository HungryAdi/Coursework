using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class LoginManager : MonoBehaviour {
    public const int MIN_LENGTH = 4; // the min number of characters in a username or password
    public static LoginManager Instance;
    [HideInInspector]
    public DatabaseConnector database;
    private InputField usernameInput;
    private InputField passwordInput;
    private InputField registerUsernameInput;
    private InputField registerPasswordInput;
    private InputField registerConfirmInput;
    private Text registerUsernameText;
    private Text registerPasswordText;
    private Text registerConfirmText;
    private Text startText;
    private Button registerButton;
    private Button loginButton;
    private Button createUserButton;
    public string username;
    private bool loggedIn = false;
    private GameObject loginInfo;
    private GameObject popup;
    private GameObject registerPanel;
    private EventSystem system;

    void Awake() {
        database = new DatabaseConnector();
        if (Instance == null) {
            Instance = this;
        } else if (Instance != this) {
            Destroy(gameObject);
        }
        DontDestroyOnLoad(gameObject);
    }

    void Update() {
        if (SceneManager.GetActiveScene() == SceneManager.GetSceneByName("Login")) {
            if (database == null) {
                database = new DatabaseConnector();
            }
            if (Input.GetKeyDown(KeyCode.Space) && loggedIn) {
                // go to the next scene here, replace once networking is set up
                SceneManager.LoadScene("Lobby");
            }
            if (Input.GetKeyDown(KeyCode.Return)) {
                OnClickLoginButton();
            }
            if (Input.GetKeyDown(KeyCode.Tab)) {
                Selectable next;
                if (registerPanel.activeSelf) {
                    next = system.currentSelectedGameObject.GetComponent<Selectable>().FindSelectableOnDown();
                } else {
                    next = system.currentSelectedGameObject.GetComponent<Selectable>().FindSelectableOnRight();
                }
                if(next != null) {
                    InputField inputfield = next.GetComponent<InputField>();
                    if (inputfield != null) {
                        inputfield.OnPointerClick(new PointerEventData(system));  //if it's an input field, also set the text caret
                    }
                    system.SetSelectedGameObject(next.gameObject, new BaseEventData(system));
                }
            }
        }

    }

    public void UpdateUsernameText() {
        if (IsValidUsername(registerUsernameInput.text)) {
            registerUsernameText.text = "Valid";
            registerUsernameText.color = Color.green;
        } else if (database.UserExists(registerUsernameInput.text)) {
            registerUsernameText.text = "Taken";
            registerUsernameText.color = Color.red;
        } else {
            registerUsernameText.text = "Invalid";
            registerUsernameText.color = Color.red;
        }
        createUserButton.interactable = CanCreateUser();
    }

    public void UpdatePasswordText() {
        if (registerPasswordInput.text.Length >= MIN_LENGTH) {
            registerPasswordText.text = "Valid";
            registerPasswordText.color = Color.green;
        } else {
            registerPasswordText.text = "Invalid";
            registerPasswordText.color = Color.red;
        }
        createUserButton.interactable = CanCreateUser();
    }

    public void UpdateConfirmText() {
        if (registerPasswordInput.text == registerConfirmInput.text && registerPasswordInput.text != "") {
            registerConfirmText.text = "Match";
            registerConfirmText.color = Color.green;
        } else if (registerPasswordInput.text == "") {
            registerConfirmText.text = "Empty";
            registerConfirmText.color = Color.grey;
        } else {
            registerConfirmText.text = "Mismatch";
            registerConfirmText.color = Color.red;
        }
        createUserButton.interactable = CanCreateUser();
    }
    // Use this for initialization
    void Start() {
        FindUI();
        system = EventSystem.current;
    }

    void FindUI() {
        if (!popup) {
            popup = GameObject.Find("Popup");
            popup.SetActive(false);
        }
        if (!loginInfo) {
            loginInfo = GameObject.Find("Login Info");
            usernameInput = loginInfo.transform.Find("Username Input").GetComponent<InputField>();
            passwordInput = loginInfo.transform.Find("Password Input").GetComponent<InputField>();
            registerButton = GameObject.Find("Register Button").GetComponent<Button>();
        }
        if (!startText) {
            startText = GameObject.Find("Start Text").GetComponent<Text>();
        }
        if (!registerPanel) {
            registerPanel = GameObject.Find("Register Panel");
            registerUsernameInput = registerPanel.transform.Find("Username Input").GetComponent<InputField>();
            registerPasswordInput = registerPanel.transform.Find("Password Input").GetComponent<InputField>();
            registerConfirmInput = registerPanel.transform.Find("Confirm Password Input").GetComponent<InputField>();
            registerUsernameText = registerPanel.transform.Find("Username Text 2").GetComponent<Text>();
            registerPasswordText = registerPanel.transform.Find("Password Text 2").GetComponent<Text>();
            registerConfirmText = registerPanel.transform.Find("Confirm Text 2").GetComponent<Text>();
            createUserButton = GameObject.Find("Create User Button").GetComponent<Button>();
            registerPanel.SetActive(false);
        }
        if (!loginButton) {
            loginButton = GameObject.Find("Login Button").GetComponent<Button>();
        }

    }
    public void OnClickLoginButton() {
        //Verify info
        if (database.UserExists(usernameInput.text)) {
            if (usernameInput.text == "") {
                PopupActivate("Please enter a username.");
            } else if (database.Login(usernameInput.text, passwordInput.text)) {
                loginInfo.SetActive(false);
                username = usernameInput.text;
                GameObject.Find("Successful Login").GetComponent<Text>().text = "You are logged in as: " + username;
                startText.text = "Press Space to Start...";
                loggedIn = true;

                GameClient.username = username;
                GameHost.username = username;
            } else {
                PopupActivate("That password is incorrect.");
            }
        } else {
            PopupActivate("That user does not exist.  Please register a new user.");
        }
        usernameInput.text = "";
        passwordInput.text = "";
    }

    public void OnClickLogOutButton() {
        loginInfo.SetActive(true);
        loggedIn = false;
        username = "";
        startText.text = "Login to start...";
    }
    public void OnClickCreateUser() {
        if (database != null) {
            if (!database.UserExists(registerUsernameInput.text)) {
                database.CreateUser(registerUsernameInput.text, registerPasswordInput.text);
                PopupActivate("Account successfully created.");
            } else {
                PopupActivate("That username is already taken. Try another");
            }
        } else {
            Debug.Log("Null database");
        }
        registerPanel.SetActive(false);
    }

    public void OnClickRegister() {
        registerPanel.SetActive(true);
        createUserButton.interactable = CanCreateUser();
    }

    public void OnClickCreateServerButton() {
        //Setup a lobby for the server
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
    }

    public void OnClickStartGameButton() {
        //Take the player to the login menu
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
    }

    public void PopupActivate(string popupText) {
        if (popup) {
            popup.SetActive(true);
            GameObject.Find("PopupText").GetComponent<Text>().text = popupText;
        }

    }

    public void ClosePopup() {
        if (popup) {
            popup.SetActive(false);
        }

    }

    public void CloseRegisterPanel() {
        if (registerPanel) {
            registerPanel.SetActive(false);
        }
    }

    public bool CanCreateUser() {
        return IsValidUsername(registerUsernameInput.text) && IsValidPassword(registerPasswordInput.text, registerConfirmInput.text);
    }

    public bool IsValidUsername(string username) {
        return !database.UserExists(username) && username.Length >= MIN_LENGTH;
    }

    public bool IsValidPassword(string password, string match) {
        return password == match && password.Length >= MIN_LENGTH;
    }
}
