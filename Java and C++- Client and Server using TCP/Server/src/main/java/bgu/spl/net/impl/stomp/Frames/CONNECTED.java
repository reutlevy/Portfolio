package bgu.spl.net.impl.stomp.Frames;

import bgu.spl.net.impl.stomp.StompFrame;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;

public class CONNECTED extends StompFrame {

    //private ConcurrentHashMap<String,String> map;
    private Boolean isError;
    private String version;

    public CONNECTED(Map<String, String> message, String body) {
        super("CONNECTED",message,body);
  //      System.out.println("the result of the headers issss "+message);
        this.version=message.get("version");
        this.isError=false;
    }

    @Override
    public String getType(){
        return "CONNECTED";
    }

/*
    public String toString(){

        String answer="CONNECTED";
        Iterator it = map.entrySet().iterator();
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
