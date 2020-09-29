package bgu.spl.mics.application;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.*;
import java.util.concurrent.*;

import bgu.spl.mics.Publisher;
import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.passiveObjects.*;
import com.google.gson.*;
import bgu.spl.mics.application.subscribers.*;
import bgu.spl.mics.application.publishers.*;
import com.google.gson.stream.JsonReader;

/**
 * This is the Main class of the application. You should parse the input file,
 * create the different instances of the objects, and run the system.
 * In the end, you should output serialized objects.
 */
public class MI6Runner {
    private static Subscriber[] subscribers;

    public static void main(String[] args) {

        Thread Timeservice = null;
        List<Thread> serviceList = new ArrayList<>();
        Gson gson = new Gson();
        JsonParser parser = new JsonParser();
        int numofM = 0;
        int numofMoneyPenny = 0;
        int numOfIntel = 0;

        try {
            JsonReader reader = new JsonReader(new FileReader(args[0]));
            JsonObject jsonObject = gson.fromJson(reader, JsonObject.class);

            final JsonArray inventoryarray = jsonObject.get("inventory").getAsJsonArray();
            String[] uploadinventory = new String[inventoryarray.size()];
            for (int i = 0; i < inventoryarray.size(); i++) {
                uploadinventory[i] = inventoryarray.get(i).getAsString();
            }
            Inventory.getInstance().load(uploadinventory);

            final JsonArray squadarray = jsonObject.get("squad").getAsJsonArray();
            final Agent[] agents = new Agent[squadarray.size()];
            for (int i = 0; i < squadarray.size(); i++) {
                final JsonObject agent = squadarray.get(i).getAsJsonObject();
                agents[i] = new Agent(agent.get("serialNumber").getAsString(), agent.get("name").getAsString());
            }
            Squad.getInstance().load(agents);

            final JsonObject Subscribers = jsonObject.get("services").getAsJsonObject();

            JsonObject resources = jsonObject.getAsJsonObject("services");
            numOfIntel = resources.getAsJsonArray("intelligence").size();
            final JsonArray intelligence = resources.getAsJsonArray("intelligence");
            List<List<MissionInfo>> missions = new ArrayList<>();
            for (int t = 0; t < numOfIntel; t++) { //number of intel
                final JsonObject jsonmission = intelligence.get(t).getAsJsonObject();
                final JsonArray mission = jsonmission.getAsJsonArray("missions");
                List<MissionInfo> uploadmissions = new ArrayList<>();
                for (int k = 0; k < mission.size(); k++) { //number of missions of eachone
                    final JsonObject onemission = mission.get(k).getAsJsonObject();
                    final JsonArray agentarray = onemission.getAsJsonArray("serialAgentsNumbers");
                    List<String> agentlist = new ArrayList<>();
                    for (int x = 0; x < agentarray.size(); x++) {
                        agentlist.add(agentarray.get(x).getAsString());
                    }
                    MissionInfo missionInfo = new MissionInfo(onemission.get("name").getAsString(), agentlist, onemission.get("gadget").getAsString(), onemission.get("timeExpired").getAsInt(), onemission.get("timeIssued").getAsInt(), onemission.get("duration").getAsInt());
                    uploadmissions.add(missionInfo);
                }
                missions.add(uploadmissions);
            }


            numofM = Subscribers.get("M").getAsInt();
            numofMoneyPenny = Subscribers.get("Moneypenny").getAsInt();
            List<Subscriber> subscribers = new LinkedList<>();
            CountDownLatch countDownLatch = new CountDownLatch(numofM + numofMoneyPenny + 1 + numOfIntel);

            for (int i = 0; i < numOfIntel; i++) {
                subscribers.add(new Intelligence(i + 1, missions.get(i), countDownLatch));
            }
            for (int i = 0; i < numofM; i++) {
                subscribers.add(new M(i + 1, countDownLatch));
            }

            for (int i = 0; i < numofMoneyPenny; i++) {
                subscribers.add(new Moneypenny(i + 1, countDownLatch));
            }

            subscribers.add(new Q(countDownLatch));

            List<Thread> threads=new LinkedList<>();
            for (Subscriber subscriber : subscribers) {
                threads.add(new Thread(subscriber, subscriber.getName()));
            }
            for (Thread thread : threads) {
                thread.start();
            }
            countDownLatch.await();

            Publisher timeService = new TimeService(Subscribers.get("time").getAsInt());
            Thread timer=new Thread(timeService,"MyClock");
            timer.start();

            timer.join();
            for (Thread thread : threads) {
                thread.join();
            }
            Inventory.getInstance().printToFile(args[1]);
            Diary.getInstance().printToFile(args[2]);

        } catch (Exception e) {
        }
    }
}