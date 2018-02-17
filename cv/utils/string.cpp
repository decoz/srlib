#include "util.h"

int wcharcnt(String s){

	int  sz = s.size();
	const char *buff = s.c_str();

	int cnt = 0;
	for(int i=0;i<sz; cnt++) {
		if( ( buff[i] & 0x80 ) == 0x80 ) i += 2;
		else i++;
	}

	return cnt;
}


String wcharAt(String s, int idx){

	int p = 0;
	const char *buff = s.c_str();

	for(int i=0; i<idx && p < (int) s.size() ; i++){

		if( ( buff[p] & 0x80 ) == 0x80 ) p += 2;
		else p++;
	}

	if( ( buff[p] & 0x80 ) == 0x80 ) return s.substr(p,2);
	else return s.substr(p,1);



}
