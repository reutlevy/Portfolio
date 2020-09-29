package bgu.spl.net.impl.stomp.Frames;

import bgu.spl.net.impl.stomp.StompFrame;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;

public class SUBSCRIBE extends StompFrame {

    private String destination;
    private String id;
    private String receipt;
    private Boolean isError;

    public SUBSCRIBE(Map<String, String> message, String body) {
        super("SUBSCRIBE",message, body);

        this.destination = message.get("destination");
        this.id = message.get("id");
        this.receipt = message.get("receipt");
        this.isError = false;
    }

    @Override
    public String getType() {
        return "SUBSCRIBE";
    }


/*
    public String toString() {

        String answer = "SUBSCRIBE";
        Iterator it = message.entrySet().iterator();
        while (it.hasNext()) {
            ConcurrentHashMap.Entry pair = (ConcurrentHashMap.Entry) it.next();
            answer = answer + " " + pair.getKey() + ":" + pair.getValue();
            //   System.out.println(pair.getKey() + " = " + pair.getValue());
            it.remove(); // avoids a ConcurrentModificationException
        }
        return answer;
    }

 */
}