<%@ page import="classes.ClassDTO" %>
<%@ page import="classes.ClassDAO" %>
<%@ page import="room.RoomDTO" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/02
  Time: 1:18 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>강의 개설.jsp</title>
</head>
<body>
<% //write.jsp 에서 객체 전부 받아온다.
    String course_id = request.getParameter("course_id");
    int class_id = Integer.parseInt(request.getParameter("class_id"));
    String class_name = request.getParameter("class_name");
    String lecturer_id = request.getParameter("lecturer_id");
    int room_id = Integer.parseInt(request.getParameter("room_id"));
    int person_max = Integer.parseInt(request.getParameter("person_max"));
    int major_id = Integer.parseInt(request.getParameter("major_id"));
    int class_no = Integer.parseInt(request.getParameter("class_no"));
    int credit = Integer.parseInt(request.getParameter("credit"));
    int year = Integer.parseInt(request.getParameter("year"));


    RoomDTO roomDTO = new RoomDTO();
//받아온 객체 정보로 dto 셋팅
    ClassDTO dto = new ClassDTO();
    dto.setCourse_id(course_id);
    dto.setClass_id(class_id);
    dto.setClass_name(class_name);
    dto.setLecturer_id(Long.parseLong(lecturer_id));
    dto.setPerson_max(person_max);
    dto.setRoom_id(room_id);
    dto.setOpened(2022);//강의 개설은 현재 년도로 하는 것으로 해석
    dto.setClass_no(class_no);
    dto.setCredit(credit);
    dto.setYear(year);
    dto.setMajor_id(major_id);

    session.setAttribute("class_id", class_id);

    ClassDAO dao = new ClassDAO(); //외래키 조건으로 class 스키마에 넣기전에 다른 스키마에 모두 넣어주어야 한다.
    int aResult = dao.checkCourse(dto);
    if(aResult == 0){  //response.sendRedirect로 하니 jsp 페이지는 넘어가지만 여기서 실행되는 코드는 계속 실행되어 오류가 발생. 포워딩을 해야한다고 생각했다.
                        //강의 개설시에 건물, 강의실, 수용인원, 중복 수업 check해서 오류 전달
%>
        <jsp:forward page="WriteError.jsp"/>
  <%  }
    //int bResult = dao.checkLecturer(dto);
    //if(bResult == 0){%>
    <%
    int cResult = dao.checkRoom(dto);
    if(cResult == 0){ %>
<jsp:forward page="WriteError.jsp"/>
    <% }

    int eResult = dao.checkPerson(dto, roomDTO); //수강인원이 강의실 수용인원보다 클 경우
    if(eResult == 0){ %>
<jsp:forward page="WriteError.jsp"/>
   <% }

    int iResult = dao.checkClass(dto);
    if(iResult == 0){ %>
        <jsp:forward page="WriteError.jsp"/>
    <% }

    dao.insertClass(dto);
    response.sendRedirect("List.jsp");
%>
</body>
</html>

