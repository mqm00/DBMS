<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ page import="java.io.PrintWriter" %>
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html"; charset="UTF-8">
    <title>메인페이지</title>
</head>
<body>

	<%
		String userID = null;
		String adminID = null;
		if(session.getAttribute("adminID") != null){		//admin 정보가 session에 있다면 가져온다
			adminID = (String) session.getAttribute("adminID");
		}
		if(session.getAttribute("userID") != null) //user정보가 session에 있다면 가져온다
		{
			userID = (String) session.getAttribute("userID");
		}
	%>
	<h3 style="test-align: center;">메인페이지</h3>
	<%
		if(adminID != null) //admin으로 로그인할 경우
		{ session.setAttribute("adminID", adminID);
	%>
	<button type="button" class="navyBtn" onClick="location.href='Write.jsp'">강의 조회 및 변경</button>
	<button type="button" class="navyBtn" onClick="location.href='studentList.jsp'">학적 조회 및 변경</button>
	<button type="button" class="navyBtn" onClick="location.href='olap.jsp'">강의 통계</button>
	<button type="button" class="navyBtn" onClick="location.href='logoutAction.jsp'">로그아웃</button>
	<%
	}
		else if(userID == null && adminID == null) //누구도 로그인 안했을 경우
		{
	%>
		<button type="button" class="navyBtn" onClick="location.href='login.jsp'">학생 로그인</button>
		<button type="button" class="navyBtn" onClick="location.href='adminlogin.jsp'">관리자 로그인</button>
		<button type="button" class="navyBtn" onClick="location.href='join.jsp'">회원가입</button>
	
	<%		
		}
		else //user로 로그인했을 경우
		{
	%>
	<button type="button" class="navyBtn" onClick="location.href='List.jsp'">수강편람</button>
	<button type="button" class="navyBtn" onClick="location.href='basket.jsp'">희망수업</button>
	<button type="button" class="navyBtn" onClick="location.href='enrolList.jsp'">신청현황</button>
	<form name = "schedule" method = "post" action="schedule.jsp">
		<input type="hidden" value="userID">
		<button type="submit" class="navyBtn" onClick="location.href='schedule.jsp'">나의 시간표</button>
	</form>
	<button type="button" class="navyBtn" onClick="location.href='logoutAction.jsp'">로그아웃</button>
<% } %>

</body>
</html>