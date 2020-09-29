package bgu.spl.mics;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

import static org.junit.jupiter.api.Assertions.*;

public class FutureTest {
    Future<Integer> test1;
    Future<Integer> test2;
    Future<Integer> test3;
    Future<Integer> test4;
    Future<Integer> test5;

    @BeforeEach
    public void setUp(){
        test1 = new Future<Integer>();
        test2 = new Future<Integer>();
        test3 = new Future<Integer>();
        test4 = new Future<Integer>();
        test5 = new Future<Integer>();
    }

    @Test
    void get() {
        test1.resolve(5);
        assertEquals(test1.get(),5);

        test2.resolve(8);
        test2.resolve(10);
        assertEquals(test2.get(),10);

        test4.resolve(null);
        assertNull(test4.get());

        test5.resolve(2);
        test5.resolve(null);
        assertNull(test5.get());
    }

    @Test
    void resolve() {
        test1.resolve(5);
        assertEquals(test1.get(),5);

        test4.resolve(null);
        assertNull(test4.get());

    }

    @Test
    void isDone() {
        test1.resolve(5);
        assertTrue(test1.isDone());

        test2.resolve(8);
        test2.resolve(10);
        assertTrue(test2.isDone());

        assertFalse(test3.isDone());
    }

    @Test
    void testGet() throws InterruptedException {
        test1.resolve(5);
        TimeUnit unit1 = TimeUnit.SECONDS;
        long timeout1 = 5;
        assertEquals(test1.get(timeout1,unit1),5);

    }
}