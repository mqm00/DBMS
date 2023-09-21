package basket;

import classes.ClassDTO;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.List;
import java.util.Map;
import java.util.Vector;

public class BasketDAO {
    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public BasketDAO() //DB연결
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

    public List<BasketDTO> basketList(){ //insert로 장바구니 담은 과목들 출력
        List<BasketDTO> bbs = new Vector<BasketDTO>(); //list와 vector를 이용하여 배열 형식으로 저장한 후에 테이블에 차례대로 출력
        try {
            String query2 = "select * from basketView"; //baksetView라는 View를 만들어서 디비상에서 원하는 조건을 먼저 만들어놓은 후 바로 가져온다.
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            while(rs.next()){
                BasketDTO dto = new BasketDTO();
                dto.setName(rs.getString("name"));
                dto.setCourse_id(rs.getString("course_id"));
                dto.setClass_id(rs.getInt("class_id"));
                dto.setLecturer_name(rs.getString("Lecturer"));
                bbs.add(dto);
            }

        } catch (Exception e) {
            System.out.println("희망 강의 중 오류 발생");
            e.printStackTrace();
        }
        return bbs;
    }


    public int insertBasket(BasketDTO basketDTO){ //희망버튼 눌렀을 때 DB 에 insert를 위함
        int result = 0;
        try {
            String query2 = "insert into basket (class_id) VALUES (?)";
            pstmt = conn.prepareStatement(query2);
            pstmt.setInt(1, basketDTO.getClass_id());
            result = pstmt.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }
}