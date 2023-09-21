<%@ page import="classes.ClassDAO" %>
<%@ page import="student.StudentDTO" %>
<%@ page import="javax.swing.plaf.synth.SynthFormattedTextFieldUI" %>
<%@ page import="student.StudentDAO" %>
<%@ page import="java.util.List" %>
<%@ page import="credits.CreditsDAO" %>
<%@ page import="credits.CreditsDTO" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/02
  Time: 5:44 PM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
  int student_id = Integer.parseInt(request.getParameter("student_id")); //student_id를 객체로 받아온다
  StudentDAO dao = new StudentDAO();
  CreditsDAO creditsDAO = new CreditsDAO();
  List<StudentDTO> gradeList = dao.gradeList(student_id); //성적 List 담기
  List<CreditsDTO> avgList = creditsDAO.avgList(student_id); //성적 평균내기
%>
<html>
<head>
    <title>학생 성적 조회</title>
</head>
<body>
<table border = "1" width = "90%">
  <tr>
    <th width="9%">학번</th>
    <th width="9%">이름</th>
    <th width="9%">학수번호</th>
    <th width="9%">수강년도</th>
    <th width="9%">성적</th>
  </tr>
  <% for(StudentDTO dto : gradeList){   //성적을 모두 출력한다
    %>
  <tr align = "center">
    <td align = "center"><%= dto.getStudent_id()%></td>
    <td align = "center"><%= dto.getName()%></td>
    <td align = "center"><%= dto.getCourse_id()%></td>
    <td align = "center"><%= dto.getYear()%></td>
    <td align = "center"><%= dto.getGrade()%></td>
  </tr>
  <%
    }
  %>
</table>
<table border = "1" width = "90%">
  <tr>
    <th width="9%">학번</th>
    <th width="9%">이름</th>
    <th width="9%">수강년도</th>
    <th width="9%">평균성적</th>
  </tr>
  <% for(CreditsDTO dto : avgList){   //그 연도에 대한 평점평균을 출력한다.
    %>
  <tr align = "center">
    <td align = "center"><%= dto.getStudent_id()%></td>
    <td align = "center"><%= dto.getName()%></td>
    <td align = "center"><%= dto.getYear()%></td>
    <td align = "center"><%= dto.getGrade()%></td>
  </tr>
  <%
    }
  %>
</table>

<button type="button" class="navyBtn" onClick="location.href='studentList.jsp'">돌아가기</button>
<button type="button" class="navyBtn" onClick="location.href='main.jsp'">메인으로 돌아가기</button>

</body>
</html>
