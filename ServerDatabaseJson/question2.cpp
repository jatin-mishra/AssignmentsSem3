#include <iostream>
#include <string>
#include "mysql.h"
#include <cstdio>
#include <climits>

using namespace std;

#define SERVER "localhost"
#define USER "jatin"
#define PASSWORD "Ja@tin123"
#define DATABASE "eventRules"



class ABCCompany{
	int basepay;
	int hoursWorked;
public:
	void setBasePay(int basepay){
		this->basepay = basepay;
	}

	void setHoursWorked(int hours){
		this->hoursWorked=hours;
	}

	int getBasePay(){
		return this->basepay;
	}

	int getHoursWorked(){
		return this->hoursWorked;
	}
};

class employee:public ABCCompany{
string name;
string empId;
string address;
string jobRole;
string designation;
int totalAmount;
public:
	void setName(string name){
		this->name = name;
	}

	void setEmpId(string id){
		this->empId=id;
	}

	void setAddress(string address){
		this->address = address;
	}

	void setjobRole(string role){
		this->jobRole = role;
	}

	void setDesignation(string designation){
		this->designation = designation;
	}

	void settotalAmount(int totalAmount){
		this->totalAmount = totalAmount;
	}

	string getName(){
		return this->name;
	}

	string getEmpId(){
		return this->empId;
	}

	string getAddress(){
		return this->address;
	}

	string getJobRole(){
		return this->jobRole;
	}

	string getDesignation(){
		return this->designation;
	}

	int gettotalAmount(){
		return this->totalAmount;
	}
};

void error(const char* mssg){
	perror(mssg);
	exit(1);
}

void insertIntoDatabase(employee* emp,string query){
	MYSQL* connect;
	connect = mysql_init(0);

	if(!connect){
		error("mysql initialization failed");
	}
	cout<<"\n\nRULES FOR PARTICIPATION \n";
	connect = mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);

	int i=0;
	if(connect){
		cout<<"connected successfully with database"<<endl;
		// string query = "";
		int number;
		// for(string rule : rules){
			// query = "insert into " + TABLE + " values ('" + rule + "');";
			cout<<query<<endl;
			const char* sqlqruery = query.c_str();
			number = mysql_query(connect,sqlqruery);
			if(number){
				error("cannot execute sql query ");
			}
			// cout<<"Rule " << i + 1 << " : "<<rule<<endl;
			// i++;
		// }

	}
	else
		error("cannot connect to database");
mysql_close(connect);
}





void printEligibleForBonus(int max){
	MYSQL* connect;
	connect = mysql_init(0);

	if(!connect){
		error("mysql initialization failed");
	}
	// cout<<"\n\nRULES FOR PARTICIPATION \n";
	connect = mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
	int number;
	if(connect){

		// finding max
		number = mysql_query(connect,"select * from employees;");
		MYSQL_RES* res;
		MYSQL_ROW row;

        if(!number){
        	res = mysql_store_result(connect);
        	while((row = mysql_fetch_row(res))){
        		cout << (float)atoi(row[6]) << " > " << (float)(0.3)*((float)max) <<endl;
                if((float)atoi(row[6]) > (float)(0.3)*((float)max)){
                	cout<<row[0] << " "<<row[5] << " " <<row[7] <<endl;
                }
        	}
        }else{
        	cout<<"query problem "<<mysql_error(connect)<<endl;
      }
      mysql_close(connect);
}
}


int main(){
	employee* emp;
	int choice=1,totalAmount,basepay,hoursWorked,extended=0,maxm;
	string name,id,addr,jobrole,designation;
	while(choice==1){
		emp = new employee();
		cout<<"enter the name of the employee "<<endl;
		cin>>name;
		cout<<"enter the id of the employee"<<endl;
		cin>>id;
		cin.ignore();
		cout<<"enter the address "<<endl;
		getline(cin,addr);
		cout<<"enter the jobrole "<<endl;
		cin>>jobrole;
		cout<<"enter the designation "<<endl;
		cin>>designation;

		cout<<"enter the base pay "<<endl;
		cin>>basepay;
		cout<<"enter the hours worked "<<endl;
		cin>>hoursWorked;
		maxm=INT_MIN;
		if((hoursWorked <= 60) && (basepay >= 400)){
			if(hoursWorked >= 40){
				extended = 60 - hoursWorked;
				hoursWorked = 40;
			}

			totalAmount = (hoursWorked*basepay) + extended*(1.5)*(basepay);
			emp->setName(name);
			emp->setEmpId(id);
			emp->setAddress(addr);
			emp->setjobRole(jobrole);
			emp->setDesignation(designation);
			emp->setBasePay(basepay);
			emp->setHoursWorked(hoursWorked);
			emp->settotalAmount(totalAmount);
			string query;
			query = "insert into employees value('" + name + "','" + id  + "','" +  addr   + "','" + jobrole + "','" + designation  + "','" + to_string(totalAmount) + "','" + to_string(basepay) + "','" + to_string(hoursWorked + extended) + "');";
			insertIntoDatabase(emp,query);
			if(basepay>maxm){
				maxm=basepay;
			}

			cout<<"press one to insert more"<<endl;
			cin>>choice;
		}
	}

	printEligibleForBonus(maxm);
	return 0;
}