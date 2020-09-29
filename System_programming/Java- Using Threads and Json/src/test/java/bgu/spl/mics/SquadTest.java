package bgu.spl.mics;

import bgu.spl.mics.application.passiveObjects.Agent;
import bgu.spl.mics.application.passiveObjects.Squad;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.lang.reflect.Array;
import java.util.*;
import static org.junit.jupiter.api.Assertions.*;

public class SquadTest {
    private Agent agent1;
    private Agent agent2;
    private Agent agent3;
    private Agent agent4;
    private Agent agent5;
    private Agent agent6;
    private Agent agent7;
    private Agent agent8;
    private Squad s1;
    private Squad s2;

    @BeforeEach


    public void setUp(){
        s1=Squad.getInstance();
        s2=Squad.getInstance();
        agent1=new Agent("007","Hadas");
        agent2=new Agent("00","Roei");
        agent3=new Agent("006","Reut");
        agent4=new Agent("004","Dani");
        Agent[] agents=new Agent[]{agent1,agent2,agent3,agent4};
        s1.load(agents);
        agent5=new Agent("001","Nurit");
        agent6=new Agent("002","Reut");
        agent7=new Agent("003","Kaplan");
        agent8=new Agent("009","Daniel");
        Agent[] agents1=new Agent[]{agent5,agent6,agent7,agent8};
        s2.load(agents1);
    }

    @Test
    void getInstance() {
        assertNotNull(s1);
        Object o1=s1.getInstance();
        Object o2=s2.getInstance();
        assertEquals(o1,o2);
    }

    @Test
    void load() {
        List<String> serials=new ArrayList<>();
        serials.add("004");
        serials.add("006");
        assertTrue(s1.getAgents(serials));


        List<String> serials1=new ArrayList<>();
        serials1.add("001");
        serials1.add("002");
        serials1.add("011");
        assertFalse(s2.getAgents(serials1));
    }

    @Test
    void releaseAgents() {
        List<String> serials=new ArrayList<>();
        serials.add("007");
        serials.add("006");
        agent3.acquire();
        agent1.acquire();
        agent2.acquire();
        s1.releaseAgents(serials);
        assertTrue(agent1.isAvailable());
        assertTrue(agent3.isAvailable());
        assertFalse(agent2.isAvailable());
    }

    @Test
    void sendAgents() {
        List<String> serials=new ArrayList<>();
        serials.add("001");
        serials.add("002");
        agent5.acquire();
        agent6.acquire();
        agent7.acquire();
        s2.releaseAgents(serials);
        assertTrue(agent5.isAvailable());
        assertTrue(agent6.isAvailable());
        assertFalse(agent7.isAvailable());
    }

    @Test
    void getAgents() {
        List<String> serials=new ArrayList<>();
        serials.add("006");
        serials.add("00");
        assertTrue(s1.getAgents(serials));
        serials.add("008");
        assertFalse(s1.getAgents(serials));
        List<String> serials1=new ArrayList<>();
        serials1.add("007");
        serials1.add("003");
        assertTrue(s2.getAgents(serials1));
        serials1.add("030");
        assertFalse(s2.getAgents(serials1));
    }

    @Test
    void getAgentsNames() {
       List<String> check1=new ArrayList<>();
       check1.add("007");
       check1.add("00");
       check1.add("020");
       List<String> compare= Arrays.asList("Hadas","Roei","Rubin");
       assertNull(s1.getAgentsNames(check1));
        List<String> compare1= Arrays.asList("Hadas","Roei","Reut");
        check1.remove("020");
        check1.add("006");
       assertTrue(s1.getAgentsNames(check1).equals(compare1));
    }
}
