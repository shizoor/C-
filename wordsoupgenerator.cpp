//command iteration depth offset 'string'

using namespace std;
#include <iostream>
#include <string>

u_int offset = 0;
std::string message;
u_int meslen = 0;

int rfunc(int c){
	u_int i;
	u_int pointer = u_int(c);

	u_int mespoint = (pointer - 1 + offset)%meslen;
	offset--;

	
	for (i=0; i<c; i++){
		mespoint++;	
		cout << message[mespoint-1];
	}	
	//cout << c << endl;
	c--;
	if (c>0){
		rfunc(c);
		rfunc(c);
		}
	
	return c;	

}


int main(int argc, char *argv[]){
	u_int offset = u_int(stoi(argv[2]));
	u_int i = u_int(stoi(argv[1]));
	message = argv[3];
	meslen = message.length();

	cout << rfunc(i) << endl;
	


}
