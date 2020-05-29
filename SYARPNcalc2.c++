#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <sstream>

using namespace std;

string getassociativity(string inputoperator) {
	string associativity = "left";
	if (inputoperator == "^") { associativity = "right"; }
	return(associativity);
}


int getprecedence(char inputoperator)
{
	int precedence = 0;
	switch (inputoperator){
	case '^':precedence = 4;
		break;
	case '*' : precedence = 3;
		break;
	case '/' : precedence = 3;
		break;
	case '+': precedence = 2;
		break;

	}
	return (precedence);
}

bool isanumeral(char inputchar) {
	bool isanumeral = false;
	if ((inputchar > ('0'-1) && inputchar < ('9'+1)) || inputchar == '.')isanumeral = true;   //Yes, a full stop is a number, shut up.
	return isanumeral;
}


bool isanoperator(char inputchar) {
	bool isanoperator = false;
	if ((inputchar == '+') || (inputchar == '-') || (inputchar == '/') || (inputchar == '*') || (inputchar == '^')  || (inputchar == '(') || (inputchar == ')'))isanoperator = true;
	return isanoperator;
}

bool isaleftparenthesis(char inputchar) {
	bool isaleftparenthesis = false;
	if (inputchar == '(')isaleftparenthesis = true;
	return isaleftparenthesis;
}

bool isarightparenthesis(char inputchar) {
	bool isarightparenthesis = false;
	if (inputchar == ')')isarightparenthesis = true;
	return isarightparenthesis;
}

bool isaspace(char inputchar) {
	bool isaspace = false;
	if (inputchar == ' ') { isaspace = true; }
	return(isaspace);
}

string syaprocess(string inputstring) {
	//inputstring.push_back('0');
	int i=0;
	int j=0;
	int strpoint;
	vector<string> tokenstream;
	vector<string> operatorstack;
	vector<string> outputqueue;
	string token;
	string outputstring;

	//cout << "split input string into tokens" << endl;
	for (i = 0; i < inputstring.size(); i++) {
		if (isanumeral(inputstring[i])) {
			tokenstream.resize(tokenstream.size()+1);
			strpoint = 0;
			while (isanumeral(inputstring[i])) {
				
				tokenstream.at(j).resize(strpoint+1);
				tokenstream.at(j)[strpoint] = inputstring[i];
				//cout << "appending : " << inputstring[i] <<  "i = " << i << " j = "  << j << endl;
				strpoint++;
				i++;
			}
			j++;
		}
		
		if (isanoperator(inputstring[i]) && !(isaleftparenthesis(inputstring[i]) || isarightparenthesis(inputstring[i]))) {
			tokenstream.resize(tokenstream.size()+1);
			tokenstream.at(j).push_back(inputstring[i]);
			//cout << "operator\n";
			j++;
		}

		if (isaleftparenthesis(inputstring[i])) {
			tokenstream.resize(tokenstream.size()+1);
			tokenstream.at(j).push_back(inputstring[i]);
			//cout << "left bracket\n";
			j++;
		}

		if (isarightparenthesis(inputstring[i])) {
			tokenstream.resize(tokenstream.size()+1);
			tokenstream.at(j).push_back(inputstring[i]);
			//cout << "Right bracket\n";
			j++;
		}


		if (!isaleftparenthesis(inputstring[i]) && !isarightparenthesis(inputstring[i]) && !isanumeral(inputstring[i]) && !isanoperator(inputstring[i]) && !isaspace(inputstring[i])) { 
			//cout << "That's not anything, breaking\n";
			break; }
	}	

	//cout << "tokenstream : ";
	//for (i = 0; i < tokenstream.size(); i++) { cout << tokenstream.at(i) << endl; }
	//cout << "end of tokenstream\n";// debug
		

	//token splitting working, now do shunting yard.    

	for (i = 0; i < tokenstream.size(); i++) {
		if (isanumeral(tokenstream.at(i)[0])) { 
			outputqueue.resize(outputqueue.size() + 1);
			outputqueue.push_back(tokenstream.at(i));   // if the token is a number, push it to the output queue
			//cout << "numeral, pushing to output queue: " << tokenstream.at(i) << endl;
		}
		
		if (isanoperator(tokenstream.at(i)[0]) && !isaleftparenthesis(tokenstream.at(i)[0]) && !isarightparenthesis(tokenstream.at(i)[0])) {
			//cout << "token " << tokenstream.at(i) <<  "is an operator.  Processing operator stack : " << endl;
			while (((!operatorstack.empty() && isanoperator(operatorstack.at(0)[0])) && ((getprecedence(operatorstack.at(0)[0]) > getprecedence(tokenstream.at(i)[0]) || (getprecedence(operatorstack.at(0)[0]) == getprecedence(tokenstream.at(i)[0]) && getassociativity(tokenstream.at(i)) == "left") ))) && !isaleftparenthesis(operatorstack.back()[0])) {
				
				outputqueue.push_back(operatorstack.back());
				//pushed to the output queue
				operatorstack.pop_back();

			}
			//pushed to the operator stack
			operatorstack.push_back(tokenstream.at(i));
		}
		
		if (isaleftparenthesis(tokenstream.at(i)[0])) { 
			//pushed to the operator stack
			operatorstack.push_back(tokenstream.at(i)); }

		if (isarightparenthesis(tokenstream.at(i)[0]) && !operatorstack.empty()) {
			//cout << "Right parenthesis .. emptying operator stack onto the outputqueue \n";
			while (!operatorstack.empty() && !isaleftparenthesis(operatorstack.back()[0])){   //implement check for mismatched parenthesis
				//cout << "pushing " << operatorstack.back() << "onto output queue\n";
				outputqueue.push_back(operatorstack.back());
				operatorstack.pop_back();

			}
			if (!operatorstack.empty()) {
				//cout << "found a left parenthesis, discarding\n";
				if (isaleftparenthesis(operatorstack.back()[0])) { operatorstack.pop_back(); }
			}
		}

	}
	
	while (!operatorstack.empty()) { outputqueue.push_back(operatorstack.back()); operatorstack.pop_back(); }
	
	for (i = 0; i <outputqueue.size(); i++)
	{
		//cout << outputqueue.at(i) << endl;
		outputstring.append(outputqueue.at(i));
		outputstring.append(" ");
	}

	//cout << "outputstring : " << outputstring << endl;
	return(outputstring);
}

