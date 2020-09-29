package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.*;
import bgu.spl.mics.application.messages.*;
import bgu.spl.mics.application.passiveObjects.Diary;
import bgu.spl.mics.application.passiveObjects.MPanswer;
import bgu.spl.mics.application.passiveObjects.Report;

import java.util.List;
import java.util.concurrent.CountDownLatch;

/**
 * M handles ReadyEvent - fills a report and sends agents to mission.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class M extends Subscriber implements Runnable {

    Diary diary;
    private int tick;
    private int duration;
    private Report report;
    private List<String> Agents;
    private String Gadget;
    private int expired;
    private String missioname;
    private int M;
    private int timeissued;
    private CountDownLatch count;

    public M(int Serial, CountDownLatch count) {
        super("M" + Serial);
        this.M = Serial;
        diary = Diary.getInstance();
        this.count = count;
    }

    @Override
    protected void initialize() {

        MessageBroker messageBroker = MessageBrokerImpl.getInstance();

        Callback<TickBroadcast> callback2 = c2 -> tick = c2.getTick();
        this.subscribeBroadcast(TickBroadcast.class, callback2);

        Callback<LastTickBroadcast> callback = c -> terminate();
        this.subscribeBroadcast(LastTickBroadcast.class, callback);

        subscribeEvent(MissionReceivedEvent.class, missionReceivedEvent -> {
            diary.incrementTotal();
            Agents = missionReceivedEvent.getMissionInfo().getSerialAgentsNumbers();
            Gadget = missionReceivedEvent.getMissionInfo().getGadget();
            this.duration = missionReceivedEvent.getMissionInfo().getDuration();
            this.expired = missionReceivedEvent.getMissionInfo().getTimeExpired();
            this.timeissued = missionReceivedEvent.getMissionInfo().getTimeIssued();
            this.missioname = missionReceivedEvent.getMissionInfo().getMissionName();
            Future<Integer> future2mp = new Future<>();
            AgentsAvailableEvent ev2 = new AgentsAvailableEvent(Agents, future2mp);
            Future<MPanswer> agentfuture = getSimplePublisher().sendEvent(ev2);

            if (agentfuture != null) {
                MPanswer answer = agentfuture.get();
                if (answer.isAcuiredThem()) {
                    GadgetAvailableEvent evn = new GadgetAvailableEvent(Gadget);
                    Future<Integer> gadgetfuture = getSimplePublisher().sendEvent(evn);
                    if (gadgetfuture != null && gadgetfuture.get() > 0) {
                        if (tick < expired) {

                            future2mp.resolve(duration);

                            report = new Report();
                            report.setAgentsNames(answer.getAgentsNames());
                            report.setAgentsSerialNumbersNumber(answer.getAgentserial());
                            report.setMissionName(missioname);
                            report.setM(M);
                            report.setMoneypenny(answer.getMpID());
                            report.setGadgetName(evn.getGadgets());
                            report.setTimeIssued(timeissued);
                            report.setQTime(gadgetfuture.get());
                            report.setTimeCreated(gadgetfuture.get() + duration);
                            diary.addReport(report);


                            complete(missionReceivedEvent, null);
                            return;
                        }
                    }
                }
            }

            future2mp.resolve(-1);
            complete(missionReceivedEvent, null);
        });
        count.countDown();
    }
}



