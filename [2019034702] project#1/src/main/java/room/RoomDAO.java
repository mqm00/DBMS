package room;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class RoomDAO {
    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public RoomDAO()
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
}
