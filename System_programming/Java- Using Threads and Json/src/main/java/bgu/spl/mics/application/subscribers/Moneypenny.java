package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Callback;
import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.*;
import bgu.spl.mics.application.passiveObjects.MPanswer;
import bgu.spl.mics.application.passiveObjects.Squad;

import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Only this type of Subscriber can access the squad.
 * Three are several Moneypenny-instances - each of them holds a unique serial number that will later be printed on the report.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Moneypenny extends Subscriber {

    private int SerialNumber;
    private Squad squad;
    private CountDownLatch count;

    public Moneypenny(int SerialNumber, CountDownLatch count) {
        super("MoneyPenny"+SerialNumber);
        this.SerialNumber = SerialNumber;
        squad = Squad.getInstance();
        this.count = count;
    }

    @Override
    protected void initialize() {

        Callback<LastTickBroadcast> callback2 = c -> terminate();
        this.subscribeBroadcast(LastTickBroadcast.class, callback2);

        subscribeEvent(AgentsAvailableEvent.class, ev -> {
            boolean Available = squad.getAgents(ev.getSerials());
			List<String> names=null;
            List<String> serial=null;
            if(Available) {
                serial=ev.getSerials();
                names = squad.getAgentsNames(ev.getSerials());
            }
            MPanswer answer=new MPanswer(Available,SerialNumber,names,serial);
            complete(ev, answer);

            if (Available) {
                int time = ev.getFuture4mp().get();
                if(time>0)
                	squad.sendAgents(ev.getSerials(),time);
				else
					squad.releaseAgents(ev.getSerials());
            }
        });
        count.countDown();
    }
}
