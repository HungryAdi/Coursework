using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class UIController : MonoBehaviour {

	public static UIController instance;
	// Use this for initialization
	void Start () {
		if (instance != this) {
			Destroy (instance);
			instance = this;
			DontDestroyOnLoad (this.gameObject);
		}
        
	}
	
	// Update is called once per frame
	void Update () {
		
	}


    public void LoadNewLevel(string sceneName)
    {
        SceneManager.LoadScene(sceneName);
    }

    public void LoadNextLevel()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
    }

    public void QuitApplication()
    {
        Application.Quit();
    }
}
