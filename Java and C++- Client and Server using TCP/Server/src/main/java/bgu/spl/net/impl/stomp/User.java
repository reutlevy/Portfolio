package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.LinkedList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;

public class User {

    private String userName;
    private String password;
    private Boolean isConnect;
    private ConcurrentHashMap<String,String> genres; //subscription id and genre
    private Integer connectionId;

    public User(String userName, String password, Boolean isConnect, ConcurrentHashMap<String,String> genres, Integer connectionId){
        this.userName=userName;
        this.password=password;
        this.isConnect=isConnect;
        this.genres=genres;
        this.connectionId=connectionId;
    }

    public String getUserName() {
        return userName;
    }

    public String getPassword() {
        return password;
    }

    public Boolean getConnect() {
        return isConnect;
    }

    public ConcurrentHashMap<String,String> getGenres() {
        return genres;
    }

    public Integer getConnectionId() {
        return connectionId;
    }

    public void setConnect(Boolean connect) {
        isConnect = connect;
    }

    public void setConnectionId(Integer connectionId) {
        this.connectionId = connectionId;
    }

    public void setGenres(ConcurrentHashMap<String, String> genres) {
        this.genres = genres;
    }

}
