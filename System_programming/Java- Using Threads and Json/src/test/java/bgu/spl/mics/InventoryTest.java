package bgu.spl.mics;

import bgu.spl.mics.application.passiveObjects.Inventory;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;


public class InventoryTest {

    private Inventory inventory;
    private Inventory inventory1;

    @BeforeEach
    public void setUp(){
        inventory=Inventory.getInstance();
        inventory1=Inventory.getInstance();
        String[] add=new String[]{"Reut","Daniel"};
        inventory.load(add);
        String[] add1=new String[]{"H1","R2"};
        inventory1.load(add1);
    }

    @Test
    public void test(){
    }

    @Test
    void getInstance() {
        assertNotNull(inventory1);
        Object o1=inventory.getInstance();
        Object o2=inventory1.getInstance();
        assertEquals(o1,o2);
    }

    @Test
    void load() {
        assertTrue(inventory.getItem("Reut"));
        assertFalse(inventory.getItem("Hadas"));

        assertTrue(inventory1.getItem("H1"));
        assertFalse(inventory1.getItem("R"));
    }

    @Test
    void getItem() {
        assertTrue(inventory.getItem("Reut"));
        assertTrue(inventory.getItem("Daniel"));
        assertFalse(inventory1.getItem("Jerry"));
        assertTrue(inventory1.getItem("H1"));
        assertTrue(inventory1.getItem("R2"));
        assertFalse(inventory.getItem("Hadas"));
    }

}
