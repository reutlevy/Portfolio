package bgu.spl.net.impl.stomp.Frames;

import bgu.spl.net.impl.stomp.StompFrame;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;

public class UNSUBSCRIBE extends StompFrame {

    private String id;
    private Boolean isError;

    public UNSUBSCRIBE(Map<String,String> message, String body){
        super("UNSUBSCRIBE",message, body);
        this.id=message.get("id");
        this.isError=false;
    }

    @Override
    public String getType(){
        return "SUBSCRIBE";
    }


/*
    public String toString() {

        String answer = "UNSUBSCRIBE";
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
