<%@ page import="time.TimeDAO" %>
<%@ page import="time.TimeDTO" %>
<%@ page import="java.util.List" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/08
  Time: 1:59 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    TimeDAO dao = new TimeDAO();

%>
<html>
<head>
    <title>시간표</title>
</head>
<body>
<%
String userID = (String) session.getAttribute("userID");
int user = Integer.parseInt(userID);
List<TimeDTO> timeListsMon = dao.timeListMon(user);%>
<table border = "1" width = "90%">
    <tr>
        <th width="5%">월</th>
        <th width="5%">시작시간</th>
        <th width="5%">종료시간</th>
        <th width="5%">시간</th>
        <th width="5%">수</th>
        <th width="5%">시간</th>
        <th width="5%">목</th>
        <th width="5%">시간</th>
        <th width="5%">금</th>
        <th width="5%">시간</th>
        <th width="5%">토</th>
        <th width="5%">시간</th>
    </tr>
        <% for(TimeDTO dto: timeListsMon){ %>
    <tr align = "center">
        <td align = "center"><%= dto.getClass_name()%></td>
        <td align = "center"><%= dto.getBegin()%></td>
        <td align = "center"><%= dto.getEnd()%></td>
    </tr>
        <%
    }
  %>
    <button type="button" onclick="location.href = 'List.jsp';">목록보기</button>
    <button type="button" onclick="location.href = 'main.jsp';">메인으로 돌아가기</button>
</body>
</html>

