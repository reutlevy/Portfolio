package bgu.spl.net.impl.stomp.Frames;

import bgu.spl.net.impl.stomp.StompFrame;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;

public class MESSAGE extends StompFrame {

    private String subscription;
    private String MessageId;
    private String destination;
    private String body;
    private Boolean isError;

    public MESSAGE(Map<String, String> message, String body) {
        super("MESSAGE",message, body);
        this.subscription=message.get("subscription");
        this.MessageId=message.get("Message-id");
        this.destination=message.get("destination");
   //     this.body = message.getOrDefault("body", "");
        this.isError=false;
    }

    @Override
    public String getType(){
        return "MESSAGE";
    }


/*
    public String toString(){

        String answer="ERROR";
        Iterator it = message.entrySet().iterator();
        while (it.hasNext()) {
            ConcurrentHashMap.Entry pair = (ConcurrentHashMap.Entry)it.next();
            answer=answer+" "+pair.getKey() + ":" + pair.getValue();
            //   System.out.println(pair.getKey() + " = " + pair.getValue());
            it.remove(); // avoids a ConcurrentModificationException
        }
        return answer;
    }

 */
}