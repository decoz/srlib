/*
 * timecheck.cpp
 *
 *  Created on: 2018. 1. 25.
 *      Author: decoz
 */

#include "timecheck.h"


timecheck *timecheck::it = NULL;

timecheck::timecheck() {
	// TODO Auto-generated constructor stub

}

timecheck::~timecheck() {
	// TODO Auto-generated destructor stub
}


void timecheck::_start(string key){
	_getInfo(key)->last = system_clock::now();

}

double timecheck::_end(string key){
	tinfo *ti = _getInfo(key);
	auto td =  system_clock::now() - ti->last;

	double interval = (double) td.count() / 1000000000;

	ti->list.push_back( interval );
	ti->time_sum += interval;
	ti->cnt++;
	return  interval;
}

tinfo* timecheck::_getInfo(string key){

	if( timeinfo.find(key) ==  timeinfo.end()){

		tinfo *ti = new tinfo;
		//ti->time_sum = chrono::duration<double>::zero();
		ti->time_sum = 0;
		ti->cnt = 0;
		timeinfo[key] = ti;
	}

	return timeinfo[key];
}

double timecheck::_everage(string key){
	tinfo *ti = _getInfo(key);
	return ti->time_sum / ti->cnt;

}

double timecheck::_getMinMax(string key, bool getmax){


	tinfo *ti = _getInfo(key);

	double 	min =  *ti->list.begin(),
				max =  *ti->list.begin();

	for(vector<float>::iterator i= ti->list.begin() + 1 ;
		 i != ti->list.end(); i++){
		if(max < *i) max = *i;
		if(min > *i) min = *i;
		//printf("%.3f \n", *i);
	}

	if(getmax) return max;
	else return min;;
}
