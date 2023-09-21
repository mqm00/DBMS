package enrolment;

import basket.BasketDTO;
import classes.ClassDTO;
import credits.CreditsDTO;

import java.sql.*;
import java.util.List;
import java.util.Vector;

public class EnrolDAO {
    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public EnrolDAO() { //수강신청 디비연결
        try {
            String dbURL = "jdbc:mysql://localhost:3307/DB2019034702?serverTimezone=Asia/Seoul";
            String dbID = "root";
            String dbPassword = "lss02838483#";
            Class.forName("com.mysql.jdbc.Driver");
            conn = DriverManager.getConnection(dbURL, dbID, dbPassword);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public List<EnrolDTO> enrolList(int userID) { //수강신청 목록 출력
        List<EnrolDTO> bbs = new Vector<EnrolDTO>();
        try {
            String query2 = "select * from enrolView where userID = ?"; //수강신청인 enrolment와 lecturer, class를 join한 View 생성
            pstmt = conn.prepareStatement(query2);
            pstmt.setInt(1, userID);
            rs = pstmt.executeQuery();
            while (rs.next()) {
                EnrolDTO dto = new EnrolDTO();
                dto.setClass_name((rs.getString("class_name")));
                dto.setCourse_id(rs.getString("course_id"));
                dto.setClass_id(rs.getInt("class_id"));
                dto.setLecturer_name(rs.getString("Lecturer_name"));
                bbs.add(dto);
            }

        } catch (Exception e) {
            System.out.println("강의 조회 중 오류 발생");
            e.printStackTrace();
        }
        return bbs;
    }

    public int checkEnrol(EnrolDTO dto, ClassDTO classDTO) { //수강신청에 대한 조건처리
        int result = 1;
        // 중복확인
        try {
            String query = "select class_id from enrolment where userID = ? and class_id = ?";
            pstmt = conn.prepareStatement(query);
            pstmt.setString(1, dto.getUserID());
            pstmt.setInt(2, dto.getClass_id());
            rs = pstmt.executeQuery();

            if (rs.next()) {
                return 0;
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        //정원 다 찼을 경우
        try {
            String query = "select person_num, person_max from class where class_id = ?";
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, classDTO.getClass_id());
            rs = pstmt.executeQuery();
            if (rs.next()) {
                classDTO.setPerson_num(rs.getInt("person_num"));
                if (rs.getInt("person_num") == rs.getInt("person_max")) {
                    return 2;
                }
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        //최대학점 확인
        try {
            String query = "select sum(credit) as max_credit from enrolment where userID = ? group by userID";
            pstmt = conn.prepareStatement(query);
            pstmt.setString(1, dto.getUserID());
            rs = pstmt.executeQuery();
            if (!rs.next()) {
                return 1;
            }
            if (rs.getInt("max_credit") > 18) {
                return 3;
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return result;
    }


    public int insertEnrol(EnrolDTO dto, ClassDTO classDTO) { //수강신청에 추가
        int result = 0;
        try {
            String query = "select credit from class where class_id = ?"; //수업에 배정된 학점 가져오기
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, dto.getClass_id());
            rs = pstmt.executeQuery();
            if (rs.next()) {
                classDTO.setCredit(rs.getInt("credit"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
        try {
            String query = "Insert INTO enrolment (class_id, userID, credit) Values (?, ?, ?)"; // enrolment에 추가
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, dto.getClass_id());
            pstmt.setString(2, dto.getUserID());
            pstmt.setInt(3, classDTO.getCredit());
            result = pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
        try {
            String query = "UPDATE class SET person_num = ? where class_id = ?"; //수업 정원 업데이트하기
            pstmt = conn.prepareStatement(query);
            classDTO.setPerson_num(classDTO.getPerson_num() + 1);
            pstmt.setInt(1, classDTO.getPerson_num());
            pstmt.setInt(2, dto.getClass_id());
            result = pstmt.executeUpdate();

        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        return result;
    }

    public String retakeCheck(EnrolDTO dto) { //재수강 여부 표시
        String result = "X";
        try {
            int tempId = Integer.parseInt(dto.getUserID());
            //enrolView와 성적 스키마인 credits 를 학번 기준으로 조회, 회원가입시 학번으로 진행해야함
            String query = "select credits.course_id from credits join enrolView on credits.student_id = tempID where enrolView.course_id = ?";
            pstmt = conn.prepareStatement(query);
            pstmt.setString(1, dto.getCourse_id());
            rs = pstmt.executeQuery();
            if (rs.next()) { //전에 수강했던 것이 있다면 O
                result = "O";
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return result;
    }
    public int deleteEnrol(EnrolDTO dto, ClassDTO classDTO){
        int result = 0;
        try {
            String query = "select person_num from class where class_id = ?"; //수업 정원 업데이트하기
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, dto.getClass_id());
            rs = pstmt.executeQuery();
            if(rs.next()){
                classDTO.setPerson_num(rs.getInt("person_num"));
            }

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        try {
            String query = "UPDATE class SET person_num = ? where class_id = ?"; //수업 정원 업데이트하기
            pstmt = conn.prepareStatement(query);
            classDTO.setPerson_num(classDTO.getPerson_num()-1);
            pstmt.setInt(1, classDTO.getPerson_num());
            pstmt.setInt(2, dto.getClass_id());
            result = pstmt.executeUpdate();

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        try {
            String query = "delete from enrolment where class_id = ? and userID = ?"; //수업에 배정된 학점 가져오기
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, dto.getClass_id());
            pstmt.setString(2, dto.getUserID());
            result = pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return result;
    }
}
