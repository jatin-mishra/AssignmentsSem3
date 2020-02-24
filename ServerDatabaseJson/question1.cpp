#include <iostream>
#include <string>
#include <vector>
#include <cstdbool>
#include <fstream>
#include <nlohmann/json.hpp>
#include "mysql.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

using namespace std;
using json = nlohmann::json;


#define SERVER "localhost"
#define USER "jatin"
#define PASSWORD "Ja@tin123"
#define DATABASE "eventRules"
#define TABLE "events" 


void error(string mssg){
	perror((const char*)&mssg);
	exit(1);
}


class Address{
private:
	string street;
	string houseNumber;
	string borough;
	string city;
	string district;
	string state;
public:
		void setStreet(string street){
			this->street = street;
	}

		void sethouseNumber(string house){
			this->houseNumber = house;
	}

		void setBorough(string bor){
			this->borough = bor;
	}

		void setCity(string City){
			this->city = City;
	}

		void setDistrict(string district){
			this->district = district;
	}

		void setState(string state){
			this->state = state;
	}


		string getStreet(){
			return this->street;
	}

		string getHouseNumber(){
			return this->houseNumber;
	}

		string getBorough(){
			return this->borough;
	}

		string getCity(){
			return this->city;
	}

		string getDistrict(){
			return this->district;
	}

		string getState(){
			return this->state;
	}


	string getAddress(){
		return "street is : " + this->getStreet() + "\nhouseNumber : " + this->getHouseNumber() + "\nborough : " + this->getBorough() + "\ncity : " + this->getCity() + "\ndistrict : " + this->getDistrict() + "\nstate : " + this->getState();
	}

	void printAddress(){
		cout<<this->houseNumber<<endl;
		cout<<this->street<<endl;
		cout<<this->borough<<endl;
		cout<<this->city<<endl;
		cout<<this->district<<endl;
		cout<<this->state<<endl;
	}

};


class Person{
private:
	string name;
	Address* address;
	string gender;
	string email;
	string mobilenumber;
	int eventList[4]={0};
public:
	Person(){

	}

	Person(string name,Address* address,string gender,string email,string mobile){
		this->name = name;
		this->address = address;
		this->gender = gender;
		this->email = email;
		this->mobilenumber = mobile;
	}


	string  getName(){
		return this->name;
	}

	string  getEmail(){
		return this->email;
	}
	string  getMobile(){
		return this->mobilenumber;
	}
	string  getGender(){
		return this->gender;
	}

	Address* getAddress(){
		return this->address;
	}

	void setName(string name){
		this->name = name;
	}

	void setEmail(string email){
		this->email = email;
	}

	void setGender(string gender){
		this->gender = gender;
	}

	void setMobile(string mb){
		this->mobilenumber = mb;
	}

	void setAddress(Address* addr){
		this->address = addr;
	}


	void setEvent(int index){
		this->eventList[index]=1;
	}

	bool isSet(int index){
		if(eventList[index]==0)
			return false;
		return true;
	}

	void printPerson(){
		cout<<"############################"<<endl;
		cout<<this->name<<endl;
		cout<<this->gender<<endl;
		cout<<this->email<<endl;
		cout<<this->mobilenumber<<endl;
		cout<<"address : \n";
		this->getAddress()->printAddress();
		cout<<"\n\n";
	}
};




class Event{
string eventName;
public:
	void setEventName(string evname){
		this->eventName = evname;
	}

	string getEventName(){
		return this->eventName;
	}
};


class Quiz : public Event{
private:
int maxMarks = 30;
int duration = 30;
string fieldOfInterest;
public:
	Quiz(){
		this->setEventName("Quiz");
	}

	int getMaxMarks(){
		return this->maxMarks;
	}

	int getDuration(){
		return this->duration;
	}

	void setFieldOfInterest(string fieldofinterest){
		this->fieldOfInterest = fieldofinterest;
	}

	string getFieldOfInterest(){
		return this->fieldOfInterest;
	}

};


