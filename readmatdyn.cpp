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
#include <tuple>
using namespace std;

#define PI 3.14159265

//mean square displacement function
//( (1/2/w_ks) * [1 + 2n(w_ks, T)] )^(1/2)
double displacement(double f, double T){
	double h_bar = 1.054571*pow(10,-34.0);
	double w_ks = f;
	double kb = 1.3806*pow(10,-23.0);
	double m = 1.9944235*pow(10,-26.0);
	double dis = pow((1/(w_ks/6579.683920502))*(0.5+1/(exp(1*(w_ks/6579.683920502)/(3.17*(pow(10,-6)))/T)-1))/(12.0107*1836.152),(0.5));
	return dis;

}

//Find every phonon mode in a given(input) matdyn file
//num = phonon branch number
//line = atoms number in the phonon
vector<string> readmat(string input, string num, int line){
	string s;
	ifstream infile;
	infile.open(input);
	vector<string> temp;
	std::vector<string> v;
	size_t found;
	double frequency;
	while(getline(infile,s)){
		if(stoi(num)<10){found=s.find(std::string("     freq (    "+num));}
		if(stoi(num)>9 && stoi(num)<100){ found=s.find(std::string("     freq (   "+num));}
		if(stoi(num)>99 && stoi(num)<1000){ found=s.find(std::string("     freq (  "+num));}

		if(found!=string::npos){
			istringstream ss(s);
			
			int i=0;
			
			while(getline(infile,s)&&i<line){
				istringstream ss(s);
				do{
					string word;
					ss>> word;

					if((word!="=")&&(word!="(")&&(word!=")")){
						temp.push_back(word);
					}
				}while(ss);
				i++;
			}	
		}
	}
	infile.close();
	return temp;
}

//Find the k point value of this given matdyn file. 
//Will be used in calculating bloch wave
vector<string> KPoint(string input){
	string s;
	ifstream infile;
	infile.open(input);
	vector<string> temp;
	std::vector<string> v;
	size_t found;
	while(getline(infile,s)){
		found = s.find("Diagonalizing the dynamical matrix");
		if (found != string::npos){
			istringstream ss(s);
			int i =0;
			while(getline(infile,s) && i< 2){
				istringstream ss(s);
				do{
					string word;
					ss>> word;

					if((word!="=")&&(word!="(")&&(word!=")")){
						temp.push_back(word);
					}
				}while(ss);
				i++;
			}
		}
	}
	infile.close();
	return temp;
}

//Find the frequnecy of a given phonon mode
double findf(string input, string num){
	string s;
	ifstream infile;
	infile.open(input);
	std::vector<string> v;
	size_t found;
	double frequency;
	while(getline(infile,s)){
		if(stoi(num)<10){found=s.find(std::string("     freq (    "+num));}
		if(stoi(num)>9 && stoi(num)<100){ found=s.find(std::string("     freq (   "+num));}
		if(stoi(num)>99 && stoi(num)<1000){ found=s.find(std::string("     freq (  "+num));}

		if(found!=string::npos){
			istringstream ss(s);
			do{
				string word;
				ss>> word;

				if((word!="=")&&(word!="(")&&(word!=")")){
					v.push_back(word);
				}

			}while(ss);
			
		}
	}
	frequency=stod(v[2]);
	infile.close();
	return frequency;
}

