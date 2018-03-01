using System;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Rock : MonoBehaviour {
    public enum Type {
        Normal,
        Lava,
        Solid,
        Breakable,
        King
    }
    private Collider2D col;
    private SpriteRenderer sr;
    private ParticleSystem ps;
    public GameObject poolParticles;
    public Sprite[] breakableSprites;
    public Sprite[] rockSprites;
    public Material[] rockMaterials;
    public Material[] breakableRockMaterials;
    public Type type;
    void Start() {
        col = GetComponent<Collider2D>();
        sr = GetComponent<SpriteRenderer>();
        ps = GetComponent<ParticleSystem>();
        IgnoreCollisions(true);
        InitType();
    }

    public Type InitType() {
        if (col) {
            int r = UnityEngine.Random.Range(0, breakableSprites.Length);
            switch (type) {
                case Type.Breakable:
                    sr.color = Color.white;
                    sr.sprite = breakableSprites[r];
                    sr.material = breakableRockMaterials[r];
                    ps.Stop();
                    gameObject.layer = LayerMask.NameToLayer("Grappleable");
                    break;
                case Type.Lava:
                    sr.color = new Color(1f, 0.8f, 0f);
                    sr.sprite = rockSprites[r];
                    gameObject.layer = LayerMask.NameToLayer("Lava");
                    ps.Play();
                    break;
                case Type.Normal:
                    sr.color = Color.white;
                    ps.Stop();
                    sr.sprite = rockSprites[r];
                    sr.material = rockMaterials[r];
                    gameObject.layer = LayerMask.NameToLayer("Grappleable");
                    break;
                case Type.Solid:
                    sr.color = Color.grey;
                    ps.Stop();
                    sr.sprite = rockSprites[0]; // always circular rock for end game screen
                    sr.material = rockMaterials[0];
                    gameObject.layer = LayerMask.NameToLayer("Grappleable");
                    transform.localScale = Vector3.one;
                    IgnoreCollisions(false);
                    break;
                case Type.King:
                    sr.color = Color.yellow;
                    sr.sprite = rockSprites[0];
                    sr.material = rockMaterials[0];
                    break;
                default:
                    break;
            }
            col.isTrigger = type == Type.Lava;
        }

        return type;
    }
    void Update() {
        if (Game.GetScreenState(gameObject) == Game.ScreenState.BelowScreen) {
            PoolRock();
        }
        if(Game.instance && Game.instance.GameOver()) {
            if(type != Type.Solid) { // dumb temporary solution for end game screen
                PoolRock();
                OnPool();
            }
        }
        if(SceneManager.GetSceneByName("Main") == SceneManager.GetActiveScene()) {
            if (Game.GetScreenState(gameObject) == Game.ScreenState.OnScreen) {
                gameObject.layer = type == Type.Lava ? LayerMask.NameToLayer("Lava") : LayerMask.NameToLayer("Grappleable");
            } else {
                gameObject.layer = LayerMask.NameToLayer("Offscreen");
            }
        }
    }

    private void IgnoreCollisions(bool ignore) {
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Player 1"), LayerMask.NameToLayer("Grappleable"), ignore);
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Player 2"), LayerMask.NameToLayer("Grappleable"), ignore);
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Player 3"), LayerMask.NameToLayer("Grappleable"), ignore);
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Player 4"), LayerMask.NameToLayer("Grappleable"), ignore);
    }

    void PoolRock() {
        // check all of the players to see if they are attached to this rock, if so, detach them
        if(Game.instance && Game.instance.Started()) {
            foreach(PlayerInfo pi in Game.instance.GetPlayers()) {
                if(pi.grappleShooter && pi.grappleShooter.hit && pi.grappleShooter.hit == gameObject) {
                    pi.grappleShooter.Detach();
                }
            }
        }
        if (RockSpawner.instance && gameObject) {
            RockSpawner.instance.ReturnRock(gameObject);
        }
    }
    public void OnPool() {
        GameObject particles = Instantiate(poolParticles, transform.position, Quaternion.identity);
        Destroy(particles, particles.GetComponent<ParticleSystem>().main.duration);
    }
}
