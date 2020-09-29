package bgu.spl.net.impl.stomp;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;

public abstract class StompFrame {

    private String type;
    protected Map<String, String> headers;
    private String body = "";

    public StompFrame(String type1,Map<String, String> headers1, String body){

        this.type=type1;
        this.headers = new LinkedHashMap<>();
        for(String key : headers1.keySet())
        {
            this.headers.put(key, headers1.get(key));
        }
        this.body=body;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String newbody){
        if(newbody!=null)
        this.body=newbody;
    }

    public void setHeaders(Map<String, String> headers1)
    {
        this.headers = new LinkedHashMap<>();
        for(String key : headers1.keySet())
        {
            this.headers.put(key, headers1.get(key));
        }
;
    }

    public String getType() {
        return type;
    }

    public Map<String, String> getHashMap() {
        return headers;
    }


    @Override
    public String toString() {
        String answer = this.type + "\n";
        for (String key : headers.keySet()) {
            answer += key + ":" + headers.get(key) + "\n";
        }
        answer += '\n';
        answer += this.getBody() + "\u0000";
        return answer;
    }
}
