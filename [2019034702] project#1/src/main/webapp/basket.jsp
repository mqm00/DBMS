<%@ page import="credits.CreditsDTO" %>
<%@ page import="credits.CreditsDAO" %>
<%@ page import="java.util.List" %>
<%@ page import="basket.BasketDTO" %>
<%@ page import="basket.BasketDAO" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/03
  Time: 2:53 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    BasketDAO dao = new BasketDAO();
    List<BasketDTO> basketLists = dao.basketList(); //장바구니 list 받아오기
%>
<html>
<head>
    <title>장바구니</title>
</head>
<body>
<table border = "1" width = "90%">
    <tr>
        <th width="9%">번호</th>
        <th width="9%">학수번호</th>
        <th width="9%">수업번호</th>
        <th width="9%">교과명</th>
        <th width="9%">교수</th>
    </tr>
        <% int i=1;
    for(BasketDTO dto: basketLists){ //for문으로 돌아가면서 정보 테이블에 바로 넣기%>
    <tr align = "center">
        <td align = "center"><%= i++%></td>
        <td align = "center"><%= dto.getCourse_id()%></td>
        <td align = "center"><%= dto.getClass_id()%></td>
        <td align = "center"><%= dto.getName()%></td>
        <td align = "center"><%= dto.getLecturer_name()%></td>
        <td><a href="enrolment.jsp?class_id=<%=dto.getClass_id()%>"
               class="btn btn_secondary" role="button">신청</a></td>
    </tr>
        <%
    }
  %>
    <button type="button" onclick="location.href = 'List.jsp';">목록보기</button>
</body>
</html>
