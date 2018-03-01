using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
using XInputDotNetPure;
#endif

public class PlayerInfo : MonoBehaviour, System.IComparable {
    public int PlayerNumber;
    public int LivesLeft;
    public float Points;
    public Color color;
    public GrappleShooter grappleShooter;
    public PunchShooter punchShooter;
    [HideInInspector]
    public Blob blob;
    public SpriteRenderer face;
    public GameObject deathParticles;
    public GameObject hitParticles;
    public float respawnDuration;
    public bool isAI = false;
    private Vector3 respawnPoint;
    [HideInInspector]
    public Rigidbody2D rb;
    [HideInInspector]
    public Collider2D col;
    private bool initiated;
    private bool isStunned;
    private Game.ScreenState screenStateLastFrame;
    private bool invulnerable;
    private bool frozen;
    [HideInInspector]
    public MeshRenderer mr;
    private Material mat;
    [HideInInspector]
    public bool alreadyHit = false;
    private float groundWorryHeight = -3.5f;

    Light playerLight;

    AudioSource source;
    AudioClip deathSound;

    public enum PlayerState
    {
        Grappling,
        Idle,
        Trouble,
        Swiped,
        Punching,
        Hit

    }

    public PlayerState pState;

    void Start() {
        LivesLeft = PlayerPrefs.GetInt("InfiniteLives") == 0 ? PlayerPrefs.GetInt("NumberOfLives") : 0;
        Points = 0;
        rb = GetComponent<Rigidbody2D>();
        mr = GetComponent<MeshRenderer>();

        #if !UNITY_EDITOR_WIN && !UNITY_STANDALONE_WIN
            mr.material.shader = Shader.Find("Sprites/Default");
        #endif

        mat = mr.material;
        col = GetComponent<Collider2D>();
        blob = GetComponent<Blob>();
        color = LoadColor();
        SetColor(color);
        frozen = false;
        initiated = false;
        // respawn to the middle of the screen
        if (PlayerSpawner.instance) {
            respawnPoint = PlayerSpawner.instance.GetSpawnPosition(0, true);
            respawnPoint.x = 0;
        }

        source = GetComponent<AudioSource>();
    }

    public void SetColor(Color color) {
        mr = GetComponent<MeshRenderer>();
        mat = mr.material;
        mat.color = color;
        if (grappleShooter) {
            if (grappleShooter.GetLineRenderer()) {
                grappleShooter.GetLineRenderer().material.color = color;
            }
            if (grappleShooter.grappleGO) {
                grappleShooter.grappleGO.GetComponent<SpriteRenderer>().color = color;
            }
        }
        if (punchShooter) {
            if (punchShooter.punch) {
                punchShooter.punch.GetComponent<SpriteRenderer>().color = color;
            }
        }
    }

    public Color LoadColor() {
        return new Color(PlayerPrefs.GetFloat("ColorR" + PlayerNumber, 1), PlayerPrefs.GetFloat("ColorG" + PlayerNumber, 1), PlayerPrefs.GetFloat("ColorB" + PlayerNumber, 1), 1);
    }

    public bool CanAct() {
        return (Game.instance && Game.instance.Started() && !Game.instance.GameOver() && !IsStunned() && LivesLeft > 0) || SceneManager.GetActiveScene() == SceneManager.GetSceneByName("CharacterReadyScene") || SceneManager.GetActiveScene() == SceneManager.GetSceneByName("TrainingScene");
    }

    // true if the player was on screen last frame
    public bool WasOnScreen() {
        return screenStateLastFrame == Game.ScreenState.OnScreen;
    }

    public void Init() {
        initiated = true;
        ToggleFreezeMovement(false);
    }

    public Collider2D GetCollider() {
        return col;
    }

