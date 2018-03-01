using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using UnityEngine;
using UnityEngine.Networking;
using System.Collections.Generic;
using System.Net;

public class Util
{

    public static string GetMyIP()
    {
        Network.Connect("https://www.google.com/"); //Google is never down... right?
        return Network.player.externalIP;
    }

    public static byte[] StringToByteBuffer(string str)
    {
        int bufferSize = 1024;
        byte[] buffer = new byte[bufferSize];
        Stream stream = new MemoryStream(buffer);
        BinaryFormatter formatter = new BinaryFormatter();
        formatter.Serialize(stream, str);

        return buffer;
    }

    public static void SendSocketMessage(int socketID, int connectionId, int channel, string message)
    {
        byte error;

        byte[] buffer = Util.StringToByteBuffer(message);

        NetworkTransport.Send(socketID, connectionId, channel, buffer, buffer.Length, out error);
    }

    public static void SendSocketMessage(int socketID, int connectionId, int channel, byte[] buffer)
    {
        byte error;
        NetworkTransport.Send(socketID, connectionId, channel, buffer, buffer.Length, out error);
    }

    public static string TrimTuple(string pair)
    {
        return pair.Replace("(", "").Replace(")", "").Replace(" ", "");
    }

    public static void PrintArray(string[] arr)
    {
        string print = "";

        foreach (string s in arr)
        {
            print += "[" + s + "],";
        }

        Debug.Log(print);
    }

    /// <summary>
    /// Creates a game session and returns a session id.
    /// </summary>
    /// <param name="hostUsername">Username of user who is hosting a game session.</param>
    /// <returns>Session id created.</returns>
    public static int CreateGameSession(string hostUsername)
    {
        using (var client = new WebClient())
        {
            string response = client.DownloadString("http://ec2-54-245-136-197.us-west-2.compute.amazonaws.com/game/CreateGameSession.php?host_username=" + hostUsername);

            //if (response.Contains(","))
            //{
            //    return int.Parse(response.Split(',')[0]);
            //}
        }

        return -1; // Not working
    }

    /// <summary>
    /// Called by the host every 15 seconds to ensure that their game session is still active.
    /// If this is not called within a minute, then the game session will be deleted next time GetServerList() is called.
    /// </summary>
    public static void PingGameSession()
    {
        using (var client = new WebClient())
        {
            client.DownloadString("http://ec2-54-245-136-197.us-west-2.compute.amazonaws.com/game/PingGameSession.php");
        }
    }

    public static List<ServerInfo> GetServerList()
    {
        List<ServerInfo> serverList = new List<ServerInfo>();

        using (var client = new WebClient())
        {
            string servers = client.DownloadString("http://ec2-54-245-136-197.us-west-2.compute.amazonaws.com/game/GetServerList.php");

            Debug.Log("servers: " + servers);

            string[] lines = servers.Split('\n');
            foreach (string line in lines)
            {
                Debug.Log("Line: " + line);
                if (line.Contains(","))
                {
                    string[] data = line.Split(',');

                    int sessionId = int.Parse(data[0]);
                    string hostIp = data[1];
                    string hostUsername = data[2];
                    int numPlayers = int.Parse(data[3]);

                    ServerInfo next = new ServerInfo(hostUsername, sessionId);
                    next.hostIP = hostIp;
                    next.numPlayers = numPlayers;

                    serverList.Add(next);
                }
            }
        }

        return serverList;
    }

    public static void SetNumPlayersServer(int numPlayers)
    {
        using (var client = new WebClient())
        {
            client.DownloadString("http://ec2-54-245-136-197.us-west-2.compute.amazonaws.com/game/SetNumPlayersServer.php?num_players=" + numPlayers);
        }
    }

    public static float BigEnough(float before, float after, float diff)
    {
        return Mathf.Abs(after - before) > diff ? after : before;
    }
}