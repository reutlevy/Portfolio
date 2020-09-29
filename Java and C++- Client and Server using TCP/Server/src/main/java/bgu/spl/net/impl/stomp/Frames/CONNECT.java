package bgu.spl.net.impl.stomp.Frames;

import bgu.spl.net.impl.stomp.StompFrame;
import bgu.spl.net.srv.BookClubManager;
import bgu.spl.net.srv.Connectionsimpl;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;

public class CONNECT extends StompFrame {

    private String acceptversion;
    private String host;
    private String login;
    private String passcode;
   private ConcurrentHashMap<String,String> message;
    private String body;
    private Boolean isError;
    private String receipt;

    public CONNECT(Map<String, String> message, String body) {
        super("CONNECT",message,body);

    //    this.body = message.getOrDefault("body", "");
        this.acceptversion=message.get("accept-version");
        this.host=message.get("host");
        this.login=message.get("login");
        this.passcode=message.get("passcode");
        this.receipt=message.get("receipt");

        this.isError=false;
    }

    @Override
    public String getType(){
        return "CONNECT";
    }

    public String getAcceptversion() {
        return acceptversion;
    }

    public String getHost() {
        return host;
    }

    public String getLogin() {
        return login;
    }

    public String getPasscode() {
        return passcode;
    }

    public String getBody() {
        return body;
    }

}
