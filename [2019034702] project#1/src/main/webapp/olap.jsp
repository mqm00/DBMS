<%@ page import="credits.CreditsDTO" %>
<%@ page import="credits.CreditsDAO" %>
<%@ page import="java.util.List" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/03
  Time: 2:53 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    CreditsDAO dao = new CreditsDAO();
    List<CreditsDTO> olapLists = dao.olap(); //평점 평균과 특정 과목의 학점 간 차이(평점평균-과목학점)가 가장 큰 Top10 과목 추출
%>
<html>
<head>
    <title>강의 통계</title>
</head>
<body>
<table border = "1" width = "90%">
    <tr>
        <th width="9%">번호</th>
        <th width="9%">학수번호</th>
    </tr>
        <% int i=1;
    for(CreditsDTO dto : olapLists){ %>
    <tr align = "center">
        <td align = "center"><%= i++%></td>
        <td align = "center"><%= dto.getCourse_id()%></td>
    </tr>
        <%
    }
  %>
</body>
</html>