void sendFileToServer(){
	int filedescriptor,number,portNumber;
	char serverip[20];
	char buffer[255];

	struct sockaddr_in serveraddr;
	struct hostent* server;

	cout<<"enter the port number"<<endl;
	cin>>portNumber;

	cout<<"enter the IP of server"<<endl;
	cin>>serverip;

	filedescriptor = socket(AF_INET,SOCK_STREAM,0);

	if(filedescriptor < 0){
		error("error in creating socket ");
	}

	server = gethostbyname(serverip);
	if(server == NULL){
		error("server not found");
	}

	bzero((char*)&serveraddr,sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	bcopy((char*)server->h_addr,(char*)&serveraddr.sin_addr.s_addr,server->h_length);
	serveraddr.sin_port = htons(portNumber);

	if(connect(filedescriptor,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		error("cannot connect to server");		
	}
	bzero(buffer,250);

	FILE* f;
	int words = 0;

	char c;
	f = fopen("participantfile.json","r");

	while((c=getc(f)) != EOF){
		fscanf(f,"%s",buffer);
		if(isspace(c) || c == '\t'){
			words++;
		}
	}

	write(filedescriptor,&words,sizeof(int));
	rewind(f);

	char ch;
	int n;
	while(ch != EOF){
		fscanf(f,"%s",buffer);
		n = write(filedescriptor,buffer,250);
		if(n<0)
			error("cannot write");
		cout<<buffer<<endl;
		ch = fgetc(f);
	}
	printf("file has been successfully sent\n");
	close(filedescriptor);
	// exit(1);
}


class paperPresentation : public Event{
// int maxMarks = 30;
int count=-1;
string topics[2];
public:
	paperPresentation(){
		this->setEventName("PaperPresentation");
	}

	void setTopic(string topic){
		this->count++;
		if(count<2)
		topics[count]=topic;
	}

	string* getTopics(){
		return this->topics;
	}
};


class posterPresentation : public Event{
// int maxMarks = 30;
int count=-1;
string topics[2];
public:
	posterPresentation(){
		this->setEventName("posterPresentation");

	}

	void setTopic(string topic){
		this->count++;
		if(count<2)
		topics[count]=topic;
	}

	string* getTopics(){
		return this->topics;
	}
};


class culturalProgram : public Event{
int duration = 15;
public:
	culturalProgram(){
		this->setEventName("culturalProgram");
	}
};



void createAndInsertEvent(){
	MYSQL* connect;
	connect = mysql_init(0);

	if(!connect){
		error("mysql initialization failed");
	}
	cout<<"\n\nRULES FOR PARTICIPATION \n";
	connect = mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
	vector<string> rules;

	rules.push_back("Participant should be above 18 yrs of Age");
	rules.push_back("Only UG4 can take part in Paper Presentation");
	rules.push_back("Paper presentation and poster presentation cannot be opted together");
	rules.push_back("Participant should take part minimum in 1 event and maximum in 3 event");
	rules.push_back("Maximum participant in Gents category must be 60% and Ladies must be 40%");
	rules.push_back("One participant should not have more than two topic for either Paper presentation or poster presentation");
	rules.push_back("Quiz can only conducted in 2nd half and max mark is 30 and time duration is 30 mins");
	rules.push_back("Paper/Poster presentation is only for 15 min, max participant is 10 can be conducted in morning");
	rules.push_back("Quiz does not require and judges and paper and poster cannot be done together.");
	rules.push_back("Cultural programs must be in the evening and a minimum duration of 1 program is 15 min and maximum 5 programs");

	int i=0;
	if(connect){
		cout<<"connected successfully with database"<<endl;
		string query;
		int number;
		for(string rule : rules){
			// query = "insert into " + TABLE + " values ('" + rule + "');";
			query = "insert into events values ('" + rule + "');";
			const char* sqlqruery = query.c_str();
			number = mysql_query(connect,sqlqruery);
			if(number){
				error("cannot execute sql query " + query);
			}
			cout<<"Rule " << i + 1 << " : "<<rule<<endl;
			i++;
		}

	}
	else
		error("cannot connect to database");

mysql_close(connect);

}


void writeIntojsonDictionary(Person* person,int count){
	cout<<"entered"<<endl;
	json newParticipant;
	// if(count!=0){
	// ifstream file("/home/jatin/oop/endSem/assign/participantfile.json");
	// cout<<"going to parse"<<endl;
	// newParticipant = json::parse(file);
	// cout<<"parsed"<<endl;
	// file.close();
	// }

	newParticipant["name"]=person->getName();
	newParticipant["gender"]=person->getGender();
	newParticipant["Email"]=person->getEmail();
	newParticipant["MobileNumber"]=person->getMobile();

	Address* personAddr;
	personAddr = person->getAddress();

	newParticipant["Address"]["street"] = personAddr->getStreet();
	newParticipant["Address"]["houseNumber"] = personAddr->getHouseNumber();
	newParticipant["Address"]["Borough"] = personAddr->getBorough();
	newParticipant["Address"]["City"] = personAddr->getCity();
	newParticipant["Address"]["District"] = personAddr->getDistrict();
	newParticipant["Address"]["State"] = personAddr->getState();
	cout<<"added "<<endl;
	// newParticipant[roll]["ParticipatingIn"]["paperPresentation"]["topics"] = {};
	// newParticipant[roll]["ParticipatingIn"]["posterPresentation"]["topics"] = {};

	string topic;
	newParticipant["ParticipatingIn"]["eventTwo"]["paperPresentation"]["topics"] = "not participating";
	newParticipant["ParticipatingIn"]["eventTwo"]["posterPresentation"]["topic"] = "not participating";
	newParticipant["ParticipatingIn"]["lastEvent"] = "not participating";
	if(!person->isSet(0)){
		newParticipant["ParticipatingIn"]["eventOne"] = "Quiz";
	}else{
		newParticipant["ParticipatingIn"]["eventOne"] = "not participating";
	}
	if(!person->isSet(1)){
		cout<<"enter the first topic you wanna have \n";
		cin>>topic;
		newParticipant["ParticipatingIn"]["eventTwo"]["paperPresentation"]["topics"] = topic;
	}else if(!person->isSet(2)){
		cout<<"enter the first topic you wanna have \n";
		cin>>topic;
		newParticipant["ParticipatingIn"]["eventTwo"]["posterPresentation"]["topic"] = topic;
	}	
	if(!person->isSet(3)){
		newParticipant["ParticipatingIn"]["lastEvent"] = "culturalProgram";
	}


	person->printPerson();
	ofstream newfile("/home/jatin/oop/endSem/assign/participantfile.json");
	newfile << newParticipant;
	newfile.close();
}

int main(){
	cout<<"WELCOME TO IIIT SCRICITY \n\n\n";
	string name,gender,email,mobilenumber;
	vector<string> topics;
	string street;
	string houseNumber;
	string borough;
	string city;
	string district;
	string state;

	createAndInsertEvent();
			
	int choice=1,age,flag=0,cs,year,count=0;
	while(choice==1){
		flag=0;
		cout<<"enter the name of the participant \n";
		cin>>name;
		cout<<"enter gender of the participant \n";
		cin>>gender;
		cout<<"enter email of the participant \n";
		cin>>email;
		cout<<"enter the mobile number \n";
		cin>>mobilenumber;
		cout<<"enter the age of the participant \n";
		cin>>age;

		//
		if(age >= 18 ){
			cout<<"age is gteater than 18"<<endl;
			Person* prsn = new Person();
			prsn->setName(name);
			prsn->setGender(gender);
			prsn->setEmail(email);
			prsn->setMobile(mobilenumber);


			// get the addrss of the person
			cout<<"enter the house numvber "<<endl;
			cin>>houseNumber;
			cout<<"enter the street "<<endl;
			cin>>street;
			cout<<"enter the borough Name \n";
			cin>>borough;
			cout<<" enter the city "<<endl;
			cin>>city;
			cout<<"enter the district \n";
			cin>>district;
			cout<<"enter the state \n";
			cin>>state;
			// free(addr);
			Address* addr = new Address();
			addr->sethouseNumber(houseNumber);
			addr->setStreet(street);
			addr->setBorough(borough);
			addr->setCity(city);
			addr->setDistrict(district);
			addr->setState(state);
			cout<<"entering address"<<endl;
			prsn->setAddress(addr);
			cout<<"address added";

			cout<<"choose the event \n\t1.quiz \n\t2. paperPresentation \n\t3. posterPresentation\n\t4. culturalProgram" <<endl;
			cin>>cs;

			// 60 gens,
			switch(cs){
				case 1:
				prsn->setEvent(0);
				flag=1;
				break;
				case 2:
					cout<<"enter batch year : 1 | 2 | 3 | 4 \n";
					cin>>year;
					if(year == 4 && !prsn->isSet(2)){
						flag=1;
						prsn->setEvent(1);
					}else{
						cout<<"only 4th year can participate in it \n";
					}
					break;
				case 3:
					if(!prsn->isSet(1)){
						prsn->setEvent(2);
						flag=1;
					}
					break;
				case 4:
					prsn->setEvent(3);
					flag=1;
					break;
			}
			// string roll;
			// cout<<"enter the roll number "<<endl;
			// cin>>roll;
			if(flag==1){
				// cout<<"enter the unique id for student \n";
				// writeIntoJsonFile(prsn);
				writeIntojsonDictionary(prsn,count);
				count++;
				// if(flagnew == 0){
				sendFileToServer();
				// }

				// sendToServer();
			}
			free(prsn);
			free(addr);
		}

		cout<<"enter 1 to continue..."<<endl;
		cin>>choice;
	}

	return 0;

}















