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

public class DISCONNECT extends StompFrame {

    private String receipt;
    private ConcurrentHashMap<String,String> message;
    private Boolean isError;

    public DISCONNECT(Map<String, String> message, String body) {
        super("DISCONNECT",message,body);

        this.receipt=message.get("receipt");
        this.isError=false;
    }

    @Override
    public String getType(){
        return "DISCONNECT";
    }

/*
    public String toString(){

        String answer="DISCONNECTED";
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
