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

public class SEND extends StompFrame {

    private String destination;
    private String body;
    private Boolean isError;

    public SEND(Map<String, String> message, String body) {
        super("SEND",message, body);

   //     this.body = message.getOrDefault("body", "");
        this.destination=message.get("destination");
        this.isError=false;
    }

    @Override
    public String getType(){
        return "SEND";
    }


/*
    public String toString(){

        String answer="SEND";
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
