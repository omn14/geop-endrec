#include <stdio.h>
#include <wiringSerial.h>
#include <iostream>
#include <limits>
#include <unistd.h>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iterator>

union mytype {
	int i;
	char c[0];
	float f;
	int16_t i16[0];
};

main ()
{
	mytype t;
	t.c[0]=4;
	t.c[1]=0;
	t.c[2]=0;
	t.c[3]=0;
	std::cerr << "f,i\n"<< sizeof(float)<<","<<sizeof(int)<<
			"\nt.n 192 7 0 0 : " << t.i << std::endl;
	t.i16[0] = -480;
	std::cerr << "int16 -480 " << (int)t.c[0] << " " << (int)t.c[1] << std::endl;
	
	//file output
	std::ofstream myfile("data.bin", std::ios::out);

	int fd = serialOpen("/dev/ttyACM0",115200);
	std::cout << "fd: (<0 = error) " << fd << std::endl;
	int d = 0;
	char buffer[256];
	std::vector<int16_t> dat;
	int it=0;
	while(1){
	int afr = serialDataAvail(fd);
	
	//std::cerr << afr << std::endl;
		if(afr>=2){
			read(fd,&buffer[0],2);
			mytype tall;
			tall.c[0]=buffer[0];
			tall.c[1]=buffer[1];
			//tall.c[2]=buffer[2];
			//tall.c[3]=buffer[3];
			dat.push_back(tall.i16[0]);
			std::cerr << "sampl#: "<< dat.size() << " vector = "<< dat[dat.size()-1] << " " << (int)tall.c[0] << " " << (int)tall.c[1] << std::endl;
			//myfile.write(reinterpret_cast<const char*>(&dat.size()),2);
			it=it+1;
			if(dat[dat.size()-1] == 4) myfile << it << std::endl;
			if(dat[dat.size()-1] != 4){
			myfile << dat[dat.size()-1] << std::endl;
			}
			
		}


		/*
		if(afr>0){
		std::cout << "press return key, to see whats available" << std::endl;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		afr = serialDataAvail(fd);
		std::cout << "char available for reading: " << afr << std::endl;
		//d = serialGetchar(fd);
		read(fd,&buffer[0],afr);
		//buffer[afr]=0;
		for(int i=0;i<afr;i++){
			std::cout << "attempted read, result: " <<(int)buffer[i] << std::endl;
		}
		//afr = serialDataAvail(fd);				
		std::cout << "chars now available for reading: " << serialDataAvail(fd) << std::endl << std::endl;
//		std::cout << "press return key, to proceed" << std::endl;
//		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
//		std::cin >> afr;
		mytype tall;
		std::vector<int> vector;
		for(int i = 0;i<afr;i=i+4){
			tall.c[0]=buffer[i];
			tall.c[1]=buffer[i+1];
			tall.c[2]=buffer[i+2];
			tall.c[3]=buffer[i+3];
			vector.push_back(tall.i);
			std::cerr << " i equals: "<< i << " vector = "<< vector[vector.size()-1] << std::endl;

		}
		
		}
		*/
	}
myfile.close();
return 0;
}
