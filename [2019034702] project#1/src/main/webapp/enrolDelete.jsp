<%@ page import="enrolment.EnrolDTO" %>
<%@ page import="enrolment.EnrolDAO" %>
<%@ page import="java.io.PrintWriter" %>
<%@ page import="classes.ClassDTO" %><%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/08
  Time: 5:08 PM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
int class_id = Integer.parseInt(request.getParameter("class_id")); //삭제할 class_id 객체를 넘겨받음
EnrolDTO dto = new EnrolDTO();
EnrolDAO dao = new EnrolDAO();
ClassDTO classDTO = new ClassDTO();
classDTO.setClass_id(class_id);
dto.setClass_id(class_id);
dto.setUserID((String)session.getAttribute("userID"));
int delResult = 0;
delResult = dao.deleteEnrol(dto, classDTO); //delete 쿼리가 제대로 executeUpdate되면 return값으로 1을 받아온다

if(delResult == 1){
    PrintWriter script = response.getWriter();
    script.println("<script>");
    script.println("alert('수강 신청이 취소되었습니다.')");
    script.println("location.href = 'enrolList.jsp'");
    script.println("</script>");
}
else if(delResult == 0){
    PrintWriter script = response.getWriter();
    script.println("<script>");
    script.println("alert('수강 신청취소가 실패하였습니다')");
    script.println("location.href = 'enrolList.jsp'");
    script.println("</script>");
}
%>
