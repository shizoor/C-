#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <sstream>

using namespace std;

//A simple Reverse Polish Notation calculator

int main(){

string inputstring, numstring, operatorstring, allowedoperators="+-/*^", allowednumerals="0123456789.";
int i=0, j=0, inputstringlen, inputchar, starti, lenallowedoperators, lenallowednumerals, flag, firstnumstringlength, secondnumstringlength, tokenflag, operatorflag=0, numstringlen=0, counter;
double firstnum=0.0, secondnum=0.0, total=0.0, number;
char operatorchar;
vector<double> thestack;
vector<string> tokenstream;
string nothing;  //For debugging

lenallowedoperators = allowedoperators.length();
lenallowednumerals = allowednumerals.length();

cout << ">>";
getline(cin, inputstring);
cout << inputstring << endl;

inputstringlen = inputstring.length();
starti = 0;

//Split the input into tokens

cout << "Splitting this into tokens : " << inputstring;

istringstream iss(inputstring);
do{
	string token;
	iss >> token;
	cout << "Substring : " << token << endl;
	tokenstream.insert(tokenstream.end(), token);
}while (iss);

tokenstream.pop_back();

cout << "Dumping tokenstream : " << endl;

for(i=0; i<tokenstream.size(); i++){
	cout << endl << tokenstream.at(i);
}


//Get token and check if a number or operator

for(i=0; i<tokenstream.size(); i++){
if (allowednumerals.find(tokenstream.at(i)[0]) !=  std::string::npos ){
cout << "numeral" << endl;
thestack.push_back(atof(tokenstream.at(i).c_str()));
}
else {cout << "operator" << endl;
	if (thestack.size()<2){cout<<"not enough numerals"<<endl;}
	else{
	secondnum = thestack.back();
	thestack.pop_back();
	firstnum = thestack.back();
	thestack.pop_back();
	operatorchar = tokenstream.at(i)[0];
	
	switch (operatorchar){
        case '+':
        total = firstnum+secondnum;
	cout << firstnum << "+" << secondnum << "=" << total << endl; 
        break;
        case '*':
        total = firstnum*secondnum;
	cout << firstnum << "*" << secondnum << "=" << total << endl;
        break;
        case '/':
        total = firstnum/secondnum;
	cout << firstnum << "/" << secondnum << "=" << total << endl;
        break;
        case '-':
        total = firstnum-secondnum;
	cout << firstnum << "-" << secondnum << "=" << total << endl;
        break;
	case '^':
	total = pow(firstnum, secondnum);
	cout << firstnum << "^" << secondnum << "=" << total << endl;
	break;
        default:
        cout << "invalid operator";
        break;
        }
        thestack.push_back(total);
	
	}
}

}



cout << "vector (thestack) contents : " << endl;

for(i=0; i<thestack.size(); i++){
 cout << thestack.at(i) << endl;
}


}


