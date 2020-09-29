package bgu.spl.mics.application.passiveObjects;

import com.google.gson.GsonBuilder;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.List;

import com.google.gson.*;

/**
 * Passive object representing the diary where all reports are stored.
 * <p>
 * This class must be implemented safely as a thread-safe singleton.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You can add ONLY private fields and methods to this class as you see fit.
 */
public class Diary {
    private List<Report> reports;
    int total;

    private static class Holder {
        private static Diary diary = new Diary();
    }

    /**
     * Retrieves the single instance of this class.
     */
    private Diary() {
        reports = new ArrayList<>();
        total = 0;
    }

    public static Diary getInstance() {
        return Holder.diary;
    }

    public List<Report> getReports() {
        return reports;
    }

    /**
     * adds a report to the diary
     *
     * @param reportToAdd - the report to add
     */
    public void addReport(Report reportToAdd) {
        synchronized (reports) {
            reports.add(reportToAdd);
        }
    }

    /**
     * <p>
     * Prints to a file name @filename a serialized object List<Report> which is a
     * List of all the reports in the diary.
     * This method is called by the main method in order to generate the output.
     */
    public void printToFile(String filename) {
        try (Writer writer = new FileWriter((filename))) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            gson.toJson(this, writer);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    /**
     * Gets the total number of received missions (executed / aborted) be all the M-instances.
     *
     * @return the total number of received missions (executed / aborted) be all the M-instances.
     */
    public int getTotal() {
        return total;
    }

    /**
     * Increments the total number of received missions by 1
     */
    public void incrementTotal() {
        synchronized (this) {
            this.total++;
        }
    }
}