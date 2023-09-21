package admin;
import java.sql.*;

import java.sql.DriverManager;
import java.util.Map;

public class adminDAO {
    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public adminDAO() //생성자에서 DB 연결
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

    public int adminLogin(String adminID, String adminPassword) //관리자 로그인
    {
        String SQL = "SELECT adminPassword FROM admin WHERE adminID = ?"; //DB에서 비밀번호 select후 비교
        try{

            pstmt = conn.prepareStatement(SQL);
            pstmt.setString(1, adminID);
            rs = pstmt.executeQuery();
            if(rs.next()) //아이디에 맞는 비밀번호가 있다면
            {
                if(rs.getString(1).equals(adminPassword))
                {
                    return 1; //로그인 성공
                }
                else
                {
                    return 0; //비밀번호 불일치
                }
            }
            return -1; // 아이디가 없음
        } catch (Exception e) {
            e.printStackTrace();
        }
        return -2; // 데이터베이스 오류
    }
}
