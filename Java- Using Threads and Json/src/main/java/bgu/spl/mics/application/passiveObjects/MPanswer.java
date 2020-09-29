package bgu.spl.mics.application.passiveObjects;

import java.util.List;

public class MPanswer {
    private boolean acuiredThem;
    private int mpID;
    private List<String> agentsNames;
    private List<String> agentserial;

    public MPanswer(boolean acuiredThem, int mpID, List<String> agentsNames, List<String> agentserial) {
        this.acuiredThem = acuiredThem;
        this.mpID = mpID;
        this.agentsNames = agentsNames;
        this.agentserial=agentserial;
    }

    public boolean isAcuiredThem() {
        return acuiredThem;
    }

    public int getMpID() {
        return mpID;
    }

    public List<String> getAgentsNames() {
        return agentsNames;
    }

    public List<String> getAgentserial(){
        return agentserial;
    }
}
