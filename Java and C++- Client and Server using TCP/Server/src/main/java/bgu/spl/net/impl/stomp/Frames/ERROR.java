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

public class ERROR extends StompFrame {

    private String receiptid;
    private String body;
    private String message;
    private Boolean isError;

    public ERROR(Map<String, String> message, String body) {
        super("ERROR",message, body);
        this.receiptid=message.get("receipt-id");
      //  this.body = message.getOrDefault("body", "");
        this.message=message.get("message");
        this.isError=false;
    }

    @Override
    public String getType(){
        return "ERROR";
    }

/*
    public String toString(){

        String answer="ERROR";
        Iterator it = messages.entrySet().iterator();
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
