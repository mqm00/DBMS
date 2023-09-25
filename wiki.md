# Web/Database

## 개발환경

---

IntelliJ IDEA 2022.2

mySQL workbench 8.0

Tomcat 9.0.65

SDK Amazon corretto version 15.0.2

Connector mysql-connertor-java-8.0.20.jar

## 실행환경

---

MacOS Monterey

IntelliJ IDEA 

사파리

## 개요

---

1. 데이터 베이스를 이용해 수강신청 사이트를 만든다.
2. 관리자와 사용자가 접근할 수 있는 부분을 구분한다.
3. 수강신청 이외에도 학적 조회 및 변경 등의 기타 기능들도 구현한다.
4. 수업 시간표를 만들어 주는 기능을 구현한다.

## 사이트 디자인 구성 계획

---

![스크린샷 2022-10-31 오후 5.25.45.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-10-31_%25EC%2598%25A4%25ED%259B%2584_5.25.45.png)

> 메인 페이지
> 

- 학생과 관리자의 명확한 역할 구분이 있음을 보이고 싶어 처음부터 구분해놓았다.

![스크린샷 2022-10-31 오후 5.30.50.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-10-31_%25EC%2598%25A4%25ED%259B%2584_5.30.50.png)

> 학생 페이지
> 
- 수강 편람과 수강 신청, 나의 시간표, 로그아웃 탭을 각각 따로 만들었다. 이 부분은 아직 구현을 못했지만 링크를 이용해 어떤 탭에 들어가도 위에서 바로 다른 탭으로 이동할 수 있게 만들고자 한다.

![스크린샷 2022-10-31 오후 5.32.51.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-10-31_%25EC%2598%25A4%25ED%259B%2584_5.32.51.png)

- 수강 편람 탭에선 교과목명, 학수 번호, 수업 번호로 각각 검색할 수 있게 만들었다.

*** 수강 신청 페이지와 시간표 페이지는 아직 구현하지 못하였다. 현재 만든 것과 비슷하게 만들어질 것 같은데 너무 딱딱해 보여서 조금 더 부드럽고 깔끔한 이미지로 만드는 것이 좋을 것 같다.

*** 신청버튼, 희망수업 버튼 추가하기

![스크린샷 2022-10-31 오후 5.36.13.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-10-31_%25EC%2598%25A4%25ED%259B%2584_5.36.13.png)

> 관리자 페이지
> 
- 관리자 페이지 또한 각각의 기능을 나누어서 접근할 수 있도록 디자인 할 계획이다.

## DB Schema 디자인

---

![pro1_ERmodel.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/pro1_ERmodel.png)

---

### student - 학생

- 기본 인적 사항 (학번, 이름, 성별)
- 추가 사항(전공, 지도교수, 학년)

### basket - 장바구니

- 희망한 강의

### time - 시간

- 각 강의의 요일 및 시간

### credits - 학점

- 해당 강의의 학점, 이수  년도 등을 포함

### enrolment - 수강 신청

- 학번에 따른 수강신청 목록 표현

### course - 과목

- 전공에 따른 과목

### major - 전공

- 교강사와 학생의 전공 표현

### lecturer - 교강사

- 교강사 번호와 기본 인적 사항

### class - 수업

- 기본 사항 (수업 번호, 학수번호, 교과목 명, 개설 전공, 학년, 강의실 등)

### room - 강의실

- 강의가 진행되는 구체적인 강의실

### building - 강의동

- 강의실들이 모여있는 건물

### admin - 관리자

- 수강신청 페이지 및 학생 정보 관리

### user - 사용자

- 수강신청 홈페이지 사용자(학생)

## SQL 코드

---

### 검색 기능

```sql
SELECT * FROM class WHERE name like '%(input)%'; -- 교과목은 키워드 포함 검색
SELECT * FROM class WHERE course_id = '(input)'; -- 학수번호 완전 일치 검색
SELECT * FROM class WHERE class_id = '(input)'; -- 수업번호 완전 일치 검색
```

### 관리 기능

```sql
--조건 1
select class_id from class A WHERE A.person_max < some(select count(student_id) from takes group by class_id having A.class_id);
-- 조건 2
create view notenrol as select class_id from class A 
where A.person_max < some(select count(student_id) from takes group by class_id having A.class_id)
select class_id from enrolment where class_id in (select class_id from notenrol);
```

