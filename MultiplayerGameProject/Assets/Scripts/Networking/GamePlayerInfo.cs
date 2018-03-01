public class GamePlayerInfo {

    private int connectionID;
    private Player player;
    private bool dead = false;
    private bool ready = false;
    private bool playingInGame = true;

    public string username = "";
    public long lastHorizTimestamp = -1;

    public GamePlayerInfo(int connectionID)
    {
        this.connectionID = connectionID;
    }

    public int GetConnectionID()
    {
        return connectionID;
    }

    public Player GetPlayer()
    {
        return player;
    }

    public void SetPlayer(Player player)
    {
        this.player = player;
    }
    public void SetDead(bool died)
    {
        this.dead = died;
    }
    public bool GetDead()
    {
        return this.dead;
    }

    public void SetReady(bool ready)
    {
        this.ready = ready;
    }

    public bool GetReady()
    {
        return this.ready;
    }

    public void SetInGame(bool playingInGame)
    {
        this.playingInGame = playingInGame;
    }

    public bool GetInGame()
    {
        return this.playingInGame;
    }

}