    void Update() {
        if (GameInput.StartButton.WasPressed(PlayerNumber))
        {
            if (!Navigator.instance.titleMenuActive && Navigator.instance.menuActivePlayerNum == 0)
            {
                Navigator.instance.PauseGameAndOpenMenu(PlayerNumber);
            }
            else if (Navigator.instance.inGameMenu.activeSelf && Navigator.instance.menuActivePlayerNum == PlayerNumber)
            {
                Navigator.instance.Resume();
            }
        }


        if (!frozen && Game.instance && Game.instance.RunningIntro()) {
            ToggleFreezeMovement(true);
        }
        if (!initiated && Game.instance && Game.instance.Started()) {
            Init();
        }
        screenStateLastFrame = Game.GetScreenState(gameObject);
        if (GameInput.StartButton.WasPressed(PlayerNumber)) {
            if (!Navigator.instance.titleMenuActive && Navigator.instance.menuActivePlayerNum == 0) {
                Navigator.instance.PauseGameAndOpenMenu(PlayerNumber);
            } else if (Navigator.instance.inGameMenu.activeSelf && Navigator.instance.menuActivePlayerNum == PlayerNumber) {
                Navigator.instance.Resume();
            }
        }
        if (transform.position.y < groundWorryHeight)
            pState = PlayerState.Trouble;
        if(!alreadyHit && pState == PlayerState.Hit)
        {
            StartCoroutine(Hit(1));
        }
        if (!alreadyHit && pState == PlayerState.Swiped)
        {
            StartCoroutine(Swiped(1));
        }

        if (pState == PlayerState.Idle)
        {
            if (UnityEngine.Random.Range(0, 100) == 1)
            {
                face.sprite = Resources.Load<Sprite>("Art/Expressions/expression_blinking");
            }
            else
                face.sprite = Resources.Load<Sprite>("Art/Expressions/expression_idle");

            //Debug.Log("Idle");
        }
        if (pState == PlayerState.Punching)
            face.sprite = Resources.Load<Sprite>("Art/Expressions/expressions_chargingpunch");

        if(pState == PlayerState.Grappling)
            face.sprite = Resources.Load<Sprite>("Art/Expressions/expression_grappling");
        if(pState == PlayerState.Trouble)
        {
            if (transform.position.y < groundWorryHeight)
                face.sprite = Resources.Load<Sprite>("Art/Expressions/expression_introuble");
            else
                pState = PlayerState.Idle;
        }

    }
    void OnCollisionEnter2D(Collision2D col) {

    }

    void OnTriggerEnter2D(Collider2D col) {
        if (col.CompareTag("Ground") && !invulnerable) {
            OnDeath();
        }
        if (col.CompareTag("Platform") && !invulnerable) {
            Rock rock = col.gameObject.GetComponent<Rock>();
            if (rock.type == Rock.Type.Lava) {
                OnDeath();
            }
        }
    }

    public void Stun(float duration = 1f) {
        grappleShooter.Detach();
        isStunned = true;
        StartCoroutine(WaitForStun(duration));
    }

    public void Push(Vector2 vel) {
        rb.velocity += vel;
    }

    public bool IsStunned() {
        return isStunned;
    }

