<%@ page import="enrolment.EnrolDAO" %>
<%@ page import="enrolment.EnrolDTO" %>
<%@ page import="java.util.List" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/08
  Time: 12:35 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
%>
<html>
<head>
    <title>수강신청 현황</title>
</head>
<body>
<%
    String userID = null;
    if(session.getAttribute("userID") != null) //session에 userID가 셋팅되어있지 않다면
    {
        userID = (String) session.getAttribute("userID"); //login.jsp에서 받은 인자로 login
        session.setAttribute("userID", session.getAttribute("userID")); }
        EnrolDAO dao = new EnrolDAO();
    int user = Integer.parseInt(userID);
        List<EnrolDTO> enrolLists = dao.enrolList(user);
     %>
<table border = "1" width = "90%">
    <tr>
        <th width="9%">번호</th>
        <th width="9%">학수번호</th>
        <th width="9%">수업번호</th>
        <th width="9%">교과명</th>
        <th width="9%">교수</th>
        <th width="9%">재수강여부</th>
        <th width="9%">신청 취소</th>
    </tr>
        <% int i=1;
    for(EnrolDTO dto: enrolLists){
        dto.setUserID(userID);
        dto.setRetake(dao.retakeCheck(dto));// 수강신청 목록 테이블에 모두 출력 %>
    <tr align = "center">
        <td align = "center"><%= i++%></td>
        <td align = "center"><%= dto.getCourse_id()%></td>
        <td align = "center"><%= dto.getClass_id()%></td>
        <td align = "center"><%= dto.getClass_name()%></td>
        <td align = "center"><%= dto.getLecturer_name()%></td>
        <td align = "center"><%= dto.getRetake()%></td>
        <td><a href="enrolDelete.jsp?class_id=<%=dto.getClass_id()%>"
               class="btn btn_secondary" role="button">취소</a></td>

    </tr>
        <%
    }
  %>
    <button type="button" onclick="location.href = 'List.jsp';">목록보기</button>
    <button type="button" onclick="location.href = 'main.jsp';">메인으로 가기</button>
</body>
</html>

