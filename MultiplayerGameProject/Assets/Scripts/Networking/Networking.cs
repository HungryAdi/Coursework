using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class Networking : MonoBehaviour {

    //Look at this for full tutorial: https://docs.unity3d.com/Manual/UNetUsingTransport.html

    // Use this for initialization
    void Start () {
        // Initializing the Transport Layer with no arguments (default settings)
        NetworkTransport.Init();

        //Two channels for data transfer. One is reliable but slower than the unreliable but fast.
        ConnectionConfig config = new ConnectionConfig();
        int myReliableChannelId = config.AddChannel(QosType.Reliable);
        int myUnreliableChannelId = config.AddChannel(QosType.Unreliable);

        //Defines how many connnections at a time and the connection configuration
        HostTopology topology = new HostTopology(config, 2); // Allow 2 connections at a time?

        int hostId = NetworkTransport.AddHost(topology, 8888); // Creates a host
    }
	
	void Update () {
        int recHostId;
        int connectionId;
        int channelId;
        byte[] recBuffer = new byte[1024];
        int bufferSize = 1024;
        int dataSize;
        byte error;
        NetworkEventType recData = NetworkTransport.Receive(out recHostId, out connectionId, out channelId, recBuffer, bufferSize, out dataSize, out error);
        switch (recData)
        {
            case NetworkEventType.Nothing:         // Nothing happened
                break;
            case NetworkEventType.ConnectEvent:    // Someone connected
                break;
            case NetworkEventType.DataEvent:       // Someone sent us data
                break;
            case NetworkEventType.DisconnectEvent: // Someone disconnected
                break;
        }

        //We can figure out the data from the recBuffer and check who sent us the data from connectionId.

        // We can use three different functions
        // Connect: connectionId = NetworkTransport.Connect(hostId, "192.16.7.21", 8888, 0, out error);
        // Disconnect: NetworkTransport.Disconnect(hostId, connectionId, out error);
        // Send data: NetworkTransport.Send(hostId, connectionId, myReiliableChannelId, buffer, bufferLength, out error);
        // If we want to receive data from the host, clients would use: NetworkTransport.ReceiveFromHost(recHostId, out connectionId, out channelId, recBuffer, bufferSize, out dataSize, out error);
    }
}
