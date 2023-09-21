package classes;
import room.RoomDTO;
import student.StudentDTO;
import user.User;

import javax.servlet.ServletContext;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Vector;

public class ClassDAO {
    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public ClassDAO() //생성자에서 DB 연결
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

    public List<ClassDTO> selectList(Map<String, Object> map){ // 수강편람에서 과목 list 출력
        List<ClassDTO> bbs = new Vector<ClassDTO>();
       String query2 = "SELECT * FROM ((((class join lecturer on class.lecturer_id = lecturer.lecturer_id) join room on class.room_id = room.room_id) join building on building.building_id=room.building_id) join time on class.class_id = time.class_id)";
        if(map.get("searchWord") != null){ //searchWord는 검색 단어, searchField는 검색 분야 (ex. 학수번호, 수업번호, 과목명)
            if(map.get("searchField").equals("name")){ //searchWord가 없다면 모두 출력, 있다면 쿼리 업데이트
                query2 += " WHERE class." + map.get("searchField") + " " +  " LIKE '%" + map.get("searchWord") + "%'";}
            else if(map.get("searchField").equals("class_id")){
                query2 += " WHERE class." + map.get("searchField") + "=" + map.get("searchWord");}
            else{
                query2 += " WHERE class." + map.get("searchField") + "=" + "'" + map.get("searchWord") + "'";
            }
            }

        try {
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            while(rs.next()){ //쿼리로 select된 것들 담기
                ClassDTO dto = new ClassDTO();
                dto.setOpened(rs.getInt("opened"));
                if(dto.getOpened() < 2022) continue; //2022년 이전에 개설되었던 교과목은 수강신청이 불가하므로 제외
                if(rs.getInt("day") == 1){ //day 값 int로 db에 저장되어 있으므로 문자로 바꿔준다.
                    dto.setDay("월");
                }else if(rs.getInt("day") == 2){
                    dto.setDay("화");}
                else if(rs.getInt("day") == 3){
                    dto.setDay("수");}
                else if(rs.getInt("day") == 4){
                    dto.setDay("목");}
                else if(rs.getInt("day") == 5){
                    dto.setDay("금");}
                else if(rs.getInt("day") == 6){
                    dto.setDay("토");}
                dto.setClass_id(rs.getInt("class_id"));
                dto.setCourse_id(rs.getString("course_id"));
                dto.setClass_name(rs.getString("class.name"));
                dto.setLecturer_name(rs.getString("lecturer.name"));
                dto.setPerson_num(rs.getInt("person_num"));
                dto.setPerson_max(rs.getInt("person_max"));
                dto.setRoom_id(rs.getInt("room_id"));
                dto.setBegin(rs.getString("begin"));
                dto.setEnd(rs.getString("end"));
                dto.setBuilding_name(rs.getString("building.name"));

                bbs.add(dto);
            }

        } catch (Exception e) {
            System.out.println("강의 조회 중 오류 발생");
            e.printStackTrace();
        }
        return bbs;
    }

    //강의를 개설하기 위해선 foreign key를 만족하여야했다. (trouble)
    //foreign key를 하나씩 check
    public int checkCourse(ClassDTO dto){ //course check

        int result =0;
        try{
            String query2 = "SELECT course_id from course where course_id = " + "'"+ dto.getCourse_id() + "'";
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            if(rs.next()) return 1; //course에 있어야 만들 수 있다.
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }

        return 0;
    }

    public int checkLecturer(ClassDTO dto){
        int result = 0;
        try{
            String query2 = "SELECT lecturer_id from lecturer where lecturer_id = " + dto.getLecturer_id();
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            if(rs.next()) return 1;
        } catch (SQLException e) {
            throw new RuntimeException(e);
        } return 0;
    }

