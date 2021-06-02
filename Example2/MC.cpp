/*
Developed 2021, and copyrighted to the authors: Yuehan Li and Vojtech Vlcek,
at the University of California, Santa Barbara

If you use or modify any part of this routine, the header should be kept, unmodi
fied.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <math.h>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h> 
#include <string.h>
using namespace std;

//Find the Atomic Position in given filename (string ar)
//number = number of atoms
vector<string> coordinates(string ar, string number){
	string s;
	ifstream tempinput;
	int n=ar.length();
	char ca[n+1];
	strcpy(ca,ar.c_str());
	tempinput.open(ca);
	vector<string> temp;
			int i=0;
			while(getline(tempinput,s)&& i<stoi(number)){
				
				istringstream ss(s);
				do{
					string word;
					ss>> word;
					if(word!=""){temp.push_back(word);}
				}while(ss);
				i++;
			}
	tempinput.close();
	return temp;
}

int main(int argc, char*argv[]){
	//argv[1] == number of atoms
	//argv[2] == unperturbed supercell file name (usually 0.0)
	//argv[3] == all file names of perturbed cells (seperate by space, overall should be quoted in "")
	//argv[4] == Output file name

	//Start to read unperturbed supercell file, push atomic position into vector p0
	string s;
	ifstream input0;
	input0.open(argv[2]);
	vector<string> p0;
			int i=0;
			while(getline(input0,s)&& i<stoi(argv[1])){			
				istringstream ss(s);
				do{
					string word;
					ss>> word;
					if(word!=""){p0.push_back(word);}
				}while(ss);
				i++;
			}
	input0.close();
	cout<< p0.size();

	//Separate filenames for perturbed files, push all file name into vector v
	string str = string(argv[3]);
	stringstream strr(str);
	vector<string> v;
	string token;
	while(getline(strr, token, ' ')){
		v.push_back(token);
	}

	//we will make a vector vector, like map
	//outer vector is index that represent the file number
	//for each index from outer vector, it contains a inner vector that contain all the coordinates from that file
	vector<vector<string> > all;
	for(int i=0; i<v.size(); i++){
		vector<string> temp;
		temp = coordinates(v[i],argv[1]);
		all.push_back(temp);
	}

	//Change our inner vector from atomic position into atomic displacement from equilibrium position
	for(int i=0; i<all.size(); i++){
		for(int j=0; j<all[i].size()/4; j++){
			all[i][4*j+1]=to_string(stod(all[i][4*j+1])-stod(p0[4*j+1]));
			all[i][4*j+2]=to_string(stod(all[i][4*j+2])-stod(p0[4*j+2]));
			all[i][4*j+3]=to_string(stod(all[i][4*j+3])-stod(p0[4*j+3]));
		}
	}

	//Start to make linear combination of the displacement
	//firstly only push first file's displacement into the final vector licombdiff
	vector<string> licombdiff;
	for(int i=0; i<all[0].size(); i++){
		licombdiff.push_back(all[0][i]);
	}

	//Generate random sign based on generating random number, and determine whether it is odd or even
	//Displace the coordinates based on the sign times the displacement
	vector<int> signs;
	srand((unsigned) time(0));
	for(int i=1; i<all.size(); i++){
		
		int randomNumber = rand();
		signs.push_back(randomNumber % 2);
		cout<<signs[i-1];
		if(signs[i-1]==1){
			for(int j=0; j<all[i].size()/4; j++){
				licombdiff[4*j+1]=to_string(stod(licombdiff[4*j+1])+stod(all[i][4*j+1]));
				licombdiff[4*j+2]=to_string(stod(licombdiff[4*j+2])+stod(all[i][4*j+2]));
				licombdiff[4*j+3]=to_string(stod(licombdiff[4*j+3])+stod(all[i][4*j+3]));
			}
		}
		else if(signs[i-1]==0){
			for(int k=0; k<all[i].size()/4; k++){
				licombdiff[4*k+1]=to_string(stod(licombdiff[4*k+1])-stod(all[i][4*k+1]));
				licombdiff[4*k+2]=to_string(stod(licombdiff[4*k+2])-stod(all[i][4*k+2]));
				licombdiff[4*k+3]=to_string(stod(licombdiff[4*k+3])-stod(all[i][4*k+3]));
			}
		}else{cout<< "false";}

	}

	//Generate two files, both perturbe from the original supercell, but in opposite sign
	ofstream output1;
	ofstream output2;
	output1.open(string(argv[4]));
	output2.open(string(argv[4])+"oppo");
	for(int i=0; i<licombdiff.size()/4; i++){
		output1<<licombdiff[i*4]<<" "<<to_string(stod(licombdiff[4*i+1])/(pow(all.size(),0.5))+stod(p0[4*i+1]))<<" "<<to_string(stod(licombdiff[4*i+2])/(pow(all.size(),0.5))+stod(p0[4*i+2]))<<" "<<to_string(stod(licombdiff[4*i+3])/(pow(all.size(),0.5))+stod(p0[4*i+3]))<<endl;
		output2<<licombdiff[i*4]<<" "<<to_string(-stod(licombdiff[4*i+1])/(pow(all.size(),0.5))+stod(p0[4*i+1]))<<" "<<to_string(-stod(licombdiff[4*i+2])/(pow(all.size(),0.5))+stod(p0[4*i+2]))<<" "<<to_string(-stod(licombdiff[4*i+3])/(pow(all.size(),0.5))+stod(p0[4*i+3]))<<endl; 
	}
	output1.close();
	output2.close();

	return 0;

}
