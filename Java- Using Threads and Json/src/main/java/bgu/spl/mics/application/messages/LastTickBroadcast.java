package bgu.spl.mics.application.messages;

import bgu.spl.mics.Broadcast;

public class LastTickBroadcast implements Broadcast {

    private int tick;

    public LastTickBroadcast(int lasttick){
        this.tick=lasttick;
    }

    public int getTick() {
        return tick;
    }

    public void setTick(int tick){
        this.tick=tick;
    }
}
