package bgu.spl.mics;

import java.util.concurrent.*;


/**
 * The {@link MessageBrokerImpl class is the implementation of the MessageBroker interface.
 * Write your implementation here!
 * Only private fields and methods can be added to this class.
 */
public class MessageBrokerImpl implements MessageBroker {
    private static class Holder {
        private static MessageBrokerImpl messageBroker = new MessageBrokerImpl();
    }

    private ConcurrentHashMap<Class<? extends Message>, ConcurrentLinkedQueue<Subscriber>> msgMap;
    private ConcurrentHashMap<Subscriber, LinkedBlockingQueue<Message>> SubMap;
    private ConcurrentHashMap<Event, Future> Futuremap;

    private MessageBrokerImpl() {
        msgMap = new ConcurrentHashMap<>();
        SubMap = new ConcurrentHashMap<>();
        Futuremap = new ConcurrentHashMap<>();
    }

    public static MessageBroker getInstance() {
        return Holder.messageBroker;
    }

    @Override
    public <T> void subscribeEvent(Class<? extends Event<T>> type, Subscriber m) {
        msgMap.putIfAbsent(type, new ConcurrentLinkedQueue<>());
        msgMap.get(type).add(m);
    }

    @Override
    public void subscribeBroadcast(Class<? extends Broadcast> type, Subscriber m) {
        msgMap.putIfAbsent(type, new ConcurrentLinkedQueue<>());
        msgMap.get(type).add(m);
    }

    @Override
    public <T> void complete(Event<T> e, T result) {
        Futuremap.get(e).resolve(result);
    }

    @Override
    public void sendBroadcast(Broadcast b) {
        for (Subscriber sub : msgMap.get(b.getClass())) {
            LinkedBlockingQueue<Message> q = SubMap.get(sub);
            if (q != null) {
                q.add(b);
            }
        }
    }

    @Override
    public <T> Future<T> sendEvent(Event<T> e) {
        Future<T> future = new Future<>();
        ConcurrentLinkedQueue<Subscriber> subscribers = msgMap.get(e.getClass());
        Subscriber subscriber;
        if (subscribers == null) {
            return null;
        }
        synchronized (subscribers) {
            subscriber = subscribers.poll();
            if (subscriber == null) {
                return null;
            }
            subscribers.add(subscriber);
        }

        Futuremap.put(e, future);
        synchronized (subscriber) {
            LinkedBlockingQueue<Message> messages = SubMap.get(subscriber);
            if (messages == null) {
                return null;
            }
            messages.add(e);
        }
        return future;
    }

    @Override
    public void register(Subscriber m) {
        LinkedBlockingQueue<Message> queue = new LinkedBlockingQueue<>();
        SubMap.put(m, queue);
    }

    @Override
    public void unregister(Subscriber subscriber) {
        if (subscriber == null) {
            return;
        }
        for (ConcurrentLinkedQueue<Subscriber> subscribers : msgMap.values()) {
            synchronized (subscribers) {
                subscribers.remove(subscriber);
            }
        }
        synchronized (subscriber) {
            for (Message message : SubMap.get(subscriber)) {
                if (Futuremap.containsKey(message)) {
                    Futuremap.get(message).resolve(null);
                    Futuremap.remove(message);
                }
            }
            SubMap.remove(subscriber);
        }
    }


    @Override
    public Message awaitMessage(Subscriber m) throws InterruptedException {
        LinkedBlockingQueue<Message> messages = SubMap.get(m);
        return messages.take();
    }
}