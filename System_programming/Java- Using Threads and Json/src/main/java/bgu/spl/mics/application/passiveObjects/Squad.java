package bgu.spl.mics.application.passiveObjects;

import java.util.*;

/**
 * Passive data-object representing a information about an agent in MI6.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You may add ONLY private fields and methods to this class.
 */
public class Squad {

    private Map<String, Agent> agents;

    private static class Holder {
        private static Squad squad = new Squad();
    }

    private Squad() {
        agents = new HashMap<>();
    }

    public static Squad getInstance() {
        return Holder.squad;
    }

    /**
     * Initializes the squad. This method adds all the agents to the squad.
     * <p>
     *
     * @param agents Data structure containing all data necessary for initialization
     *               of the squad.
     */
    public void load(Agent[] agents) {
        for (int i = 0; i < agents.length; i++) {
            this.agents.put(agents[i].getSerialNumber(), agents[i]);
        }
    }

    /**
     * Releases agents.
     */
    public void releaseAgents(List<String> serials) {
        for (String serial : serials) {
            agents.get(serial).release();
            synchronized (agents.get(serial)) {
                agents.get(serial).notifyAll();
            }
        }
    }

    /**
     * simulates executing a mission by calling sleep.
     *
     * @param time milliseconds to sleep
     */
    public void sendAgents(List<String> serials, int time) {
        try {
            Thread.currentThread().sleep(100 * (long) time);
            releaseAgents(serials);
        } catch (InterruptedException ignored) {
        }
    }

    /**
     * acquires an agent, i.e. holds the agent until the caller is done with it
     *
     * @param serials the serial numbers of the agents
     * @return ‘false’ if an agent of serialNumber ‘serial’ is missing, and ‘true’ otherwise
     */
    public boolean getAgents(List<String> serials) {
        boolean result = true;
        for (int i = 0; i < serials.size() & result; i++) {
            result = agents.containsKey(serials.get(i));
        }

        try {
            if (result) {
                serials.sort(Comparator.naturalOrder());
                for (String serial : serials) {
                    Agent agent = agents.get(serial);
                    synchronized (agent) {
                        while (!agent.available) {
                            agent.wait();
                        }
                        agent.acquire();
                    }
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return result;
    }

    /**
     * gets the agents names
     *
     * @param serials the serial numbers of the agents
     * @return a list of the names of the agents with the specified serials.
     */
    public List<String> getAgentsNames(List<String> serials) {
        for (int i = 0; i < serials.size(); i++) {
            if (!agents.containsKey(serials.get(i)))
                return null;
        }
        List<String> result = new ArrayList<>();
        for (int i = 0; i < serials.size(); i++) {
            result.add(agents.get(serials.get(i)).getName());
        }
        return result;
    }
}
