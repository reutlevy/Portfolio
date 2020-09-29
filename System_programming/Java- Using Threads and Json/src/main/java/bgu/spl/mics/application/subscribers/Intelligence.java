package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.*;
import bgu.spl.mics.application.messages.LastTickBroadcast;
import bgu.spl.mics.application.messages.MissionReceivedEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.MissionInfo;

import java.util.*;
import java.util.concurrent.CountDownLatch;

import static java.util.Comparator.comparing;


/**
 * A Publisher only.
 * Holds a list of Info objects and sends them
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Intelligence extends Subscriber {

    private List<MissionInfo> missionInfos = new ArrayList<>(); //list order by
    private int tick;
    private MissionInfo missionInfo;
    private LinkedList<MissionInfo> missionInfoList;
    private int serial;
    private CountDownLatch count;

    public Intelligence(int serial, List<MissionInfo> missionInfo, CountDownLatch count) {
        super("Intelligence"+serial);
        this.missionInfos = missionInfo;
        Collections.sort(missionInfos, comparing(MissionInfo::getTimeIssued));
        this.serial = serial;
        this.count = count;
        missionInfoList = new LinkedList<>(missionInfo);
        missionInfoList.sort(Comparator.comparingInt(MissionInfo::getTimeIssued));
    }

    @Override
    protected void initialize() {


        Callback<LastTickBroadcast> callback1 = c -> terminate();
        this.subscribeBroadcast(LastTickBroadcast.class, callback1);

        subscribeBroadcast(TickBroadcast.class, ev -> {
            tick = ev.getTick();
            while (!missionInfoList.isEmpty() && missionInfoList.getFirst().getTimeIssued() <= tick) {
                missionInfo = missionInfoList.removeFirst();
                MissionReceivedEvent missionReceivedEvent = new MissionReceivedEvent(missionInfo);
                getSimplePublisher().sendEvent(missionReceivedEvent);
            }
        });
        count.countDown();
    }
}


