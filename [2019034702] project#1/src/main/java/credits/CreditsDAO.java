package credits;

import classes.ClassDTO;
import student.StudentDTO;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.text.DecimalFormat;
import java.util.List;
import java.util.Map;
import java.util.Vector;

public class CreditsDAO {
    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public CreditsDAO()
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

    public List<CreditsDTO> olap(){ //olap로 주어진 과제는 데이터베이스에 olap라는 View를 만들어서 구현하였다.
        List<CreditsDTO> bbs = new Vector<CreditsDTO>();
        String query2 = "select * from olap"; //olap에서 바로 가져온다
        try {
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            while(rs.next()){
                CreditsDTO dto = new CreditsDTO();
                dto.setCourse_id(rs.getString("course_id"));
                bbs.add(dto);
            }

        } catch (Exception e) {
            System.out.println("OLAP 조회 중 오류 발생");
            e.printStackTrace();
        }
        return bbs;
    }

    public List<CreditsDTO> avgList(int student_id) { //학생들의 평점 추출
        List<CreditsDTO> bbs = new Vector<CreditsDTO>();
        DecimalFormat df = new DecimalFormat("0.00"); //소수점 둘째자리 까지만 출력

        String query = "Select student.student_id, student.name, credits.year, avg(credits.grade) as mygrade from student join credits on student.student_id = credits.student_id where student.student_id = ? group by credits.year order by credits.year desc";
        try {
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, student_id);
            rs = pstmt.executeQuery();
            while (rs.next()) {
                CreditsDTO dto = new CreditsDTO();
                dto.setStudent_id(rs.getInt("student_id"));
                dto.setName(rs.getString("name"));
                dto.setYear(rs.getInt("year"));
                dto.setGrade(Float.parseFloat(df.format((rs.getFloat("mygrade")))));

                bbs.add(dto);
            }

        } catch (Exception e) {
            System.out.println("성적 조회 중 오류 발생");
            e.printStackTrace();
        }
        return bbs;
    }
}
