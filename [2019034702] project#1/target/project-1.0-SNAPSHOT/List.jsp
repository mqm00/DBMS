<%@ page import="classes.ClassDAO" %>
<%@ page import="classes.ClassDTO" %>
<%@ page import="java.util.*" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/10/30
  Time: 6:08 PM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" pageEncoding="UTF-8" %>
<%
  ClassDAO dao = new ClassDAO();
  Map<String, Object> param = new HashMap<String, Object>(); // 검색필드와 검색어 맵핑
  String searchField = request.getParameter("searchField");
  String searchWord = request.getParameter("searchWord");
  if(searchWord != null){ //검색어가 있다면 파라미터로 넘겨준다
  param.put("searchField", searchField);
  param.put("searchWord", searchWord);
  }

  List<ClassDTO> classLists = dao.selectList(param); //list 출력
%>
<!DOCTYPE html>
<html>
<head>
  <meta charset = "UTF-8">
    <title>강의 조회</title>
</head>
<body>
<h2> 수강편람</h2>
<h3>같은 수업 번호일 경우 요일이 달라도 하나만 신청하면 됩니다.</h3>
<button type="button" onclick="location.href = 'main.jsp';">메인으로 가기</button>
<form method="get">
<table border = "1" width="90%">
  <tr>
    <td align = "center">
      <select name = "searchField">
        <option value ="name">교과목명</option>
        <option value="class_id">수업번호</option>
        <option value="course_id">학수번호</option>
      </select>
      <input type = "text" name = "searchWord"/>
      <input type = "submit" value = "검색하기"/>
    </td>
  </tr>
</table>
</form>

<table border = "1" width = "90%">
  <tr>
    <th width="7%">수업번호</th>
    <th width="7%">학수번호</th>
    <th width="7%">교과목명</th>
    <th width="7%">교강사</th>
    <th width="7%">신청인원</th>
    <th width="7%">수강정원</th>
    <th width="7%">시작시간</th>
    <th width="7%">종료시간</th>
    <th width="7%">요일</th>
    <th width="7%">강의실</th>
    <th width="7%">강의동</th>
    <th width="7%">신청</th>
    <th width="7%">희망</th>
  </tr>
  <%
    if(classLists.isEmpty()){
  %>
  <tr>
    <td colspan = "5" align=center">
    등록된 강의가 없습니다.
    </td>
  </tr>
  <%} else {
    for(ClassDTO dto : classLists){   //list에 담긴 객체들 테이블 순서에 맞게 출력
  %>
  <tr align = "center">
      <td align = "center"><%= dto.getClass_id()%></td>
      <td align = "center"><%= dto.getCourse_id()%></td>
      <td align = "center"><%= dto.getClass_name()%></td>
      <td align = "center"><%= dto.getLecturer_name()%></td>
      <td align = "center"><%= dto.getPerson_num()%></td>
      <td align = "center"><%= dto.getPerson_max()%></td>
      <td align = "center"><%= dto.getBegin()%></td>
      <td align = "center"><%= dto.getEnd()%></td>
    <td align = "center"><%= dto.getDay()%></td>
      <td align = "center"><%= dto.getRoom_id()%></td>
      <td align = "center"><%= dto.getBuilding_name()%></td>
     <td><a href="enrolment.jsp?class_id=<%=dto.getClass_id()%>"
            class="btn btn_secondary" role="button">신청</a></td>
    <td><a href="basketProcess.jsp?class_id=<%=dto.getClass_id()%>"
           class="btn btn_secondary" role="button">희망</a></td>
  </tr>
  <%
      }
    }
  %>
</table>
</body>
</html>