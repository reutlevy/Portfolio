package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.StompFrame;

import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;

public interface Connections<T> {

    ConcurrentHashMap<Integer, ConnectionHandler<T>> getClients();

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);
}
