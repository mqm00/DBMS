<%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/02
  Time: 12:27 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="student.StudentDAO" %>
<%@ page import="student.StudentDTO" %>
<%@ page import="java.util.List" %>
<%
    int student_id = Integer.parseInt(request.getParameter("student_id"));
    StudentDAO dao = new StudentDAO();
    StudentDTO dto = dao.selectView(student_id);

%>
<html>
<head>
    <meta charset="UTF-8">
    <titl>학적 조회 및 번경</titl>
    <script type = "text/javascript">
        function validateFrom(form){
            if(form.status.value == ""){ //form에서 넘어가는 status value값이 없을 때
                alert("학적을 입력하세요");
                form.status.focus(); //status가 설정되어있는지
                return false;
            }
        }
    </script>
</head>
<body>
  <h2>학생 정보 수정하기</h2>
<form name = "writeFrm" method = "post" action = "EditProcess.jsp" onsubmit = "return validateForm(this);"> <!-- editProcess로 넘어가는 form -->
  <input type = "hidden" name = "student_id" value = "<%= dto.getStudent_id()%>" /> <!-- student_id를 항상 넘기기위해 hidden type으로 input -->
    <table border="1" width="90%">
        <tr>
            <td align="center">학적 변경</td>
            <td>
                <input type='radio' name=status value="attendance" />재학
            </td>
            <td>
                <input type='radio' name=status value="absence" />휴학
            </td>
            <td>
                <input type='radio' name=status value="leaved" />자퇴
            </td>
            <td align="center">
                <button type = "submit">작성 완료</button>
            </td>
            <td>
                <button type = "reset">초기화</button>
            </td>
            <td>
                <button type = "button" onclick = "location.href='studentList.jsp'; ">목록보기</button>
            </td>
        </tr>
    </table>
</form>
</body>
</html>
