package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.impl.stomp.Frames.*;

import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;


public class StompEncoderDecoder implements MessageEncoderDecoder<StompFrame> {
    private byte[] bytes = new byte[1 << 10]; //start with 1k
    private int len = 0;

    @Override
    public StompFrame decodeNextByte(byte nextByte) {
        //notice that the top 128 ascii characters have the same representation as their utf-8 counterparts
        //this allow us to do the following comparison
        if (nextByte == '\u0000') {
            //System.out.println("end of bytes!!!!" + popString());
            return popString();
        }
        pushByte(nextByte);
        return null; //not a line yet
    }

    @Override
    public byte[] encode(StompFrame message) {
        return (message.toString()).getBytes(); //TODO find out about ^@
    }

    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }

        bytes[len++] = nextByte;
    }

    private StompFrame popString() {
        String translate = new String(bytes, 0, len, StandardCharsets.UTF_8);
        StompFrame result = null;
        String body = "";
        len = 0;
        String[] lines = translate.split("\n");


        Map<String, String> message1 = new LinkedHashMap<>();
        for (int i = 1; i < lines.length; i++) {
            if ((lines[i].contains(":") & (!lines[i].isEmpty()))) {
                String[] header = lines[i].split(":");
                message1.put(header[0], header[1]);
                System.out.println(header[0] + header[1]);
            } else {
                if (!lines[i].isEmpty()) {
                    body = lines[i];
                }
            }
        }

            Map<String, String> message = new LinkedHashMap<>(message1);
            int size0 = message.size();
            switch (lines[0]) {
                case "MESSAGE":
                    result = new MESSAGE(message, body);
                    break;
                case "CONNECT":
                    result = new CONNECT(message, body);
                    break;
                case "DISCONNECT":
                    result = new DISCONNECT(message, body);
                    break;
                case "ERROR":
                    result = new ERROR(message, body);
                    break;
                case "RECEIPT":
                    result = new RECEIPT(message, body);
                    break;
                case "SEND":
                    result = new SEND(message, body);
                    break;
                case "SUBSCRIBE":
                    result = new SUBSCRIBE(message, body);
                    //   System.out.println("----- recognize subscribe");
                    break;
                case "UNSUBSCRIBE":
                    result = new UNSUBSCRIBE(message, body);
                    break;
                default:
                    return null;
            }
            int size1 = message.size();
            result.setHeaders(message1);
            int size2 = message.size();
            int size3 = result.headers.size();
            return result;
    }


    //region debug
    String b2s(byte b, boolean spaces) {
        byte[] bytes = {b};
        String s = new String(bytes);
        if (spaces) {
            s = " " + s;
            if (b > 99)
                s = " " + s;
        }
        switch (b) {
            case '\n':
                return "\\n";
            case '\r':
                return "\\r";
            case '\0':
                return "\\0";
            default:
                return s;
        }
    }

    //endregion
}
