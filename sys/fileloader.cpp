/*
 * ImageManager.cpp
 *
 *  Created on: 2012. 8. 4.
 *      Author: decoz
 */

#include "fileloader.h"

#include <stdio.h>
#include <string.h>
#include "io.h"

fileloader::fileloader() {
	// TODO Auto-generated constructor stub
	cur = 0;
}

fileloader::~fileloader() {
	// TODO Auto-generated destructor stub
}



void fileloader::load(char *fpath){

	 char *path = cutoffPath(fpath);

	 //printf("path:%s\n",path);

	_finddata_t fd;
	long handle = _findfirst(fpath,&fd);

	if(handle == -1) 	return ;

	if(fd.attrib != _A_SUBDIR){
		flist.push_back( pathName(path,fd.name));
		//printf("%s\n",fd.name);
	}

	while( _findnext(handle,&fd) != -1){
		if(fd.attrib != _A_SUBDIR)
			flist.push_back( pathName(path,fd.name));
	}
	_findclose(handle);

}



void fileloader::loadDir(char *fpath){

	 char *path = cutoffPath(fpath);

	 //printf("path:%s\n",path);

	_finddata_t fd;
	long handle = _findfirst(fpath,&fd);

	if(handle == -1) 	return ;

	if(fd.attrib == _A_SUBDIR){
		flist.push_back( pathName(path,fd.name));
	}

	while( _findnext(handle,&fd) != -1){
		if(fd.attrib == _A_SUBDIR)
			flist.push_back( pathName(path,fd.name));
	}
	_findclose(handle);

}





char *fileloader::pathName(char *path, char *name){

	//printf("[%s] + [%s]\n",path,name);

	int nl = strlen(name) + strlen(path);
	char *fname = new char[nl+2];

	strcpy(fname,path);
	strcat(fname,name);
	return fname;
}


char *fileloader::cutoffPath(char *str){



	char *path = new char[200];

	int p = strlen(str);
	while( --p > 0  ){
		if( str[p] == '\\' || str[p] == '/'  ) {
			p++;
			break;
		}
	}

	memcpy(path,str,p+1);
	path[p] = 0;

	//printf("cutpath:%s => %s\n",str,path);

	return path;
}



char * fileloader::next(){
	//printf("file list size : %d, cur : %d\n", flist.size(), cur);
	if( flist.size() > cur ) 	return  flist[cur++];
	else return NULL;

}

char *fileloader::getName(){

	if(!cur ) return NULL;

	char *path = cutoffPath(flist[cur-1]);
	int l = strlen(path);

	char *name = new char[1000];
	strcpy(name, flist[cur-1] + l) ;
	return name;

}


char *fileloader::getPath(){

	if(!cur ) return NULL;

	char *path =cutoffPath(flist[cur-1]);
	return path;

}

