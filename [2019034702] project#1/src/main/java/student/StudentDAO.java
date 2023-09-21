package student;

import java.sql.*;
import java.util.List;
import java.util.Map;
import java.util.Vector;

public class StudentDAO{
    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public StudentDAO() //디비연결
    {
        try{
            String dbURL = "jdbc:mysql://localhost:3307/DB2019034702?serverTimezone=Asia/Seoul";
            String dbID = "root";
            String dbPassword = "lss02838483#";
            Class.forName("com.mysql.jdbc.Driver");
            conn = DriverManager.getConnection(dbURL, dbID, dbPassword);
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    public int studentEdit(StudentDTO dto){ //학생 정보 수정
        int result = 0;
        try {
            String query = "UPDATE student SET status = ? WHERE student_id = ?";
            pstmt = conn.prepareStatement(query);
            pstmt.setString(1, dto.getStatus());
            pstmt.setInt(2, dto.getStudent_id());

            result = pstmt.executeUpdate();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return result;
    }

    public List<StudentDTO> stuList(Map<String, Object> map){ // 학생 목록 가져오기
        List<StudentDTO> bbs = new Vector<StudentDTO>();

        String query2 = "SELECT * from student join lecturer on student.lecturer_id = lecturer.lecturer_id"; //교과목 교수 이름을 같이 보여주고 싶어서 join 하였다
        if(map.get("searchWord") != null){ //이름 혹은 학번으로 검색가능
            query2 += " WHERE student." + map.get("searchField") + " " +  "LIKE '%" + map.get("searchWord") + "%'";
        }

        try {
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            while(rs.next()){
                StudentDTO dto = new StudentDTO();

                dto.setStudent_id(rs.getInt("student_id"));
                dto.setName(rs.getString("name"));
                dto.setLecturer_name(rs.getString("lecturer.name"));
                dto.setSex(rs.getString("sex"));
                dto.setStatus(rs.getString("status"));

                bbs.add(dto);
            }

        } catch (Exception e) {
            System.out.println("학생 조회 중 오류 발생");
            e.printStackTrace();
        }
        return bbs;
    }

    public StudentDTO selectView(int student_id) { //학생 정보 한명씩 보기
        StudentDTO dto = new StudentDTO();
        String query = "Select student.student_id, name, sex, major_id, lecturer_id, status from student where student_id = ?";

        try {
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, student_id);
            rs = pstmt.executeQuery();

            if(rs.next()){
                dto.setStudent_id(rs.getInt("student_id"));
                dto.setName(rs.getString("name"));
                dto.setSex(rs.getString("sex"));
                dto.setLecturer_id(rs.getLong("lecturer_id"));
                dto.setStatus(rs.getString("status"));

            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return dto;
    }

    public List<StudentDTO> gradeList(int student_id){ //학번에 따른 성적 검색하기, 연도별로 검색
        List<StudentDTO> bbs = new Vector<StudentDTO>();

        String query = "Select student.student_id, student.name, credits.course_id, credits.year, credits.grade from student join credits on student.student_id = credits.student_id where student.student_id = ? order by credits.year desc";
        try {
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, student_id);
            rs = pstmt.executeQuery();
            while(rs.next()){
                StudentDTO dto = new StudentDTO();

                dto.setStudent_id(rs.getInt("student_id"));
                dto.setName(rs.getString("name"));
                dto.setCourse_id(rs.getString("course_id"));
                dto.setYear(rs.getInt("year"));
                dto.setGrade(rs.getString("grade"));

                bbs.add(dto);
            }

        } catch (Exception e) {
            System.out.println("성적 조회 중 오류 발생");
            e.printStackTrace();
        }
        return bbs;
    }


}