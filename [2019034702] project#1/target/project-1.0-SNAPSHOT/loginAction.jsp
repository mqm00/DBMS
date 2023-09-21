<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ page import="user.UserDAO" %>
<%@ page import="java.io.PrintWriter" %>
<% request.setCharacterEncoding("UTF-8"); %>
<jsp:useBean id="user" class="user.User" scope="page" />
<jsp:setProperty name="user" property="userID" />
<jsp:setProperty name="user" property="userPassword" />
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html"; charset="UTF-8">
    <title>로그인 액션</title>
</head>
<body>
    <%
    	String userID = null;
    	if(session.getAttribute("userID") != null) //session에 userID가 셋팅되어있지 않다면
    	{
    		userID = (String) session.getAttribute("userID"); //login.jsp에서 받은 인자로 login
            session.setAttribute("userID", session.getAttribute("userID"));
    	}
    	if(userID != null)  //session에 이미 userID가 있다면 로그인을 진행한 것
    	{
    		PrintWriter script = response.getWriter();
            script.println("<script>");
            script.println("alert('이미 로그인이 되어있습니다.')");
            script.println("location.href = 'main.jsp'");
            script.println("</script>");
    	}
    
        UserDAO userDAO = new UserDAO();

        int result = userDAO.login(user.getUserID(), user.getUserPassword());  //db에 있는 유저 정보와 비교

        if(result == 1) //db에 있는 정보와 입력값이 일치한다면 로그인 성공
        {
        	session.setAttribute("userID", user.getUserID());  //session에 userID 셋팅
            PrintWriter script = response.getWriter();
            script.println("<script>");
            script.println("location.href = 'main.jsp'");
            script.println("</script>");
        }
        else if(result == 0) //db에 있는 비밀번호 정보와 일치하지 않는 경우
        {
            PrintWriter script = response.getWriter();
            script.println("<script>");
            script.println("alert('비밀번호가 틀립니다.')");
            script.println("history.back()");
            script.println("</script>");
        }
        else if(result == -1) //db에서 아이디와 같은 튜플을 select할 수 없는 경우
        {
            PrintWriter script = response.getWriter();
            script.println("<script>");
            script.println("alert('존재하지 않는 아이디입니다.')");
            script.println("history.back()");
            script.println("</script>");
        }
        else if(result == -2) //db에서 정보를 못읽을 경우
        {
            PrintWriter script = response.getWriter();
            script.println("<script>");
            script.println("alert('데이터베이스 오류가 발생했습니다.')");
            script.println("history.back()");
            script.println("</script>");
        }

    %>

</body>
</html>