### 수강신청 기능

```sql
-- 수강신청 기능
Insert into enrolment Values (student_id, class_id);
-- 신청 버튼 누르면 저장했다가 위 쿼리 부르기
delete from enrolment where class_id = (select class_id from taken where grade > 'B0');
-- B0 이상이라는 조건 정확하게 만들기
-- 조건2
select class_id from class where person_max = takes;
-- 조건3 시간 부분 다시 공부하기
-- 조건4 
select student_id from enrolment where sum(credit) > 18 group by student_id;
```

### 수업 시간표 조회

```sql
-- 학생이 자신의 시간표 조회
select * from class natural join enrolment where student_id = '현재 로그인된 ID';

--관리자가 학생들의 시간표 조회
select * from (student natural join (class natural join enrolment));
```

### OLAP

```sql
학생의 GPA를 구하는 View
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `db2019034702`.`gpa` AS select `db2019034702`.`credits`.`student_id` AS `student_id`,avg(`db2019034702`.`credits`.`grade`) AS `eachPersonGPA` from `db2019034702`.`credits` group by `db2019034702`.`credits`.`student_id`
성적과 학생의 GPA를 뺀 View 
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `db2019034702`.`diff` AS select `db2019034702`.`credits`.`student_id` AS `student_id`,`db2019034702`.`credits`.`credits_id` AS `credits_id`,`db2019034702`.`credits`.`course_id` AS `course_id`,(`db2019034702`.`gpa`.`eachPersonGPA` - `db2019034702`.`credits`.`grade`) AS `realDiff` from (`db2019034702`.`credits` join `db2019034702`.`gpa` on((`db2019034702`.`credits`.`student_id` = `db2019034702`.`gpa`.`student_id`)))
학생들의 GPA에서 성적을 뺀 것의 평균을 구한다
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `db2019034702`.`olap` AS select `db2019034702`.`diff`.`course_id` AS `course_id`,avg(`db2019034702`.`diff`.`realDiff`) AS `answer` from `db2019034702`.`diff` group by `db2019034702`.`diff`.`credits_id` order by `answer` desc limit 10

```

adminDAO

```java

public int adminLogin(String adminID, String adminPassword); 관리자가 로그인할 때

```

basketDAO

```java

public List<BasketDTO> baksetList(); 희망강의들을 테이블에 반복문으로 출력하기 위해 list에 담는 함수를 작성
public int insertBasket(BasketDTO basketDTO); 희망 버튼을 누르면 DB에 반영하기 위한 함수

```

classDAO

```java

public List<ClassDTO> selectList(Map<String, Object> map); 수강편람 클릭시 교과목 조회 함수
map의 searchWord는 검색 단어, searchField는 검색 분야 (ex. 학수번호, 수업번호, 과목명)

if(dto.getOpened() < 2022) continue; 2022년 이전에 개설되었던 교과목은 수강신청이 불가하므로 제외
 
```

classDAO - check

```java
check 목록
course, building, room, person, class

course - 이미 등록되어있는 학수번호에 속한 새로운 수업을 만든다고 해석하여서 학수번호는 주어진 data에 있는 것이어야 한다.
public int checkCourse(ClassDTO dto)

building도 data 안에 있는 것이어야만 한다.
public int checkBuilding(ClassDTO dto)

room도 data로 주어진 것으로만 한정
public int checkRoom(ClassDTO dto)

수강인원이 강의실 인원보다 클 수 없으므로 강의실 최대수용인원을 넘으면 개설 불가
public int checkPerson(ClassDTO dto, RoomDTO roomDTO)

인원조건 충족되었을 시 insert
public int insertPerson(ClassDTO dto)

중복된 class_id를 갖으면 안되기 때문에 확인
public int checkClass(ClassDTO dto)

중복된 class_id 없을 시 insert
public int insertClass(ClassDTO dto)

```

classDAO - delete

```java
삭제할 강의 특정해서 ClassDTO에 저장한 후에 ClassDTO를 가지고 delete 진행
public ClassDTO selectView(int class_id)

강의삭제
public int deleteClass(ClassDTO dto)
수강신청한 목록에서 삭제
String query2 = "delete from enrolment where class_id=?"; 
class에서 삭제
String query = "delete from class where class_id=?";
```

creditsDAO