    public int insertLecturer(ClassDTO dto) {
        int result = 0;
         try{
            String query2 = "Insert INTO lecturer (" + "lecturer_id) Values (" + "?)";

            pstmt = conn.prepareStatement(query2);
            pstmt.setLong(1, dto.getLecturer_id());
            result = pstmt.executeUpdate();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return result;
    }
    public int checkBuilding(ClassDTO dto){ //building도 data 안에 있는 것이어야만 한다.
        int result = 0;
        try{
            String query2 = "SELECT building_id from building where building_id = " + dto.getBuilding_id();
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            if(rs.next()) return 1;
        } catch (SQLException e) {
            throw new RuntimeException(e);
        } return 0;
    }

    public int checkRoom(ClassDTO dto){ // room도 data로 주어진 것으로만 한정
        int result = 0;
        try{
            String query2 = "SELECT room_id from room where room_id = " + dto.getRoom_id();
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            if(rs.next()) return 1;
        } catch (SQLException e) {
            throw new RuntimeException(e);
        } return 0;
    }

    public int checkPerson(ClassDTO dto, RoomDTO roomDTO) { //수강인원이 강의실 인원보다 클 때 처리
        int result = 0;
        try {
            String query1 = "select occupancy from room where room_id = ?";
            pstmt = conn.prepareStatement(query1);
            pstmt.setInt(1, dto.getRoom_id());
            rs = pstmt.executeQuery();
            if (rs.next()) {
                roomDTO.setOccupancy(rs.getInt("occupancy"));
            }
            else
                return 0;

            if (roomDTO.getOccupancy() < dto.getPerson_max()) //최대 수용인원보다 개설하려는 인원이 더 많을 경우
                return 0;
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return 1;
    }

    public int insertPerson(ClassDTO dto) { //인원 조건을 충족했다면 insert
        int result = 0;
        try {
            String query2 = "UPDATE class set person_max = " + dto.getPerson_max() + " where class_id = ?";
            pstmt = conn.prepareStatement(query2);
            pstmt.setInt(1, dto.getClass_id());
            result = pstmt.executeUpdate();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return result;
    }

    public int checkClass(ClassDTO dto) { //class_id 중복 방지
        int result = 0;
        try {
            String query2 = "SELECT class_id from class where class_id = " + dto.getClass_id();
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            if (rs.next()) return 0; // class_id는 겹치면 안되니까
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return 1;
    }

    public int insertClass(ClassDTO dto){ //중복된 class_id가 없을 시 insert
        int result = 0;
        try{
            String query = "Insert INTO class (" + "course_id, class_id, class_no, name, major_id, year, credit, lecturer_id, room_id, person_max, opened, person_num)" + " Values (" + "?, ?, ?, ?, ?, ?, ? ,?, ?, ?, ?, ?)";

            pstmt = conn.prepareStatement(query);
            pstmt.setString(1, dto.getCourse_id());
            pstmt.setInt(2, dto.getClass_id());
            pstmt.setInt(3, dto.getClass_no());
            pstmt.setString(4, dto.getClass_name());
            pstmt.setInt(5, dto.getMajor_id());
            pstmt.setInt(6, dto.getYear());
            pstmt.setInt(7, dto.getCredit());
            pstmt.setLong(8, dto.getLecturer_id());
            pstmt.setInt(9, dto.getRoom_id());
            pstmt.setInt(10,dto.getPerson_max());
            pstmt.setInt(11, dto.getOpened());
            pstmt.setInt(12, 0);

            result = pstmt.executeUpdate();
        } catch (Exception e) {
            System.out.println("입력 중 오류 발생");
            e.printStackTrace();
        }
        return result;
}

    public int deleteClass(ClassDTO dto){ //강의 삭제
        int result = 0;
        try{
            String query2 = "delete from enrolment where class_id=?"; //수강신청한 목록에서 삭제하고
            pstmt = conn.prepareStatement(query2);
            pstmt.setInt(1, dto.getClass_id());
            result = pstmt.executeUpdate();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }

        try{
            String query = "delete from class where class_id=?"; //최종적으로 class에서 삭제
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, dto.getClass_id());
            result = pstmt.executeUpdate();
        } catch (SQLException e) {
            System.out.println("삭제 처리 중 예외 발생");
            throw new RuntimeException(e);
        }
        return result;
    }
    public ClassDTO selectView(int class_id) {
        ClassDTO dto = new ClassDTO();
        String query = "Select class_id from class where class_id = ?";
        try {
            pstmt = conn.prepareStatement(query);
            pstmt.setInt(1, class_id);
            rs = pstmt.executeQuery();

            if(rs.next()){
               dto.setClass_id(rs.getInt("class_id"));
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return dto;
    }
}
