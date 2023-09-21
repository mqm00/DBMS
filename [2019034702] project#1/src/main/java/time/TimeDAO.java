package time;

import basket.BasketDTO;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.List;
import java.util.Vector;

public class TimeDAO {
    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public TimeDAO()
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

    public List<TimeDTO> timeListMon(int userID){
        List<TimeDTO> bbs = new Vector<TimeDTO>();
        try {
            String query2 = "select * from schedule order by day";
            pstmt = conn.prepareStatement(query2);
            rs = pstmt.executeQuery();
            while(rs.next()){
                TimeDTO dto = new TimeDTO();
                dto.setClass_id(rs.getInt("class_id"));
                dto.setClass_name(rs.getString("name"));
                dto.setUserID(userID);
                dto.setDay(rs.getInt("day"));
                dto.setBegin(rs.getString("begin"));
                dto.setEnd(rs.getString("end"));
                bbs.add(dto);
            }

        } catch (Exception e) {
            System.out.println("시간표 조회 중 오류 발생");
            e.printStackTrace();
        }
        return bbs;
    }
}