```java
평점 평균과 특정 과목의 학점 간 차이(평점평균-과목학점)가 가장 큰 Top10 과목 추출
olap 함수에서 진행
public List<CreditsDTO> olap() 
DB에 olap이라는 View를 만들어서 바로 가져올 수 있게 함수의 logic을 만듦
10개의 항목을 저장해야하므로 List에 담는다.

학생들의 평점을 뽑아내는 함수
연도별로 묶고 소수점 두번째자리까지만 출력되도록
public List<CreditsDTO> avgList(int student_id)
```

enrolDAO

```java
수강신청 목록 출력
public List<EnrolDTO> enrolList() 

수강신청인 enrolment와 lecturer, class를 join한 View 생성
String query2 = "select * from enrolView"; 

수강신청에 대한 조건처리
public int checkEnrol(EnrolDTO dto, ClassDTO classDTO) 

중복신청 확인
String query = "select class_id from enrolment where userID = ? and class_id = ?";

수강정원 확인
String query = "select person_num, person_max from class where class_id = ?";

이수가능한 최대학점 확인
String query = "select sum(credit) as max_credit from enrolment where userID = ? group by userID";

수강신청에 추가
public int insertEnrol(EnrolDTO dto, ClassDTO classDTO)

수업 학점 가져오기
String query = "select credit from class where class_id = ?";

신청목록에 삽입
String query = "Insert INTO enrolment (class_id, userID, credit) Values (?, ?, ?)";

클래스 목록에서 정원 업데이트
String query = "UPDATE class SET person_num = ? where class_id = ?";
```

roomDAO

```java
roomDAO에는 다른 사용된 함수는 없다
다만, roomDTO에서 방에 설정된 최대 수용 가능 인원을 얻어오는 역할을 하고있다.
public int getOccupancy() {
        return occupancy;
    }

    public void setOccupancy(int occupancy) {
        this.occupancy = occupancy;
    }
```

studentDAO

```java
학생 정보 수정
public int studentEdit(StudentDTO dto){
        int result = 0;
        try {
            String query = "UPDATE student SET status = ? WHERE student_id = ?";

학생 목록 가져오기
public List<StudentDTO> stuList(Map<String, Object> map)

이름 혹은 학번으로도 검색가능
if(map.get("searchWord") != null){ //이름 혹은 학번으로 검색가능
            query2 += " WHERE student." + map.get("searchField") + " " +  "LIKE '%" + map.get("searchWord") + "%'";
        }

학생 정보 한명씩 가져오기, 학생정보 수정에서 정보 가져오는 데 사용
public StudentDTO selectView(int student_id)

학번에 따른 성적 검색하기, 연도별로 검색
public List<StudentDTO> gradeList(int student_id){
```

userDAO

```java
로그인 함수
public int login(String userID, String userPassword)

DB에 등록된 아이디에 따른 비밀번호 select후 입력된 비밀번호와 비교
String SQL = "SELECT userPassword FROM USER WHERE userID = ?";

회원가입
public int join(User user)
```

## TroubleShooting

1. request.getParameter() 와 session.getAtrribute()

 jsp를 이동할 때, 객체를 전달하고 전달받는 것에서 오류가 많이 남

html에 대한 이해도가 부족하여 정보는 객체로 전달해야한다는 것을 몰라서 오류가 많이 났었다.

request.getParameter는 form태그에서 input이 있고 특정 Jsp로 submit을 했을 때 받는 것

session.getAttribute는 하나의 session, 즉 하나의 브라우저에서 정보를 공유할 때 사용

form 태그와 request는 내가 이해한 것보다 더 많은 기능이 있어서 조금 더 공부를 한다면 session과 request를 적소에 사용할 수 있을 것 같다. 이번에 이 둘에 대한 이해가 부족한 상태로 과제를 진행했는데 과제를 마칠 때쯤 되니 용도가 조금씩 보이기 시작해서 조금만 더 빨리 깨달았으면 더 효율적이고 깔끔하게 코드를 짤 수 있었을 것 같다는 아쉬움이 들었다.

1. executeUpdate()

스키마를 삽입/삭제/업데이트할 때는 executeQuery()가 아니라 executeUpdate()를 써야함을 알지 못하여서 업데이트하는 부분에서 에러가 많이 발생했다.

 executeUpdat는 성공시에 return값 1을 반환하기 때문에 이를 이용해 예외처리 또한 자연스럽게 가능했다.

