package bgu.spl.mics.application.publishers;


import Interfaces.TimePassedEvent;
import bgu.spl.mics.*;
import bgu.spl.mics.application.messages.LastTickBroadcast;
import bgu.spl.mics.application.messages.TickBroadcast;

import java.util.Timer;
import java.util.*;
import java.util.TimerTask;


/**
 * TimeService is the global system timer There is only one instance of this Publisher.
 * It keeps track of the amount of ticks passed since initialization and notifies
 * all other subscribers about the current time tick using {@link TickBroadcast}.
 * This class may not hold references for objects which it is not responsible for.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class TimeService extends Publisher implements Runnable {

    private int tickNumber = 1;
    private int endtick;

    public TimeService(int ticks) {
        super("TimeService");
        this.endtick = ticks;
    }

    @Override
    protected void initialize() {
    }

    @Override
    public synchronized void run() {
        try {

            while (tickNumber < endtick) {
                TickBroadcast tickBroadcast = new TickBroadcast(tickNumber++);
                getSimplePublisher().sendBroadcast(tickBroadcast);

                Thread.currentThread().sleep(100);
            }

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        LastTickBroadcast last = new LastTickBroadcast(endtick);
        this.sendbroadcast(last);
    }
}

