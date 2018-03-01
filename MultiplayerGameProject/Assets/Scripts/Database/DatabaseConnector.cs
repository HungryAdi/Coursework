using UnityEngine;
using MySql.Data;
using MySql.Data.MySqlClient;
using System.Collections.Generic;

//You might get redlines in Visual Studio but if it runs in unity then its ok. -Brad
public class DatabaseConnector {

    private MySqlConnection connection;
    private string server;
    private string database;
    private string uid;
    private string password;

    public DatabaseConnector()
    {
        Initialize();
    }

    /// <summary>
    /// Called in the constructor. Initializes a MySql database connection object.
    /// </summary>
    private void Initialize()
    {
        server = "ics168faceoff.ch8apcvq8inj.us-west-2.rds.amazonaws.com";
        database = "faceoff_db";
        uid = "game";
        password = "gamefam";
        string connectionString;
        connectionString = "SERVER=" + server + ";" + "DATABASE=" +
        database + ";" + "UID=" + uid + ";" + "PASSWORD=" + password + ";";

        connection = new MySqlConnection(connectionString);
    }

    public bool OpenConnection()
    {
        try
        {
            connection.Open();
            return true;
        }
        catch (MySqlException ex)
        {
            switch (ex.Number)
            {
                case 0:
                    Debug.Log("Cannot connect to server. Contact administrator");
                    break;
                case 1045:
                    Debug.Log("Invalid username/password, please try again");
                    break;
            }
            return false;
        }
    }

    public bool CloseConnection()
    {
        try
        {
            connection.Close();
            return true;
        }
        catch (MySqlException ex)
        {
            Debug.Log(ex.Message);
            return false;
        }
    }

    //Creates a new user row and stores it into the database.
    public bool CreateUser(string username, string password)
    {
        string query = "INSERT INTO users (username, password) VALUES('" + username + "', '" + password + "')";

        if (OpenConnection())
        {
            //create command and assign the query and connection from the constructor
            MySqlCommand cmd = new MySqlCommand(query, connection);

            //Execute command
            cmd.ExecuteNonQuery();

            //close connection
            CloseConnection();

            return true;
        }

        return false;
    }
    
    //Returns true if the username/password is valid.
    public bool Login(string username, string password)
    {
        string query = "SELECT Count(*) FROM users WHERE username='" + username + "' AND password='" + password + "'";

        //Open Connection
        if (this.OpenConnection() == true)
        {
            //Create Mysql Command
            MySqlCommand cmd = new MySqlCommand(query, connection);

            //ExecuteScalar will return one value
            int Count = int.Parse(cmd.ExecuteScalar() + "");

            //close Connection
            CloseConnection();

            return Count > 0;
        }

        return false;
    }

    //Returns true if the inputed username exists.
    public bool UserExists(string username)
    {
        string query = "SELECT Count(*) FROM users WHERE username='" + username + "'";

        //Open Connection
        if (this.OpenConnection() == true)
        {
            //Create Mysql Command
            MySqlCommand cmd = new MySqlCommand(query, connection);

            //ExecuteScalar will return one value
            int Count = int.Parse(cmd.ExecuteScalar() + "");

            //close Connection
            this.CloseConnection();

            return Count > 0;
        }

        return false;
    }

    //Updates a user's highscore.
    public void UpdateHighscore(string username, int newHighscore)
    {
        string query = "UPDATE tableinfo SET name='Joe', age='22' WHERE name='John Smith'";

        //Open connection
        if (OpenConnection())
        {
            //create mysql command
            MySqlCommand cmd = new MySqlCommand();
            //Assign the query using CommandText
            cmd.CommandText = query;
            //Assign the connection using Connection
            cmd.Connection = connection;

            //Execute query
            cmd.ExecuteNonQuery();

            //close connection
            CloseConnection();
        }
    }

    //Deletes a user from the database.
    public void DeleteUser(string username)
    {
        string query = "DELETE FROM users WHERE username='" + username + "'";

        if (OpenConnection())
        {
            MySqlCommand cmd = new MySqlCommand(query, connection);
            cmd.ExecuteNonQuery();
            CloseConnection();
        }
    }

    public void DeleteGameSession(int sessionID)
    {
        string query = "DELETE FROM game_sessions WHERE session_id=" + sessionID;

        //Open connection
        if (OpenConnection())
        {
            //create mysql command
            MySqlCommand cmd = new MySqlCommand();
            //Assign the query using CommandText
            cmd.CommandText = query;
            //Assign the connection using Connection
            cmd.Connection = connection;

            //Execute query
            cmd.ExecuteNonQuery();

            //close connection
            CloseConnection();
        }
    }

    //Deprecated
    public List<ServerInfo> GrabServers()
    {
        List<ServerInfo> serverInfos = new List<ServerInfo>();
        string query = "SELECT * FROM game_sessions";

        //Open Connection
        if (OpenConnection() == true)
        {
            //Create Mysql Command
            MySqlCommand cmd = new MySqlCommand(query, connection);

            MySqlDataReader reader = cmd.ExecuteReader();

            while (reader.Read())
            {
                int sessionID = reader.GetInt32(0);
                string hostIP = reader.GetString(1);
                string hostUsername = reader.GetString(2);

                Debug.Log("sessID: " + sessionID + ", hostIP: " + hostIP + ", hostUsername: " + hostUsername);

                ServerInfo serverInfo = new ServerInfo(hostUsername, sessionID);
                serverInfo.hostIP = hostIP;

                serverInfos.Add(serverInfo);
            }

            CloseConnection();
        }

        return serverInfos;
    }



}
