package bgu.spl.net.impl.stomp.Frames;

import bgu.spl.net.impl.stomp.StompFrame;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;

public class RECEIPT extends StompFrame {

    private String receiptid;
    private Boolean isError;

    public RECEIPT(Map<String, String> message, String body) {
        super("RECEIPT",message,body);
        this.receiptid=message.get("receipt-id");
        this.isError=false;
    }

    @Override
    public String getType(){
        return "RECEIPT";
    }


/*
    public String toString(){

        String answer="RECEIPT";
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
