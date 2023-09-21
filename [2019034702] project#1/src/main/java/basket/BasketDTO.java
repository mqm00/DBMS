package basket;

public class BasketDTO {
    private String course_id;
    private int class_id;
    private String name;

    private String Lecturer_name;

    public int getClass_id() {
        return class_id;
    }

    public void setCourse_id(String course_id) {
        this.course_id = course_id;
    }

    public String getName() {
        return name;
    }

    public void setClass_id(int class_id) {
        this.class_id = class_id;
    }

    public String getCourse_id() {
        return course_id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getLecturer_name() {
        return Lecturer_name;
    }

    public void setLecturer_name(String lecturer_name) {
        Lecturer_name = lecturer_name;
    }
}

