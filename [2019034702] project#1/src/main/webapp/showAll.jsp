<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ page import="user.UserDAO" %>
<%@ page import="user.User" %>
<%@ page import="java.util.ArrayList" %>
<%@ page import="java.io.PrintWriter" %>
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html"; charset="UTF-8">
    <title>학적 조회 및 변경</title>
</head>
<body>
    <h3 style="test-align: center;">모든 사용자 정보</h3>
    <div class="container">
        <div class="row">
            <table class="table" width ="750" style="text-align: center; border: 1px solid #dddddd">
                <thead>
                <tr>
                    <th style="text-align: center;">  학번  </th>
                    <th style="text-align: center;">  성명  </th>
                    <th style="text-align: center;">  전공  </th>
                    <th style="text-align: center;">  성별  </th>
                    <th style="text-align: center;">  지도교수  </th>
                    <th style="text-align: center;">  학적  </th>
                </tr>

                </thead>
                <tbody>
                <%
                    UserDAO userDAO = new UserDAO();
                    ArrayList<User> list = userDAO.showAll();

                    for(int i = 0; i < list.size(); i++) {
                %>

                <tr>
                    <td><%= (i + 1)%></td>
                    <td><%= list.get(i).getUserID()%></td>
                    <td><%= list.get(i).getUserPassword()%></td>
                    <td><%= list.get(i).getUserName()%></td>
                    <td><%= list.get(i).getUserGender()%></td>
                    <td><%= list.get(i).getUserEmail()%></td>
                    <td>
                        <input type="checkbox">
                    </td>
                </tr>

                <%
                    }
                %>

                </tbody>
            </table>
        </div>
    </div>


</body>
</html>