1. response.sendRedirect()와 <jsp:forward page=”~.jsp”>

코드 진행 중에 조건이 맞았을 때 다른 jsp로 넘어가는 기능을 구현하고 싶었다.

하지만 계속해서 에러가 났고 디버깅 결과 response.sendRedirect가 호출된 이후에도 나머지 코드가 그래도 실행되었다. 검색 결과 sendRediect는 코드를 계속 진행하는 것이 맞았고 내가 원하는 함수는 forwarding을 해주어야 하는 것이었다. 그래서 `<*jsp:forward page*="~.jsp"/>`를 사용하여 이 문제를 해결하였다.

하지만 조건문안에 forward태그를 넣어놨을 때 에러가 계속해서 났고 디버깅 결과 다른 것들은 실행되지 않고 forward 페이지로 넘어가는 것처럼 보여서 forward태그에 대해 검색하였다.

- JSP 페이지 내에서 foward 액션 태그를 만나면그전까지 출력 버퍼에 저장되어 있던 내용을 제거한 후 지정 페이지로 이동

다음과 같은 것을 알 수 있었다. 처음 forward태그를 사용했을 땐 이 함수만 사용하면 되겠다고 생각했는데 함수에 대한 이해없이 코드를 짠 후에 후회를 많이 하였다. 앞으로 라이브러리나 API를 사용할 때 그것에 대한 이해가 먼저가 되어야함을 느꼈다. 

이에 대한 오류는 form 태그&submit으로 넘기거나, session.setAttribute&OnClick으로 넘기는 것으로 해결하였다.

1. 수강편람에서 각 row에 대한 정보를 신청페이지로 넘기기(<form>에 대한 이해)

수강편람에서 수강신청 버튼을 만들고 버튼을 누르면 그 row에 있는 정보를 객체로 enrolment에 전달하고 싶었다. 처음에는 radio button 으로 구현을 하였는데 어떤 row를 선택해서 눌러도 첫번째 row에 있는 정보가 전달되었다. button의 문제인 줄 알고 checkbox, submit 등 모든 type의 버튼을 사용했는데도 해결되지 않아서 객체 전달에 관한 것을 검색했다. 그 결과 하나의 form 태그에 대해서 여러 정보를 넘기는 것 자체가 좋은 방법은 아니라는 것을 알았고 태그의 문제임을 알았다. form 태그를 잘 이용해 정보를 넘기려했는데 잘되지 않아서 결국 

```
<a href="enrolment.jsp?class_id=<%=dto.getClass_id()%>"
class="btn btn_secondary"role="button">신청</a>
```

<a> 태그와 href를 이용해 특정 페이지로 이동하고 뒤에 필요한 정보를 붙여서 request로 정보를 받을 수 있게 만들었다.

 

## 빌드 과정 및 실행 과정

![스크린샷 2022-11-08 오후 2.36.50.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_2.36.50.png)

run configuration을 TOMCAT 9.0.65로 맞춘 후 Run

### 실행 결과

### 수강편람 기능

신청버튼, 수업번호, 학수번호, 교과목명, 교강사 이름, 수업시간, 신청인원, 수강정원, 강의실, 강의동

![스크린샷 2022-11-08 오후 3.46.00.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_3.46.00.png)

- 교과목명 키워드 포함 검색

![스크린샷 2022-11-08 오후 3.46.31.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_3.46.31.png)

- 수업번호 완전 일치 검색
    
    ![스크린샷 2022-11-08 오후 3.46.55.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_3.46.55.png)
    
- 학수번호 완전 일치 검색
    
    ![스크린샷 2022-11-08 오후 3.48.57.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_3.48.57.png)
    
    ### 수강신청 기능
    
    - 신청 전
    
    ![스크린샷 2022-11-08 오후 3.57.44.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_3.57.44.png)
    
    - 수업번호 8837 신청 후 신청인원 변경

![스크린샷 2022-11-08 오후 3.58.15.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_3.58.15.png)

![스크린샷 2022-11-08 오후 3.58.22.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_3.58.22.png)

