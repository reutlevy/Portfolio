package bgu.spl.mics;

import bgu.spl.mics.application.subscribers.M;
import bgu.spl.mics.application.subscribers.Moneypenny;
import bgu.spl.mics.application.subscribers.Q;
import bgu.spl.mics.example.messages.ExampleBroadcast;
import bgu.spl.mics.example.messages.ExampleEvent;
import bgu.spl.mics.example.subscribers.ExampleEventHandlerSubscriber;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.concurrent.CountDownLatch;

import static org.junit.jupiter.api.Assertions.*;

public class MessageBrokerTest {

    private MessageBroker m1;
    private MessageBroker m2;
    private Subscriber q;
    private Event event1;
    private Event event2;
    private ExampleBroadcast broadcast;
    private Subscriber moneypenny;
    private Subscriber m;

    @BeforeEach
    public void setUp(){
       m1=MessageBrokerImpl.getInstance();
       m2=MessageBrokerImpl.getInstance();
       event1=new ExampleEvent("event1");
       broadcast=new ExampleBroadcast("100");
       q=new Q(new CountDownLatch(1));
       moneypenny=new Moneypenny(2,new CountDownLatch(1));
       event2=new ExampleEvent("event2");
       m=new M(2,new CountDownLatch(1));
       m1.register(q);
       m1.register(moneypenny);
       m1.register(m);

    }


   @Test
   void getInstance() {
       assertNotNull(m1);
       assertEquals(m1,m2);
   }

    @Test
    void subscribeBroadcast(){
        Message message1= null;

        try {
            m1.subscribeBroadcast(ExampleBroadcast.class,q);
            m1.sendBroadcast(broadcast);
            message1 = m1.awaitMessage(q);
        }

        catch (Exception e){
            fail("The test had failed"+ e);
        }
        assertNotNull(message1);
    }

  /*  @Test
    void subscribeEvent()  {
        m1.subscribeEvent(ExampleEvent.class,q);
        m1.sendEvent(event1);
        Message message1=null;

        try {
            message1 = m1.awaitMessage(q);
        }
        catch (Exception e){
            fail("The test had failed"+ e);
        }
        assertNotNull(message1);
    } */



    @Test
    void Register()  {
        Q q1=new Q(new CountDownLatch(1));
        m1.register(q1);
        m1.subscribeBroadcast(ExampleBroadcast.class,q1);
        m1.sendBroadcast(broadcast);
        Message message1= null;

        try{
            message1=m1.awaitMessage(q1);
            assertNotNull(message1);
        }catch (Exception e){
            fail("Exception " + e);
        }
    }

  /*  @Test
    void testUnregister(){

        Message message1=null;
        try {

            m1.sendEvent(event1);
            m1.unregister(q);
            message1=m1.awaitMessage(q);
            assertNull(message1);

        } catch (Exception e){
            fail("Unexpected Exception: ");
        }
    } */

    @Test
    void AwaitMessage()  {
        m1.subscribeBroadcast(ExampleBroadcast.class,q);
        m1.sendBroadcast(broadcast);

        try {
            assertNotNull(m1.awaitMessage(q));
        }
        catch (Exception e){
            fail("Exception "+e);
        }
    }

    @Test
    void sendEvent()  {
        m1.subscribeEvent(ExampleEvent.class,q);
        m1.sendEvent(event1);
        Message message1=null;

        try {
           message1 = m1.awaitMessage(q);
        }
        catch (Exception e){
            fail("The test had failed"+ e);
        }
        assertNotNull(message1);
    }

    @Test
    void sendBroadcast()  {
        Message message1= null;

        try {
            m1.subscribeBroadcast(ExampleBroadcast.class,moneypenny);
            m1.sendBroadcast(broadcast);
            message1 = m1.awaitMessage(moneypenny);
        }

        catch (Exception e){
            fail("The test had failed"+ e);
        }
        assertNotNull(message1);
    }

}
