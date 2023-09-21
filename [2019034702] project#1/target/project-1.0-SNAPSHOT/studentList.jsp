<%@ page import="student.StudentDAO" %>
<%@ page import="java.util.Map" %>
<%@ page import="student.StudentDTO" %>
<%@ page import="java.util.List" %>
<%@ page import="java.util.HashMap" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/01
  Time: 5:30 PM
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" language="java" pageEncoding="UTF-8" %>
<%
    StudentDAO dao = new StudentDAO();
    Map<String, Object> param = new HashMap<String, Object>(); //학생을 이름과 학번으로 검색할 수 있게 검색필드와 검색어 mapping
    String searchField = request.getParameter("searchField");
    String searchWord = request.getParameter("searchWord");
    if(searchWord != null){ //검색어로 들어온 것이 있다면
        param.put("searchField", searchField);
        param.put("searchWord", searchWord);
    }

    List<StudentDTO> stuLists = dao.stuList(param); //mapping된 파라미터를 가지고 list 추출
%>
<!DOCTYPE html>
<html>
<head>
    <meta charset = "UTF-8">
    <title>학생 조회</title>
</head>
<body>
<h2> 학적 조회</h2>
<form method="get">
    <table border = "1" width="90%">
        <tr>
            <td align = "center">
                <select name = "searchField"> <!-- 검색하는 부분 만들기 -->
                    <option value ="name">성명</option>
                    <option value="student_id">학번</option>
                </select>
                <input type = "text" name = "searchWord"/>
                <input type = "submit" value = "검색하기"/>
            </td>
        </tr>
    </table>
</form>

<form name = "listform" method = "post" action="studentEdit.jsp" > <!-- studentEdit으로 넘길 객체 보내기 -->
<table border = "1" width = "90%" style="margin-left: auto; margin-right: auto;">
    <tr>
        <th width="9%">성명</th>
        <th width="9%">학번</th>
        <th width="9%">지도교수</th>
        <th width="9%">성별</th>
        <th width="9%">학적</th>
    </tr>
    <%
        if(stuLists.isEmpty()){ //list에 검색된 학생이 없다면
    %>
    <tr>
        <td colspan = "5" align=center">
            검색된 학생이 없습니다.
        </td>
    </tr>
    <%} else {
        for(StudentDTO dto : stuLists){ //있다면 차례대로 출력
            %>
    <tr align = "center">
        <td align = "center"><%= dto.getName()%></td>
        <td align = "center"><%= dto.getStudent_id()%></td>
        <td align = "center"><%= dto.getLecturer_name()%></td>
        <td align = "center"><%= dto.getSex()%></td>
        <td align = "center"><%= dto.getStatus()%></td>
    </tr>
    <%
            }
        }
    %>
</table>
    <br>
    <br>
    <div><!-- 객체 전달 -->
        <input type = "text" name = "student_id" placeholder="학번을 입력해주세요" style="width:400px;height:50px;font-size:20px;">
        <button type="submit" class="navyBtn" style="width:100px;height: 50px;font-size:20px;">학생정보 수정하기</button>
    </div>
    <br>
</form>
<form name = "loadform" method = "post" action="studentGrade.jsp">
    <div>
        <input type="text" name = "student_id" placeholder="학번을 입력해주세요" style="width:400px;height:50px;font-size:20px;">
        <button type="submit" class="navyBtn" onClick="location.href='student_grade.jsp" style="width:100px;height:50px;font-size:20px;">성적 조회하기</button>
    </div>
</form>
</body>
</html>