package user;

import java.sql.*;
import java.util.ArrayList;

public class UserDAO {

    private Connection conn;
    private PreparedStatement pstmt;
    private ResultSet rs;

    public UserDAO()
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

    public int login(String userID, String userPassword) //로그인
    {
        String SQL = "SELECT userPassword FROM USER WHERE userID = ?"; //DB에 등록된 아이디에 따른 비밀번호 select후 입력된 비밀번호와 비교
        try{

            pstmt = conn.prepareStatement(SQL);
            pstmt.setString(1, userID);
            rs = pstmt.executeQuery();
            if(rs.next())
            {
                if(rs.getString(1).equals(userPassword))
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

    
    public int join(User user) //회원가입
    {
    	String SQL = "INSERT INTO USER VALUES (?, ?, ?, ?, ?)";
    	try
    	{
    		pstmt = conn.prepareStatement(SQL);
    		pstmt.setString(1, user.getUserID());
    		pstmt.setString(2, user.getUserPassword());
    		pstmt.setString(3, user.getUserName());
    		pstmt.setString(4, user.getUserGender());
    		pstmt.setString(5, user.getUserEmail());

    		return pstmt.executeUpdate();
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	return -1; // 데이터베이스 오류
    }

    public ArrayList<User> showAll() //모든 사용자 정보 보기
    {
        String SQL = "SELECT * FROM USER";
        ArrayList<User> studentList = new ArrayList<User>();
        try
        {
            pstmt = conn.prepareStatement(SQL);
            rs = pstmt.executeQuery();

            while(rs.next())
            {
                User user = new User();
                user.setUserID(rs.getString(1));
                user.setUserPassword(rs.getString(2));
                user.setUserName(rs.getString(3));
                user.setUserGender(rs.getString(4));
                user.setUserEmail(rs.getString(5));
                studentList.add(user);
            }

        } catch (Exception e)
        {
            e.printStackTrace();
        }
        return studentList;
    }
}
