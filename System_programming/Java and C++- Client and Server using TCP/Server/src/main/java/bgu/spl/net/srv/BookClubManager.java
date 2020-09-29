package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.StompFrame;
import bgu.spl.net.impl.stomp.User;

import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.LinkedList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class BookClubManager<T> {

    private Connectionsimpl<StompFrame> connections = null;
    private ConcurrentHashMap<String, LinkedList<Integer>> Genres;
    private ConcurrentHashMap<Integer,User> existusers;

    private BookClubManager(){
        Genres= new ConcurrentHashMap<>();
        existusers=new ConcurrentHashMap<>();
    }

    private static class Holder {
        private static BookClubManager dataBase = new BookClubManager();
    }
    public static BookClubManager getInstance() {
        return Holder.dataBase;
    }

    public ConcurrentHashMap<Integer,User> getExistusers(){
        return existusers;
    }

    public ConcurrentHashMap<String,LinkedList<Integer>> getgenres(){
        return Genres;
    }

    public void initialConnections(Connectionsimpl<StompFrame> connections) {
        if (this.connections == null)
            this.connections = connections;
    }

   /* public void joinReadingClub(User user,String genres){
        if(Genres.containsKey(genres)){
            LinkedList<Integer> users=Genres.get(genres);
            users.add(user.getConnectionId());
        }
        else {
            LinkedList<Integer> users=new LinkedList<>();
            users.add(user.getConnectionId());
            Genres.put(genres,users);
        }
    }

    public void ExitReadingClub(User user,String genre){
        if(Genres.containsKey(genre)){
            LinkedList<Integer> users=Genres.get(genre);
            if(users.contains(user.getConnectionId())){
                users.remove(user.getConnectionId());
            }
        }
    }
*/
}