/*
 * fileloader.h
 *
 *  Created on: 2012. 8. 4.
 *      Author: decoz
 *
 *
 *      ���Ϸδ��� ������ ���ϸ���Ʈ�� �о� �����ϴ� ��Ȱ�� �Ѵ�.
 *      db�� ���ڵ�ó�� next() ����� �����Ѵ�.
 *
 *		��:)
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

	void load(char *path); 		// �н�+  ���ϸ�(����) �� �Է�  ex) load("images/*.jpg " );
	void loadDir(char *path);	// �н� directory �� �д´�.

	char *next();			// ���� ������ ���ϳ����� �����ϰ� ���ΰ�� null �� ����
	char *getName(); 	// ���� ���õ� ������ ���ϸ��� ���� (���丮�� ��� ���丮��)
	char *getPath(); 	// ���� ���õ� ������ �н��� ����
	char *pathName(char *path, char *name); // ��ο�  ���ϸ��� ���ļ� ����

	fileloader();
	virtual ~fileloader();

private:
	int cur;

	char *cutoffPath(char *str); // ���ϸ��� ������ ��θ� �и�,  �������� / ÷��

};


#endif /* IMAGEMANAGER_H_ */
