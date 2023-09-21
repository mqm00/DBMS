#include <bits/stdc++.h>
using namespace std;

class name_age {
public:
    string name;
    string age;

    void set_name_age(string tuple)
    {
        stringstream tuplestr(tuple);
        string agestr;

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
        string salarystr;

        getline(tuplestr, name, ',');
        getline(tuplestr, salary);
    }
};

string make_tuple(string name, string age, string salary)
{
    return name+ ',' + age + ',' + salary + '\n';
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

int main() {

    string buffer[2];
    name_age temp0;
    name_salary temp1;
    fstream block[12];
    ofstream output;

    output.open("./output2.csv");

    if (output.fail()) {
        cout << "output file opening fail.\n";
    }



    /******************************************************************/
    int csvNameAge = 0;
    int csvNameSalary = 0;
    int blockNameAge = 0;
    int blockNameSalary = 0;
    int ageHashing;
    int salHashing;
    output.close();

    while (csvNameAge < 1000) {
        for (blockNameAge = 0; blockNameAge < 10; blockNameAge++) {
            block[blockNameAge].open("/Users/seungsu/(Ass2)problem/case2/name_age/" + to_string(csvNameAge) + ".csv",ios::in);
            csvNameAge++;
        }

        for (blockNameAge = 0; blockNameAge < 10; blockNameAge++) {
            while (!block[blockNameAge].eof()) {
                getline(block[blockNameAge], buffer[0]);
                temp0.set_name_age(buffer[0]);
                if (temp0.name.empty()) break;
                ageHashing = rolHash(temp0.name);

                output.close();
                output.open("/Users/seungsu/(Ass2)problem/case2/buckets/" + to_string(ageHashing) + ".csv",ofstream::out | ofstream::app);
                output << temp0.name << "," << temp0.age << endl;
            }
            block[blockNameAge].close();
        }
    }
    output.close();
    output.open("./output2.csv");

    while (csvNameSalary < 1000) {
        for (blockNameSalary = 0; blockNameSalary < 10; blockNameSalary++) {
            block[blockNameSalary].open("/Users/seungsu/(Ass2)problem/case2/name_salary/" + to_string(csvNameSalary) + ".csv", ios::in);
            csvNameSalary++;
        }

        for (blockNameSalary = 0; blockNameSalary < 10; blockNameSalary++) {
            while (!block[blockNameSalary].eof()) {
                getline(block[blockNameSalary], buffer[1]);
                temp1.set_name_salary(buffer[1]);
                if (temp1.name.empty()) break;
                salHashing = rolHash(temp1.name);

                block[10].close();
                block[10].open("/Users/seungsu/(Ass2)problem/case2/buckets/" + to_string(salHashing) + ".csv", ios::in);

                getline(block[10], buffer[0]);
                temp0.set_name_age(buffer[0]);
                if(temp1.name == temp0.name){
                    output << make_tuple(temp0.name, temp0.age, temp1.salary);
                }
            }
            block[blockNameSalary].close();
        }
    }

    /******************************************************************/

    output.close();


}

