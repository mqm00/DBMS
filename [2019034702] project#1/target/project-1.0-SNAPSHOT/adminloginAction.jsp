<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ page import="admin.adminDAO" %>
<%@ page import="java.io.PrintWriter" %>
<% request.setCharacterEncoding("UTF-8"); %>
<jsp:useBean id="admin" class="admin.adminDTO" scope="page" />
<jsp:setProperty name="admin" property="adminID" />
<jsp:setProperty name="admin" property="adminPassword" />
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html"; charset="UTF-8">
    <title>로그인 액션</title>
</head>
<body>
<%
    String adminID = null;
    if(session.getAttribute("adminID") != null) //session에서 받은 adminID가 없다면
    {
        adminID = (String) session.getAttribute("adminID"); //session에서 받아온다
    }
    if(adminID != null) //받아온 것이 없다면
    {
        PrintWriter script = response.getWriter();
        script.println("<script>");
        script.println("alert('이미 로그인이 되어있습니다.')");
        script.println("location.href = 'main.jsp'");
        script.println("</script>");
    }

    adminDAO adminDAO = new adminDAO();

    int result = adminDAO.adminLogin(admin.getAdminID(), admin.getAdminPassword()); //받아온 ID, password를 가지고 로그인

    if(result == 1) //아이디에 맞는 비밀번호가 입력되었을 때
    {
        session.setAttribute("adminID", admin.getAdminID()); //session에 set
        PrintWriter script = response.getWriter();
        script.println("<script>");
        script.println("location.href = 'main.jsp'"); //main으로 간다
        script.println("</script>");
    }
    else if(result == 0) //비밀번호가 틀렸을 때
    {
        PrintWriter script = response.getWriter();
        script.println("<script>");
        script.println("alert('비밀번호가 틀립니다.')");
        script.println("history.back()");
        script.println("</script>");
    }
    else if(result == -1) //아이디가 select이 안됐을 때
    {
        PrintWriter script = response.getWriter();
        script.println("<script>");
        script.println("alert('존재하지 않는 아이디입니다.')");
        script.println("history.back()");
        script.println("</script>");
    }
    else if(result == -2) //기타 오류시
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