int main(int argc, char*argv[]){
	//argv[1] == atomic position file name
	//argv[2] == matdyn file name
	//argv[3] == cell parameter file
	//argv[4] == frequency number
	//argv[5][6][7] == Supercell size
	//argv[8] == temperature;
	//argv[9] == output file name
	//argv[10] == angstrom || bohr unit
	
	string s;

	//Start to read in cell parameter file, push it to vector v
	//////////////CELL PARAMETER MUST IN CARTESION COORDINATES//////////////
	//////////////KEEP SAME UNIT AS ATOMIC POSITION//////////////
	vector<string> v;
	ifstream input0;
	input0.open(argv[3]);
	while(getline(input0,s)){
		int i=0;
			istringstream ss(s);
			do{
				string word;
				ss>> word;
				if(word!=""){v.push_back(word);}
			}while(ss);
			i++;
	}
	input0.close();

	//Start to read in the atomic position within the unit cell, push it to vecto pp
	//number of atoms = line
	//////////////ACTOMIC POSITION MUST IN CARTESION COORDINATES///////////////
	//////////////KEEP SAME UNIT AS CELL PARAMETER//////////////
	ifstream input1;
	input1.open(argv[1]);	
	vector<string> pp;
	while(getline(input1,s)){
		int i=0;
			istringstream ss(s);
			do{
				string word;
				ss>> word;
				if(word!=""){pp.push_back(word);}
			}while(ss);
			i++;
	}
	input1.close();
	int line = pp.size()/4;

	//x, y, z will be supercell size in three dimension
	int x, y, z;
	x = stoi(string(argv[5]));
	y = stoi(string(argv[6]));
	z = stoi(string(argv[7]));

	//Constructing the initial/unperturbed supercell
	if(string(argv[2]) == "0"){
		vector<string> fin;
	for(int xx=0;xx<abs(x);xx++){
		for(int yy=0;yy<abs(y);yy++){
			for(int zz=0;zz<abs(z);zz++){
				for(int i=0; i<pp.size()/4; i++){
					fin.push_back(pp[i*4]);
					fin.push_back(to_string(stod(pp[i*4+1])+stod(v[0])*double(xx)+stod(v[3])*double(yy)+stod(v[6])*double(zz)));
					fin.push_back(to_string(stod(pp[i*4+2])+stod(v[1])*double(xx)+stod(v[4])*double(yy)+stod(v[7])*double(zz)));
					fin.push_back(to_string(stod(pp[i*4+3])+stod(v[2])*double(xx)+stod(v[5])*double(yy)+stod(v[8])*double(zz)));
				}
			}
		}
	}

	//OUTPUT the unperturbed supercell into the file call 0.0 (default) 
	ofstream output0;
	output0.open("0.0");
	for (int i=0;i<fin.size()/4;i++){
		output0<<fin[i*4]<<" "<<fin[i*4+1]<<" "<<fin[i*4+2]<<" "<<fin[i*4+3]<<endl;
	}
	output0.close();
	return 0;
	}

	//Find the phonon mode of chosen number, push into vector mat
	//Find the k point of this phonon mode, push into kpoint
	vector<string> mat;
	vector<string> kpoint;
	mat = readmat(string(argv[2]),string(argv[4]),line);
	kpoint = KPoint(string(argv[2]));

	//Normalization process, make sure the mean square dispalcement from the phonon is the same as calculated
	//Calculate the final amplitude (unit will transform into bohr)
	double squ_sum = 0.0;
	for(int i=0; i<mat.size()/7;i++){
		squ_sum +=pow(stod(mat[7*i]),2);
		squ_sum +=pow(stod(mat[7*i+2]),2);
		squ_sum +=pow(stod(mat[7*i+4]),2);
	}

	double frequency = findf(string(argv[2]),string(argv[4]));
	double amplitude;
	if(string(argv[10])=="b"){
		amplitude = displacement(frequency, stod(argv[8]))/(pow(squ_sum/line,0.5));
	}
	else if(string(argv[10])=="a"){
		amplitude = displacement(frequency, stod(argv[8]))/(pow(squ_sum/line,0.5))*0.529177;
	}

	//Constructing the initial/unperturbed supercell
	vector<string> fin;
	for(int xx=0;xx<abs(x);xx++){
		for(int yy=0;yy<abs(y);yy++){
			for(int zz=0;zz<abs(z);zz++){
				for(int i=0; i<pp.size()/4; i++){
					fin.push_back(pp[i*4]);
					fin.push_back(to_string(stod(pp[i*4+1])+stod(v[0])*double(xx)+stod(v[3])*double(yy)+stod(v[6])*double(zz)));
					fin.push_back(to_string(stod(pp[i*4+2])+stod(v[1])*double(xx)+stod(v[4])*double(yy)+stod(v[7])*double(zz)));
					fin.push_back(to_string(stod(pp[i*4+3])+stod(v[2])*double(xx)+stod(v[5])*double(yy)+stod(v[8])*double(zz)));
				}
			}
		}
	}

	//Displace the supercell based on the phonon mode, push into vector fin
	for(int xx=0;xx<abs(x);xx++){
		for(int yy=0;yy<abs(y);yy++){
			for(int zz=0;zz<abs(z);zz++){
				for(int j=0;j<line;j++){
					fin[(abs(z)*abs(y)*xx+abs(z)*yy+zz)*4*line+j*4+1]=to_string(stod(fin[(abs(z)*abs(y)*xx+abs(z)*yy+zz)*4*line+j*4+1])+stod(mat[j*7])*amplitude*cos((xx*stod(kpoint[2])+yy*stod(kpoint[3])+zz*stod(kpoint[4])) * 2 * PI)); 
					fin[(abs(z)*abs(y)*xx+abs(z)*yy+zz)*4*line+j*4+2]=to_string(stod(fin[(abs(z)*abs(y)*xx+abs(z)*yy+zz)*4*line+j*4+2])+stod(mat[j*7+2])*amplitude*cos((xx*stod(kpoint[2])+yy*stod(kpoint[3])+zz*stod(kpoint[4])) * 2 * PI));
					fin[(abs(z)*abs(y)*xx+abs(z)*yy+zz)*4*line+j*4+3]=to_string(stod(fin[(abs(z)*abs(y)*xx+abs(z)*yy+zz)*4*line+j*4+3])+stod(mat[j*7+4])*amplitude*cos((xx*stod(kpoint[2])+yy*stod(kpoint[3])+zz*stod(kpoint[4])) * 2 * PI));
				}
			}
		}
	}

	//Output the final displaced supercell into filename argv[9]
	ofstream output;
	output.open(string(argv[9]));
	for (int i=0;i<fin.size()/4;i++){
		output<<fin[i*4]<<" "<<fin[i*4+1]<<" "<<fin[i*4+2]<<" "<<fin[i*4+3]<<endl;
	}

	output.close();
	return 0;

}

