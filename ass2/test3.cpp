#include <bits/stdc++.h>
using namespace std;

class name_grade {
public:
    string student_name;
    int korean;
    int math;
    int english;
    int science;
    int social;
    int history;

    void set_grade(string tuple)
    {
        stringstream tuplestr(tuple);
        string tempstr;

        getline(tuplestr, student_name, ',');

        getline(tuplestr, tempstr, ',');
        korean = stoi(tempstr);

        getline(tuplestr, tempstr, ',');
        math = stoi(tempstr);

        getline(tuplestr, tempstr, ',');
        english = stoi(tempstr);

        getline(tuplestr, tempstr, ',');
        science = stoi(tempstr);

        getline(tuplestr, tempstr, ',');
        social = stoi(tempstr);

        getline(tuplestr, tempstr);
        history = stoi(tempstr);
    }
};

class name_number{
public :
    string student_name;
    string student_number;

    void set_number(string tuple)
    {
        stringstream tuplestr(tuple);
        string tempstr;


        getline(tuplestr, student_name, ',');
        getline(tuplestr, student_number, ',');
    }
};

string make_tuple(string name, string number)
{
    string ret = "";

    ret += name+ "," + number +"\n";

    return ret;
}

int rolHash(string name){ //polynomial rolling hash function
    int p = 31;
    int mod = 1e9 + 9;
    int hashValue = 0;
    int pow = 1;

    for(int i = 0; i < name.length(); i++){
        hashValue = (hashValue + (name[i] -'a'+1) * pow) % mod;
        pow = (p * pow) % mod;
    }
    return hashValue;
}

int compGrade(name_grade temp0, name_grade temp1){
    int valid = 0;

    if(temp0.korean > temp1.korean) valid++;
    if(temp0.math > temp1.math) valid++;
    if(temp0.english > temp1.english) valid++;
    if(temp0.science > temp1.science) valid++;
    if(temp0.social > temp1.social) valid++;
    if(temp0.history > temp1.history) valid++;

    if(valid >= 2) return 1;
    else return 0;
}

int main(){

    string buffer[2];
    name_grade temp0;
    name_grade temp1;
    name_number temp2;
    fstream block[12];
    ofstream output;

    output.open("./output3.csv");

    if(output.fail())
    {
        cout << "output file opening fail.\n";
    }

    /*********************************************************************/
    int csvGrade1 = 0;
    int csvGrade2 = 0;
    int csvNumber = 0;
    int blockIndex = 0;
    int grade1_hashing = 0;
    int grade2_hashing = 0;
    int number_hashing = 0;
    output.close();

    //1학기 hashing
    while (csvGrade1 < 1000) {
        for (blockIndex = 0; blockIndex < 10; blockIndex++) {
            block[blockIndex].open("/Users/seungsu/(Ass2)problem/case3/name_grade1/" + to_string(csvGrade1) + ".csv",ios::in);
            csvGrade1++;
        }

        for (blockIndex = 0; blockIndex < 10; blockIndex++) {
            while (!block[blockIndex].eof()) {
                getline(block[blockIndex], buffer[0]);
                if (buffer[0].empty()) break;
                temp0.set_grade(buffer[0]);
                grade1_hashing = rolHash(temp0.student_name);

                output.close();
                output.open("/Users/seungsu/(Ass2)problem/case3/buckets_grade/" + to_string(grade1_hashing) + ".csv",ofstream::out | ofstream::app);
                output << temp0.student_name << "," << temp0.korean << "," << temp0.math << "," << temp0.english << "," << temp0.science << "," << temp0.social << "," << temp0.history << endl;
            }
            block[blockIndex].close();
        }
    }
    output.close();

    //학번 hashing
    while (csvNumber < 1000) {
        for (blockIndex = 0; blockIndex < 10; blockIndex++) {
            block[blockIndex].open("/Users/seungsu/(Ass2)problem/case3/name_number/" + to_string(csvNumber) + ".csv",ios::in);
            csvNumber++;
        }

        for (blockIndex = 0; blockIndex < 10; blockIndex++) {
            while (!block[blockIndex].eof()) {
                getline(block[blockIndex], buffer[1]);
                if (buffer[1].empty()) break;
                temp2.set_number(buffer[1]);
                number_hashing = rolHash(temp2.student_name);

                output.close();
                output.open("/Users/seungsu/(Ass2)problem/case3/buckets_number/" + to_string(number_hashing) + ".csv",
                            ofstream::out | ofstream::app);
                output << temp2.student_name << "," << temp2.student_number << endl;
            }
            block[blockIndex].close();
        }
    }
    output.close();

    output.open("./output3.csv");

    while (csvGrade2 < 1000) {
        for (blockIndex = 0; blockIndex < 10; blockIndex++) {
            block[blockIndex].open("/Users/seungsu/(Ass2)problem/case3/name_grade2/" + to_string(csvGrade2) + ".csv", ios::in);
            csvGrade2++;
        }

        for (blockIndex = 0; blockIndex < 10; blockIndex++) {
            while (!block[blockIndex].eof()) {
                getline(block[blockIndex], buffer[1]);
                if (buffer[1].empty()) break;
                temp1.set_grade(buffer[1]);
                grade2_hashing = rolHash(temp1.student_name);

                block[10].close();
                block[10].open("/Users/seungsu/(Ass2)problem/case3/buckets_grade/" + to_string(grade2_hashing) + ".csv", ios::in);
                block[11].close();
                block[11].open("/Users/seungsu/(Ass2)problem/case3/buckets_number/" + to_string(grade2_hashing) + ".csv", ios::in);
                getline(block[10], buffer[0]);
                getline(block[11], buffer[1]);
                temp0.set_grade(buffer[0]);
                temp2.set_number(buffer[1]);
                if(compGrade(temp0, temp1)){
                    output << make_tuple(temp0.student_name, temp2.student_number);
                }
            }
            block[blockIndex].close();
        }
    }

    /*********************************************************************/


    output.close();


}

