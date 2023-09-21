<%--
  Created by IntelliJ IDEA.
  User: seungsu
  Date: 2022/11/02
  Time: 12:44 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" pageEncoding="UTF-8" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>강의 개설</title>
    <script type ="text/javascript">
        function validateForm(form){ //입력칸에 제대로 입력이 되었는지 확인하는 function
            if(form.course_id.value == ""){
                alert("학수번호를 입력하세요");
                form.course_id.focus();
                return false;
            }
            else if(form.class_id.value == ""){
                alert("수업 번호를 입력하세요");
                form.class_id.focus();
                return false;
            }
            else if(form.class_name.value == ""){
                alert("교과목 이름을 입력하세요");
                form.class_name.focus();
                return false;
            }
            else if(form.lecturer_id.value == ""){
                alert("교강사 번호를 입력하세요");
                form.lecturer_id.focus();
                return false;
            }
            else if(form.room_id.value == ""){
                alert("강의실 번호를 입력하세요");
                form.room_id.focus();
                return false;
            }
            else if(form.person_max.value == ""){
                alert("최대 정원을 입력하세요");
                form.class_id.focus();
                return false;
            }
            else if(form.room_id.value == ""){
                alert("강의실 번호를 입력하세요");
                form.building_id.focus();
                return false;
            }else if(form.class_no.value == ""){
                alert("수업을 입력하세요");
                form.building_id.focus();
                return false;
            }else if(form.major_id.value == ""){
                alert("전공 번호를 입력하세요");
                form.building_id.focus();
                return false;
            }else if(form.year.value == ""){
                alert("권장 학년을 입력하세요");
                form.building_id.focus();
                return false;
            }else if(form.credit.value == ""){
                alert("강의 학점을 입력하세요");
                form.building_id.focus();
                return false;
            }
        }
    </script>
</head>
<body>
<h2>강의 개설</h2>
<form name = "writeFrm" method = "post" action="WriteProcess.jsp" onsubmit="return validateForm(this);"> <!-- function으로 인풋 확인 -->
    <table border = "1" width = "90%">
        <tr>
            <td>학수 번호</td>
            <td>
                <input type = "text" name = "course_id" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>수업 번호</td>
            <td>
                <input type = "text" name = "class_id" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>교과목 이름</td>
            <td>
                <input type = "text" name = "class_name" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>교강사 번호</td>
            <td>
                <input type = "text" name = "lecturer_id" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>강의실 번호</td>
            <td>
                <input type = "text" name = "room_id" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>수강 정원</td>
            <td>
                <input type = "text" name = "person_max" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>수업</td>
            <td>
                <input type = "text" name = "class_no" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>전공</td>
            <td>
                <input type = "text" name = "major_id" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>권장 학년</td>
            <td>
                <input type = "text" name = "year" style = "width: 90%"; />
            </td>
        </tr>
        <tr>
            <td>학점</td>
            <td>
                <input type = "text" name = "credit" style = "width: 90%"; />
            </td>
        </tr>

        <tr>
            <td colspan="2" align="center">
                <button type="submit">입력</button>
                <button type="reset">초기화</button>
                <button type="button" onclick="location.href = 'List.jsp';">목록보기</button>
            </td>
        </tr>
    </table>
</form>
    <div> <!-- div로 위 테이블과 화면 나눈 후 강의 삭제 기능 추가 -->
        <h2>강의 삭제</h2>
        <form name = "deleteFrm" method = "post" action="DeleteProcess.jsp"> <!-- class_id 객체 넘긴다 -->
            <table border = "1" width = "90%">
            <tr>
                <td>수업 번호</td>
                <td>
                    <input type = "text" name = "class_id" style = "width: 90%"; />
                </td>
            </tr>
            <tr>
                <td colspan="2" align="center">
                    <button type="submit">입력</button>
                    <button type="reset">초기화</button>
                    <button type="button" onclick="location.href = 'List.jsp';">목록보기</button>
                </td>
            </tr>
        </table>
        </form>
    </div>
</body>
</html>
