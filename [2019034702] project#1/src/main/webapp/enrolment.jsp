<%@ page import="enrolment.EnrolDTO" %>
<%@ page import="enrolment.EnrolDAO" %>
<%@ page import="classes.ClassDTO" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/02
  Time: 7:01 PM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html"; charset="UTF-8">
    <title>메인페이지</title>
</head>
<body>
<%
    int class_id = Integer.parseInt(request.getParameter("class_id"));
    String userID = (String)session.getAttribute("userID");

    ClassDTO classDTO = new ClassDTO();
    EnrolDTO dto = new EnrolDTO();
    classDTO.setClass_id(class_id);
    dto.setClass_id(class_id);
    dto.setUserID(userID);

    int check = 0;
    EnrolDAO dao = new EnrolDAO();
    int result = dao.checkEnrol(dto, classDTO);
    if(result == 0) { check ++;%>
<script>
    alert('이미 신청한 과목입니다.');
    location.href = "List.jsp";
</script>
<% } else if(result == 2) { check++;%>
<script>
    alert('수강 허용인원이 초과되었습니다.')
    location.href = "List.jsp";
</script> <% }else if(result == 3) {check++;%>
<script>
    alert('최대 이수가능 학점이 초과되었습니다.')
    location.href = "List.jsp";
</script> <% } %>

    <% int insertResult = 0;
        if(check ==0){
        insertResult = dao.insertEnrol(dto, classDTO);}
    if(insertResult == 1) { %>
<script>
    alert('수강신청이 완료되었습니다.')
    location.href = "List.jsp";
</script>
<% } else {%>
<script>
    alert('수강신청에 실패하였습니다.')
    location.href = "List.jsp";
</script>
<%}%>

</body>
</html>
