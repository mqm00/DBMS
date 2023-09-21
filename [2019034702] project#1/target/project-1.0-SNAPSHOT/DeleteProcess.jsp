<%@ page import="classes.ClassDTO" %>
<%@ page import="classes.ClassDAO" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/03
  Time: 12:59 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    int class_id = Integer.parseInt(request.getParameter("class_id")); //삭제할 class_id 객체를 넘겨받음
    ClassDTO dto = new ClassDTO();
    ClassDAO dao = new ClassDAO();
    dto = dao.selectView(class_id); //class_id에 해당하는 class 튜플 가져오기

    dto.setClass_id(class_id);
    int delResult = 0;
    delResult = dao.deleteClass(dto); //delete 쿼리가 제대로 executeUpdate되면 return값으로 1을 받아온다

    if(delResult == 1){
        response.sendRedirect("List.jsp");
    }
    else if(delResult == 0){
        response.sendRedirect("WriteError.jsp");
    }
%>
