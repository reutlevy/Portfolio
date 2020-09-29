package bgu.spl.mics.application.messages;

import bgu.spl.mics.Event;
import bgu.spl.mics.Future;
import bgu.spl.mics.application.passiveObjects.MPanswer;

import java.util.List;

public class AgentsAvailableEvent implements Event<MPanswer> {

    private List<String> serials;
    private Future<Integer> future4mp;

    public AgentsAvailableEvent(List<String> agents,Future<Integer> future4mp){
        this.serials=agents;
        this.future4mp=future4mp;
    }

    public Future<Integer> getFuture4mp() {
        return future4mp;
    }

    public List<String> getSerials(){
        return serials;
    }

}