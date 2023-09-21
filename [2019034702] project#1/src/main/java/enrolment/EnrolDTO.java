package enrolment;

public class EnrolDTO {
    private int class_id;
    private String userID;

    private String Lecturer_name;

    private String course_id;

    private String class_name;

    private String retake;

    private int max_credit;

    public String getUserID() {
        return userID;
    }

    public void setUserID(String userID) {
        this.userID = userID;
    }

    public int getClass_id() {
        return class_id;
    }

    public void setClass_id(int class_id) {
        this.class_id = class_id;
    }

    public int getMax_credit() {
        return max_credit;
    }

    public void setMax_credit(int max_credit) {
        this.max_credit = max_credit;
    }

    public String getLecturer_name() {
        return Lecturer_name;
    }

    public void setLecturer_name(String lecturer_name) {
        Lecturer_name = lecturer_name;
    }

    public String getCourse_id() {
        return course_id;
    }

    public void setCourse_id(String course_id) {
        this.course_id = course_id;
    }

    public String getClass_name() {
        return class_name;
    }

    public void setClass_name(String class_name) {
        this.class_name = class_name;
    }

    public String getRetake() {
        return retake;
    }

    public void setRetake(String retake) {
        this.retake = retake;
    }
}
