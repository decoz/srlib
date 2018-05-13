/*
 * fileloader.h
 *
 *  Created on: 2012. 8. 4.
 *      Author: decoz
 *
 *
 *      파일로더는 폴더의 파일리스트를 읽어 관리하는 역활을 한다.
 *      db의 레코드처럼 next() 펑션을 지원한다.
 *
 *		예:)
 *
 *		fileloader fl
 *		fl.load("images", "*.jpg");
 *		char *fname;
 *		while( ( fname = fl.next( ) != null ){
 *
 *
 *		}
 *
 */

#ifndef IMAGEMANAGER_H_
#define IMAGEMANAGER_H_

#include <vector>
using namespace std;

class fileloader {

public:

	vector <char *> flist;

	void load(char *path); 		// 패스+  파일명(필터) 를 입력  ex) load("images/*.jpg " );
	void loadDir(char *path);	// 패스 directory 를 읽는다.

	char *next();			// 다음 파일의 파일네임을 리턴하고 끝인경우 null 을 리턴
	char *getName(); 	// 현재 선택된 파일의 파일명을 리턴 (디렉토리의 경우 디렉토리명)
	char *getPath(); 	// 현재 선택된 파일의 패스를 리턴
	char *pathName(char *path, char *name); // 경로와  파일명을 합쳐서 리턴

	fileloader();
	virtual ~fileloader();

private:
	int cur;

	char *cutoffPath(char *str); // 파일명을 제외한 경로만 분리,  마지막에 / 첨가

};


#endif /* IMAGEMANAGER_H_ */
