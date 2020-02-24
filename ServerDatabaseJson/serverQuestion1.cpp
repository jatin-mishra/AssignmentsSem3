#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <fstream>
#include <netdb.h>
#include "nlohmann/json.hpp"
#include "mysql.h"

using json = nlohmann::json;
using namespace std;

#define USER "jatin"
#define SERVER "localhost"
#define DATABASE "eventRules"
#define PASSWORD "Ja@tin123"

void error(const char* mssg){
	perror(mssg);
	exit(1);
}

void receiveJSONandSave(int portNumber){
	int filedescriptor,newFileDes,number;
 char buffer[250];

 struct sockaddr_in serverAddress,clientAddress;
 socklen_t clientLength;

 filedescriptor = socket(AF_INET,SOCK_STREAM,0);

 if(filedescriptor < 0){
 	error("cannot create file descriptor");
 }

 bzero((char*)&serverAddress,sizeof(serverAddress));


	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);

	if(bind(filedescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress))){
		error("binding error");
	}

	listen(filedescriptor,5);

	clientLength = sizeof(clientAddress);

	newFileDes = accept(filedescriptor,(struct sockaddr*)&clientAddress,&clientLength);
	if(newFileDes < 0)
		error("cannot accept the connections");
	bzero(buffer,250);
	// write into the json file;
	// read from the json file
	// insert into the database

	FILE* fp;
	int words,n;
	fp = fopen("serverParticipant.json","w");

	read(newFileDes,&words,sizeof(int));

	int count=0;
	while(count != words){
		bzero(buffer,250);
		n = read(newFileDes,buffer,250);
		if(n<0)
			error("cannot read file");
		fprintf(fp, "%s ",buffer);
		count++;
	}

	cout<<"file has been recieved successfully : name is :- serverParticipant.json" <<endl;

	close(filedescriptor);
	close(newFileDes);
}


void saveToTheDatabase(){
	ifstream file("/home/jatin/oop/endSem/assign/participantfile.json");
	json newParticipant = json::parse(file);

	string name = newParticipant["name"];
	string gender = newParticipant["gender"];
	string email = newParticipant["Email"];
	string mobilenumber = newParticipant["MobileNumber"];
	string street = newParticipant["Address"]["street"];
	string houseNumber = newParticipant["Address"]["houseNumber"];
	string Borough = newParticipant["Address"]["Borough"];
	string city = newParticipant["Address"]["City"];
	cout<<"city is " << city<<endl;
	string district = newParticipant["Address"]["District"];
	string state = newParticipant["Address"]["State"];

	string quiz = newParticipant["ParticipatingIn"]["eventOne"];
	string paperPresentation = newParticipant["ParticipatingIn"]["eventTwo"]["paperPresentation"]["topics"];
	string posterPresentation = newParticipant["ParticipatingIn"]["eventTwo"]["posterPresentation"]["topic"];
	string cultural = newParticipant["ParticipatingIn"]["lastEvent"];

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
		string query;
		int number;
		// for(string rule : rules){
			// query = "insert into " + TABLE + " values ('" + rule + "');";
			query = "insert into participants values ('" + name + "','" + gender  + "','" +  email   + "','" +  mobilenumber  + "','" + street  + "','" +  houseNumber  + "','" +   Borough  + "','" +  city  + "','" +  district  + "','" +  state   + "','" +  quiz  + "','" +  paperPresentation  + "','" +  posterPresentation  + "','" + cultural  + "');";
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















int main(int argc,char* argv[]){
 if(argc<2){
 	fprintf(stderr,"port number not specifed plz specify as command line argument");
 	exit(1);
 }

	 receiveJSONandSave(atoi(argv[1]));
 	int cs =1;
 	while(cs){
 		
 	 saveToTheDatabase();
 	 cout<<"do you want to read and insert press 1 for yes "<<endl;
 		cin>>cs;
 	}


	return 0;

}