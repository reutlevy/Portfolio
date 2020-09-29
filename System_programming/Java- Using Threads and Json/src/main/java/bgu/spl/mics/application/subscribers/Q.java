package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.GadgetAvailableEvent;
import bgu.spl.mics.application.messages.LastTickBroadcast;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.Inventory;

import java.util.concurrent.CountDownLatch;

/**
 * Q is the only Subscriber\Publisher that has access to the {@link bgu.spl.mics.application.passiveObjects.Inventory}.
 *
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Q extends Subscriber implements Runnable {

	private int tick;
	private Inventory inventory;
    private CountDownLatch count;

	public Q(CountDownLatch count) {
		super("Q");
		inventory=Inventory.getInstance();
		this.count=count;
	}

	@Override
	protected void initialize() {

		this.subscribeBroadcast(TickBroadcast.class,c2 -> {tick = c2.getTick();});

		this.subscribeBroadcast(LastTickBroadcast.class,c -> terminate());

		subscribeEvent(GadgetAvailableEvent.class, (GadgetAvailableEvent ev) -> {
			boolean isGadgetAvail=inventory.getItem(ev.getGadgets());
			int result = isGadgetAvail ? tick : -1;
			complete(ev,result);
		});

		count.countDown();
	}
}
