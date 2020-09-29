package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.StompFrame;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.channels.SelectionKey;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {


    private final StompMessagingProtocol<T> protocol;
    private final MessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private static Connectionsimpl connectionsimpl;

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, StompMessagingProtocol<T> protocol) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        connectionsimpl=Connectionsimpl.getInstance();
    }

    @Override
    public void run() {

        try (Socket sock = this.sock) { //just for automatic closing
            int read;
         //   System.out.println("trying to read a message");
            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            int id = connectionsimpl.add(this);
            this.getProtocol().start(id, connectionsimpl);

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                T nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    System.out.println("------ THE INPUT OF THE SERVER IS-----");
                    System.out.println (nextMessage);
                    protocol.process(nextMessage);

                    if (protocol.shouldTerminate()) close();
                }
          //      else System.out.println("the message is null");
            }

        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(T msg) {
        try {
            System.out.println("------ THE MESSAGE OF THE SERVER IS-----");
            System.out.println (msg);
            out.write(encdec.encode(msg));
            out.flush();
            if (protocol.shouldTerminate()) close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public StompMessagingProtocol<T> getProtocol() {
        return protocol;
    }

    public MessageEncoderDecoder<T> getEncdec() {
        return encdec;
    }
}