- 수강 내역에 추가
    
    ![스크린샷 2022-11-08 오후 5.33.37.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_5.33.37.png)
    
    - 최대 학점인 18학점을 초과하여 신청할 경우
    
    ![스크린샷 2022-11-08 오후 4.55.41.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_4.55.41.png)
    
    - 최대 신청 인원을 초과할 경우 (첫번째 row)
        
        ![스크린샷 2022-11-08 오후 4.57.49.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_4.57.49.png)
        
        ![스크린샷 2022-11-08 오후 4.58.22.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_4.58.22.png)
        
        - 수강 신청 취소
            
            ![스크린샷 2022-11-08 오후 5.33.53.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_5.33.53.png)
            
            ![스크린샷 2022-11-08 오후 5.34.13.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_5.34.13.png)
            
            ![스크린샷 2022-11-08 오후 5.34.20.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_5.34.20.png)
            
        
        ## 관리기능
        
        ### 설강 및 폐강
        
        학수번호, 교과목이름, 강의실 번호, 건물 번호, 교강사번호는 주어진 데이터 안에서만 입력할 수 있다.
        
        ![스크린샷 2022-11-08 오후 5.35.38.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_5.35.38.png)
        
        ![스크린샷 2022-11-08 오후 6.27.13.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.27.13.png)
        
        ![스크린샷 2022-11-08 오후 7.38.00.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_7.38.00.png)
        
        수업번호 22222가 잘 들어간 것을 확인할 수 있다.
        
        강의실의 수용 가능 인원보다 더 많은 인원은 정원으로 설정했을 때 에러페이지가 도출되고
        
        class에 추가되지 않는다.
        
        ![스크린샷 2022-11-08 오후 7.39.24.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_7.39.24.png)
        
        ![스크린샷 2022-11-08 오후 7.39.34.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_7.39.34.png)
        
        ![스크린샷 2022-11-08 오후 7.40.25.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_7.40.25.png)
        
        ![스크린샷 2022-11-08 오후 7.52.47.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_7.52.47.png)
        
        이런식으로 입력단에 input이 없으면 경고창을 뜨게 만들었다.
        
        ### 폐강
        
        삭제 전 수강편람과 학생의 신청현황
        
        ![스크린샷 2022-11-08 오후 6.33.05.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.33.05.png)
        
        ![스크린샷 2022-11-08 오후 6.35.16.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.35.16.png)
        
        ![스크린샷 2022-11-08 오후 6.35.41.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.35.41.png)
        
        ![스크린샷 2022-11-08 오후 6.35.47.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.35.47.png)
        
        ![스크린샷 2022-11-08 오후 6.36.02.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.36.02.png)
        
        강의 삭제시 수강편람과 신청/희망에서 자동으로 삭제된다
        
    
    ### 학생 정보 조회
    
    ![스크린샷 2022-11-08 오후 6.37.32.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.37.32.png)
    
    ### 학생 정보 수정하기 - 학적 상태 변경
    
    김관유 -2018004052 학번 입력
    
    ![스크린샷 2022-11-08 오후 6.38.38.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.38.38.png)
    
    ![스크린샷 2022-11-08 오후 6.38.52.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.38.52.png)
    
    ![스크린샷 2022-11-08 오후 6.39.00.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.39.00.png)
    
    ![스크린샷 2022-11-08 오후 6.39.06.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.39.06.png)
    
    -재학으로 작성 완료시 학적 변경
    
    ### 성적 조회
    
    ![스크린샷 2022-11-08 오후 6.39.36.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.39.36.png)
    
    ![스크린샷 2022-11-08 오후 6.39.43.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.39.43.png)
    
    과목별 성적과 학년별 평균성적 산출
    
    ### OLAP - 평점 평균과 특정 과목의 학점 간 차이(평점평균-과목학점)가 가장 큰 Top10 과목 추출
    
    ![스크린샷 2022-11-08 오후 6.41.05.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_6.41.05.png)
    
    메인에서 강의통계 버튼 누르면 산출
    
    ### 희망수업
    
    ![스크린샷 2022-11-08 오후 7.23.30.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_7.23.30.png)
    
    수업번호 8840 희망 버튼 누를시
    
    다음과 같이 추가
    
    이전에 희망 목록에 추가했던 수업들도 모두 보여준다
    
    ![스크린샷 2022-11-08 오후 7.23.58.png](Web%20Database%2080ae7b72557e4251abe89c74d1e8aa74/%25EC%258A%25A4%25ED%2581%25AC%25EB%25A6%25B0%25EC%2583%25B7_2022-11-08_%25EC%2598%25A4%25ED%259B%2584_7.23.58.png)