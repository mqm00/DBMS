<%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/02
  Time: 4:28 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="student.StudentDTO" %>
<%@ page import="student.StudentDAO" %>
<%
    int student_id = Integer.parseInt(request.getParameter("student_id")); //student_id를 객체로 전달받음
    String status = request.getParameter("status"); //현재 학적 상태

    StudentDTO dto = new StudentDTO();
    dto.setStudent_id(student_id);
    if(status == null){     //학적 상태가 설정되어있지 않다면 설정페이지로 바로 이동한다.
        response.sendRedirect("studentEdit.jsp");
    }
    dto.setStatus(status);

    StudentDAO dao = new StudentDAO();
    int result = dao.studentEdit(dto); //executeupdate 완료될 시 1 return

    if(result == 1){ //제대로 설정되면 학생 리스트로, 아니면 다시 설정페이지로
        response.sendRedirect("studentList.jsp");
    } else {
        response.sendRedirect("studentEdit.jsp");
    }
%>
<html>
<head>
    <title>Title</title>
</head>
<body>

</body>
</html>
