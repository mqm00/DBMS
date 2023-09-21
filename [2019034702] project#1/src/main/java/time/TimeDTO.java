package time;

public class TimeDTO {
    private int class_id;
    private String begin;
    private String end;

    private int day;

    private int userID;

    private String class_name;


    public int getClass_id() {
        return class_id;
    }

    public void setClass_id(int class_id) {
        this.class_id = class_id;
    }


    public void setBegin(String begin) {
        this.begin = begin;
    }


    public void setEnd(String end) {
        this.end = end;
    }

    public String getBegin() {
        return begin;
    }

    public String getEnd() {
        return end;
    }

    public String getClass_name() {
        return class_name;
    }

    public void setClass_name(String class_name) {
        this.class_name = class_name;
    }

    public int getUserID() {
        return userID;
    }

    public void setUserID(int userID) {
        this.userID = userID;
    }

    public int getDay() {
        return day;
    }

    public void setDay(int day) {
        this.day = day;
    }
}