//A simple Reverse Polish Notation calculator as a module



double rpnprocess(string inputstring) {

	string numstring, operatorstring;
	int i = 0, j = 0, inputstringlen, inputchar, starti, operatorflag = 0, numstringlen = 0, counter;
	long double firstnum = 0.0, secondnum = 0.0, total = 0.0, number;
	char operatorchar;
	vector<long double> thestack;
	vector<string> tokenstream;
	string nothing;  //For debugging
	char* numtest;

	inputstringlen = inputstring.length();
	starti = 0;

	//Split the input into tokens

	//cout << "Splitting this into tokens : " << inputstring;

	istringstream iss(inputstring);
	do {
		string token;
		iss >> token;
		//cout << "Substring : " << token << endl;
		tokenstream.insert(tokenstream.end(), token);
	} while (iss);

	tokenstream.pop_back();

	/*
	cout << "Dumping tokenstream : " << endl;
	for(i=0; i<tokenstream.size(); i++){
		cout << endl << tokenstream.at(i);
	}
	*/

	//Get token and check if a number or operator

	for (i = 0; i < int(tokenstream.size()); i++) {

		number = strtod(tokenstream.at(i).c_str(), &numtest);

		if (!(*numtest)) {
			//cout << "numeral" << endl;
			thestack.push_back(number);
		}
		else {	//cout << "operator" << endl;
			if (thestack.size() < 2) { cout << "not enough numerals" << endl; }
			else {
				secondnum = thestack.back();
				thestack.pop_back();
				firstnum = thestack.back();
				thestack.pop_back();
				operatorchar = tokenstream.at(i)[0];

				switch (operatorchar) {
				case '+':
					total = firstnum + secondnum;
					//cout << firstnum << "+" << secondnum << "=" << total << endl; 
					break;
				case '*':
					total = firstnum * secondnum;
					//cout << firstnum << "*" << secondnum << "=" << total << endl;
					break;
				case '/':
					total = firstnum / secondnum;
					//cout << firstnum << "/" << secondnum << "=" << total << endl;
					break;
				case '-':
					total = firstnum - secondnum;
					//cout << firstnum << "-" << secondnum << "=" << total << endl;
					break;
				case '^':
					total = pow(firstnum, secondnum);
					//cout << firstnum << "^" << secondnum << "=" << total << endl;
					break;
				default:
					cout << "invalid operator";
					break;
				}
				thestack.push_back(total);

			}
		}

	}

	if (thestack.size() > 1) {

		cout << "The stack has more than one value, contents : " << endl;

		for (i = 0; i < int(thestack.size()); i++) {
			cout.precision(17);
			cout << thestack.at(i) << endl;
		}

	}
	if (thestack.size() > 0) {
		return thestack.at(0);
	}
}


int main() {
	string inputstring;
	cout.precision(16);
	while (!(inputstring=="quit")) {
		cout << ">>";
		getline(cin, inputstring);
		if ((int(inputstring.length()) > 0) && inputstring !="quit") { cout << rpnprocess(syaprocess(inputstring)) << endl; }

	}

}

