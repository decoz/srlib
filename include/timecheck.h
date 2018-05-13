/*
 * timecheck.h
 *
 *  Created on: 2018. 1. 25.
 *      Author: decoz
 *
 * checking elapsed time report
 *
 */

#ifndef TIMECHECK_H_
#define TIMECHECK_H_

#include <stdio.h>
#include <map>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;
typedef time_point<system_clock>  tpoint;
typedef duration<milliseconds> tduration;;

typedef struct {
	vector<float> list;
	tpoint last;
	int cnt;
	double time_sum;
} tinfo;

class timecheck {
private:
	timecheck();

public:
	static timecheck* it;
	map<string, tinfo*> timeinfo;

	static timecheck* getIt() { if( it == NULL)  it = new timecheck(); return it;; };
	virtual ~timecheck();

	static tinfo* 		getInfo(string key){ return getIt()->_getInfo(key); }
	static void 		start(string key)	{ getIt()->_start(key); } ;
	static double 	end(string key)	{ return  getIt()->_end(key); } ;
	static double 	everage(string key) { return getIt()->_everage(key); };
	static double 	getMax(string key) { return getIt()->_getMinMax(key, true); };
	static double 	getMin(string key) { return getIt()->_getMinMax(key, false); };

	tinfo* 	_getInfo(string key);
	void 	_start(string key); /* start time measurement */
	double	_end(string key); /*  end time measurement */
	double	_everage(string key);
	double 	_getMinMax(string key, bool getmax);

};

#endif /* TIMECHECK_H_ */
