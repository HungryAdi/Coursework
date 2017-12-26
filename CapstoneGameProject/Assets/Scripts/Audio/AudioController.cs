using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.Audio;

public class AudioController : MonoBehaviour {

    public AudioMixer masterMixer;
    public static AudioController instance { get; private set; } // lazy singleton class
    public AudioClip[] sounds;

    private Dictionary<string, AudioClip> clips;
    private AudioSource[] sources;
    private AudioSource backgroundSource;
    private AudioSource sfxSource;
    private AudioSource ambientSource;
    private bool childActive; // For menu toggle, is the menu active?

    // Use this for initialization
    void Awake() {
        sources = GetComponents<AudioSource>();
    }

    void Start() {
        instance = this;

        transform.GetChild(0).gameObject.SetActive(false);
        sources = GetComponents<AudioSource>();
        // make dictionary of audio clips and their names (name is exact filename string)
        clips = new Dictionary<string, AudioClip>();


        //init audio sources
        for (int i = 0; i < sources.Length; i++) {
            if (sources[i].outputAudioMixerGroup.name == "BackgroundMusic") {
                backgroundSource = sources[i];
            } else if (sources[i].outputAudioMixerGroup.name == "Ambient") {
                ambientSource = sources[i];
            } else if (sources[i].outputAudioMixerGroup.name == "SFX") {
                sfxSource = sources[i];
            }
        }

        //load all audio clips
        LoadClips();

    }

    void Update() {
        if (!instance) {
            instance = this;
        }
    }

    public void Toggle() {
        childActive = transform.GetChild(0).gameObject.activeSelf;
        transform.GetChild(0).gameObject.SetActive(!childActive);
        Navigator.instance.audioMenuActive = !childActive;
        if (!childActive) {
            EventSystemFirstSelectedSetup.instance.audioControlsMenu = true;
            EventSystemFirstSelectedSetup.instance.ChangeSelectedChild(0, gameObject);
        } else {
            EventSystemFirstSelectedSetup.instance.audioControlsMenu = false;
            EventSystemFirstSelectedSetup.instance.ChangeSelectedChild(0, Navigator.instance.gameObject);
            //Debug.Log("Toggle");
        }


    }

    // for changing audioMixer levels
    public void setMasterLvl(float masterLvl) {
        masterMixer.SetFloat("masterVol", masterLvl);
    }

    public void setSFXLvl(float sfxLvl) {
        masterMixer.SetFloat("sfxVol", sfxLvl);
    }

    public void setAmbLvl(float ambLvl) {
        masterMixer.SetFloat("ambVol", ambLvl);
    }

    public void setBackLvl(float backLvl) {
        masterMixer.SetFloat("backVol", backLvl);
    }

    private void OnEnable() {
        SceneManager.sceneLoaded += OnSceneChange;
    }

    private void OnDisable() {
        SceneManager.sceneLoaded -= OnSceneChange;
    }


    //Switch songs depending on the scene
    private void OnSceneChange(Scene scene, LoadSceneMode mode) {
        if (!backgroundSource) {
            for (int i = 0; i < sources.Length; i++) {
                if (sources[i].outputAudioMixerGroup.name == "BackgroundMusic") {
                    backgroundSource = sources[i];
                }
            }
        }
        if (!ambientSource) {
            for (int i = 0; i < sources.Length; i++) {
                if (sources[i].outputAudioMixerGroup.name == "Ambient") {
                    backgroundSource = sources[i];
                }
            }
        }

        if (scene.name == "TitleScene") {
            AudioClip titleClip = Resources.Load<AudioClip>("Audio/BackgroundMusic/Godcano_Menu_V2");

            backgroundSource.clip = titleClip;


            backgroundSource.Play();
        }

        if (scene.name == "Main") {
            AudioClip clip = Resources.Load<AudioClip>("Audio/BackgroundMusic/Godcano-Theme2_Mixdown2");
            AudioClip ambientClip = Resources.Load<AudioClip>("Audio/Ambient/LavaAmbienceLoop");

            if (ambientSource && ambientClip) {
                ambientSource.clip = ambientClip;
                ambientSource.Play();
            }
            if (backgroundSource && clip) {
                backgroundSource.clip = clip;
                backgroundSource.Play();
            }
        }
    }

    private void LoadClips() {
        for (int i = 0; i < sounds.Length; ++i) {
            clips.Add(sounds[i].name, sounds[i]);
        }
    }

    public void PlaySFX(string clipName, float volume) {
        AudioClip clip = null;
        if (clips.ContainsKey(clipName)) {
            clip = clips[clipName];
        } else {
            Debug.Log("No audio clip: " + clipName);
        }

        if (clip && sfxSource) {
            sfxSource.PlayOneShot(clip, volume);
        }
    }

    public void PlayAmbient(string clipName, float volume) {
        AudioClip clip = null;
        if (clips.ContainsKey(clipName)) {
            clip = clips[clipName];
        } else {
            Debug.Log("No audio clip: " + clipName);
        }
        
        if (clip && ambientSource) {
            ambientSource.PlayOneShot(clip, volume);
        }
    }
}
