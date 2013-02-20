#include <iostream>
#include <string.h>
#include <fstream>
#include <cstring>

using namespace std;


int main()
{
	int i=10;
	
	ifstream mystream("hello.txt", ios::in | ios::binary);
	if(!mystream) {
		cout << "Cannot open file.\n";
	}
	ofstream out("everything.txt", ios::out | ios::binary);
	if(!out) {
		cout << "Cannot open file.\n";
	}

	int packet_size=1500;
	int packet_number=1;
	char p[1500],p1[1500],ch;
	char *pp;
	int qq;
	
		cout<<"\nEnter the packet number:";
		cin>>packet_number;
		mystream.seekg (packet_number*packet_size,ios_base::beg);
		mystream.read(p, packet_size);
		
		if(!strlen(p))
		{

			cout<<"\nNOTHING IS THERE";
		}	
		else
		{
			
			p[strlen(p)-2]='\0';
			cout<<"\nthe sie is "<<strlen(p)<<endl;
			pp=strdup(p);
			//strcpy(p1,p);
			cout<<p;
		}

	cout<<endl;
	out.close();
	mystream.close();
	return 0;
}