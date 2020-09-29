package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.newsfeed.FetchNewsCommand;
import bgu.spl.net.impl.newsfeed.NewsFeed;
import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.impl.rci.RCIClient;
import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {

        if (args[0].equals("tpc")) {
            Server<StompFrame> threadPerClient = Server.threadPerClient(
                    7777,
                    () -> new StompProtocol(),
                    () -> new StompEncoderDecoder());
            threadPerClient.serve();

        } else if (args[0].equals("reactor")) { //Reactor
            Server<StompFrame> reactor = Server.reactor(
                    2,
                    7777,
                    () -> new StompProtocol(),
                    () -> new StompEncoderDecoder());
            reactor.serve();

        } else {
            System.out.println("wrong args[0]");
        }
    }
}