    private IEnumerator WaitForStun(float duration = 1f) {
        yield return new WaitForSeconds(duration);
        isStunned = false;
    }
    public void OnHit() {
        GameObject particles = Instantiate(hitParticles, transform.position, Quaternion.identity);
        ParticleSystem ps = particles.GetComponent<ParticleSystem>();
        var m = ps.main;
        m.startColor = color;
        Destroy(particles, ps.main.duration);
    }
    public void OnDeath() {
        pState = PlayerState.Idle;
        GameObject particles = Instantiate(deathParticles, transform.position, Quaternion.identity);
        ParticleSystem ps = particles.GetComponent<ParticleSystem>();
        var m = ps.main;
        m.startColor = color;
        Destroy(particles, ps.main.duration);
        if (PlayerPrefs.GetInt("InfiniteLives", 0) != 1) {
            LivesLeft--;
        } else {
            if(PlayerPrefs.GetString("GameMode") == "King of the Hill") {
                Points -= 5;
                Points = Mathf.Clamp(Points, 0, 100);
            } else {
                LivesLeft++;
            }
        }
        if(PlayerPrefs.GetString("GameMode") != "Solo") {
            Game.instance.UpdateHUD(this, Game.instance.GetPlayerDisplay(PlayerNumber - 1));
        }
        grappleShooter.Detach();
        if (punchShooter) {
            punchShooter.ResetPunch();
        }

        float randomVal = UnityEngine.Random.value;
        if (randomVal <= 0.33f) {
            deathSound = Resources.Load<AudioClip>("Audio/SFX/DeathSound1");
        } else if (randomVal <= 0.66f) {
            deathSound = Resources.Load<AudioClip>("Audio/SFX/DeathSound4");
        } else {
            deathSound = Resources.Load<AudioClip>("Audio/SFX/DeathSound5");
        }
        if (!source.isPlaying) {
            source.PlayOneShot(deathSound, 0.5f);
        }

        if (LivesLeft == 0 && PlayerPrefs.GetInt("InfiniteLives") == 0) {
            Game.instance.RemovePlayer(this);
            gameObject.SetActive(false);
            //mr.enabled = false;
            //col.enabled = false;
            return;
        }
        StartCoroutine(Respawn(respawnDuration));
    }

    private IEnumerator Respawn(float duration) {
        transform.position = respawnPoint;
        rb.velocity = Vector2.zero;
        invulnerable = true;
        blob.Restart();
        float timer = 0;
        while (timer < duration) {
            timer += Time.deltaTime;
            Color currColor = mat.color == color ? new Color(color.r, color.g, color.b, color.a / 4f) : color;
            mat.color = currColor;
            yield return new WaitForSeconds(Time.deltaTime);
        }
        mat.color = color;
        invulnerable = false;
    }

    public void ToggleAim(bool activate) {
        if (grappleShooter && grappleShooter.aimGrapple) {
            grappleShooter.aimGrapple.ToggleVisibility(activate);
        }
    }

    public void ToggleFreezeMovement(bool activate) {
        rb.constraints = activate ? RigidbodyConstraints2D.FreezeAll : RigidbodyConstraints2D.FreezeRotation;
        foreach (Transform t in transform) {
            Rigidbody2D trb = t.GetComponent<Rigidbody2D>();
            if (trb) {
                trb.constraints = activate ? RigidbodyConstraints2D.FreezeAll : RigidbodyConstraints2D.FreezeRotation;
            }
        }
    }

    public int CompareTo(object obj) {
        return PlayerNumber - ((PlayerInfo)obj).PlayerNumber;
    }

    public void ShakeController(float time) {
        StartCoroutine(ControllerShake(time));
    }

    private IEnumerator Hit(float time)
    {

        alreadyHit = true;
        face.sprite = Resources.Load<Sprite>("Art/Expressions/expression_punched");
        yield return new WaitForSeconds(time);
        pState = PlayerState.Idle;
        alreadyHit = false;
    }

    private IEnumerator Swiped(float time)
    {
        alreadyHit = true;
        face.sprite = Resources.Load<Sprite>("Art/Expressions/expression_swiped");
        yield return new WaitForSeconds(time);
        pState = PlayerState.Idle;
        alreadyHit = false;
    }

    private IEnumerator ControllerShake(float time) {
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        GamePad.SetVibration((PlayerIndex)PlayerNumber - 1, 1, 1);
        yield return new WaitForSeconds(time);
        GamePad.SetVibration((PlayerIndex)PlayerNumber - 1, 0, 0);
#else
        yield return 0;
#endif
    }

    public void OnDisable()
    {
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        //Infinite rumble.
        GamePad.SetVibration((PlayerIndex)PlayerNumber - 1, 0, 0);
#endif
    }
}
