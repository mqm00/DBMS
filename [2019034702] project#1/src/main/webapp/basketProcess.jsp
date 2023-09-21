<%@ page import="java.util.List" %>
<%@ page import="basket.BasketDTO" %>
<%@ page import="basket.BasketDAO" %>
<%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/08
  Time: 12:27 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    int class_id = Integer.parseInt(request.getParameter("class_id"));
    BasketDAO dao = new BasketDAO();
    BasketDTO dto2 = new BasketDTO();
    dto2.setClass_id(class_id);
    dao.insertBasket(dto2);
    List<BasketDTO> basketLists = dao.basketList();
%>
<jsp:forward page="basket.jsp"/>
