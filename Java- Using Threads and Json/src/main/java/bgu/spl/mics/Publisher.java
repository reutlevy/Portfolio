package bgu.spl.mics;
import bgu.spl.mics.Future;
import bgu.spl.mics.application.messages.MissionReceivedEvent;
import bgu.spl.mics.application.passiveObjects.MissionInfo;

import java.util.ArrayList;
import java.util.List;

/**
 * The Publisher is an abstract class that any publisher in the system
 * must extend. The abstract Publisher class is responsible to send
 * messages to the singleton {@link MessageBroker} instance.
 * <p>
 * Derived classes of Publisher should never directly touch the MessageBroker.
 * method).
 *
 * Only private fields and methods may be added to this class.
 * <p>
 */
public abstract class Publisher extends RunnableSubPub {
    private MessageBroker messageBroker=MessageBrokerImpl.getInstance();
    private List<Future<MissionInfo>> mission;

    /**
     * @param name the Publisher name (used mainly for debugging purposes -
     *             does not have to be unique)
     */
    public Publisher(String name) {
        super(name);
        mission = new ArrayList<>();
    }

    protected void initialize() {}

    protected final <T> Future<T> sendEvent(Event<T> e) {
        return messageBroker.sendEvent(e);
    }

    protected final void sendbroadcast (Broadcast b){
        messageBroker.sendBroadcast(b);
    }

}
