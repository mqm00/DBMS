#include <bits/stdc++.h>
using namespace std;

class name_age {
	public:
		string name;
		string age;

		void set_name_age(string tuple)
		{
			stringstream tuplestr(tuple);
			//string agestr;

			getline(tuplestr, name, ',');
			getline(tuplestr, age);
		}
};

class name_salary {
	public:
		string name;
		string salary;

		void set_name_salary(string tuple)
		{
			stringstream tuplestr(tuple);
			//string salarystr;

			getline(tuplestr, name, ',');
			getline(tuplestr, salary);
		}
};

string make_tuple(string name, string age, string salary)
{
	return name+ ',' + age + ',' + salary + '\n';
}

int main(){

	string buffer[2]; //relation 2개를 여기다 할당하는 건가?
	name_age temp0; //tuple 나눠주는 거겠지
	name_salary temp1;
	int current_block[2] = {};
	fstream block[12]; //주어진 데이터 여기다 다 넣고 buffer에 하나씩 불러와서 join?
	ofstream output;
	output.open("./output1.csv");

	if(output.fail()) {
        cout << "output file opening fail.\n";
    }

	/*********************************************************************************/
    //변수를 잘 구분해야함
    //1. csv파일 이름에 대한 변수, 2. csv파일을 block에 올렸을 때 그 block의 index, 3. 한 블락 안에서 튜플을 나타낼 변수
    //1번
    int csvNameAge=0;
    int csvNameSalary=0;
    //2번
    int blockNameAge = 5;
    int blockNameSalary = 10;
    //3번
    int tupleNameAge=0;
    int tupleNameSalary=0;
    int check = 0;
    //필요한 Operation
    // 1. csv파일을 block에 올리는 것 2.block 하나를 다 읽으면 다음 block으로 넘어가는 것 3. tuple을 순서대로 읽는 것
    //1번 block 0번~4번은 nameAge, block5~9번은 nameSalary
    while(csvNameAge <= 995 && csvNameSalary <= 1000){
        check++;
        if(blockNameAge == 5){
            for(blockNameAge=0; blockNameAge<5; blockNameAge++) {
                block[blockNameAge].close();
                block[blockNameAge].open("/Users/seungsu/(Ass2)problem/case1/name_Age/" + to_string(csvNameAge) + ".csv", ios::in);
                csvNameAge++;
            }
            blockNameAge = 0;
        }
        if(blockNameSalary == 10){
            for(blockNameSalary=5; blockNameSalary<10; blockNameSalary++) {
                block[blockNameSalary].close();
                block[blockNameSalary].open(
                        "/Users/seungsu/(Ass2)problem/case1/name_salary/" + to_string(csvNameSalary) + ".csv", ios::in);
                csvNameSalary++;
            }
            blockNameSalary = 5;
        }
        //blockNameAge = 0;
        //blockNameSalary = 5;
        //2번 비교대상이 되는 block을 buffer에 업데이트해주는 loop -> 위에서 파일을 open한 block을 다 썼다면 다시 가서 가져온다.
        while(blockNameAge < 5 && blockNameSalary < 10) {
            if (tupleNameAge == 0) {
                getline(block[blockNameAge], buffer[0]);
            } //inner가 먼저 넘어가기때문에 첫시작은 둘 다 0이어야한다는 조건으로 수정
            if (tupleNameSalary == 0 && tupleNameAge == 0) {
                getline(block[blockNameSalary], buffer[1]);
            }
            //3번 tuple 하나씩 가져와서 비교
            while (true) {
                if (block[blockNameAge].eof()) {
                    tupleNameAge = 0;
                    blockNameAge++;
                    break;
                }
                if (block[blockNameSalary].eof()) {
                    tupleNameSalary = 0;
                    blockNameSalary++;
                    break;
                }

                temp0.set_name_age(buffer[0]);
                temp1.set_name_salary(buffer[1]);

                if ((temp0.name).compare(temp1.name) == 0 && !temp0.name.empty()) {
                    output.write(make_tuple(temp0.name, temp0.age, temp1.salary).c_str(),make_tuple(temp0.name, temp0.age, temp1.salary).size());
                    getline(block[blockNameSalary], buffer[1]);
                    tupleNameSalary++;
                } else if ((temp0.name).compare(temp1.name) < 0) {
                    getline(block[blockNameAge], buffer[0]);
                    tupleNameAge++;
                } else {
                    getline(block[blockNameSalary], buffer[1]);
                    tupleNameSalary++;
                }

            }
        }

    }



	/*********************************************************************************/
    
	output.close();
}

