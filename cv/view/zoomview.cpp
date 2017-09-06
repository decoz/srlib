/*
 * zoomview.cpp
 *
 *  Created on: 2017. 2. 17.
 *      Author: decoz
 */

#include "zoomview.h"

namespace sr_lib {

zoomview *zoomview::it = NULL;
int zoomview::sx = 0, zoomview::sy = 0;
Scalar zoomview::textcolor = Scalar(255,255,255);

void zoomview::_release(char *name){

	if( zinfo[name]  != NULL  ) {
		zinfo[name]->src.release();
		zinfo[name]->view.release();
		delete zinfo[name];
		zinfo[name] = NULL;
	}

}

void 	zoomview::_show(char *name, Mat img, Size sz){

	float zoomrate = 1;
	Size wsize;

	float  vr  =  (float)sz.height / img.rows, hr = (float) sz.width / img.cols;

	if(  sz.width != 0 &&  sz.height  != 0  )
		zoomrate = vr < hr ?  vr : hr;

	if( sz.width == 0 &&  sz.height  == 0 ) {
		zoomrate = 1;
	} else 	if( sz.width == 0 ){
		zoomrate = hr;
	} else if( sz.height  == 0 ) {
		zoomrate =  vr;
	}

	wsize = Size(img.cols * zoomrate, img.rows * zoomrate );

	Mat view;
	resize(img, view, wsize );

	if( zinfo[name]  != NULL  ) {
		delete zinfo[name];
	}
	zinfo[name] = new ZInfo{ img, view, wsize, zoomrate, Point(0,0) };
	imshow(name, view);

	setMouseCallback(name, handle_mouse, (void*)name);
}


void zoomview::handle_mouse(int evt, int x, int y, int flags, void *param){

 	 zoomview *it = zoomview::getIt();

	 char *name = (char*) param;
	 ZInfo * zi = it->zinfo[name];
	 Size ws = zi->wsize;



	 if(	flags& CV_EVENT_FLAG_CTRLKEY  &&
		( evt == CV_EVENT_LBUTTONDOWN ||   evt == CV_EVENT_RBUTTONDOWN ) ){

		 int cx = ws.width / 2, cy = ws.height /2 ;
		 int mx = ( x - cx  ) , my = (y - cy ) ;

		 zi->pt.x += mx, zi->pt.y += my;

		 switch( evt ){
		 case CV_EVENT_LBUTTONDOWN : zi->zrate *= 1.2;  break;
		 case CV_EVENT_RBUTTONDOWN : zi->zrate *= 0.8;	break;
		 }

		 float  vr  =  (float) ws.height / zi->src.rows, hr = (float) ws.width / zi->src.cols;
		 float  max_zr = hr>vr ? hr : vr;

		 if(zi->zrate > 1 ) zi->zrate = 1;
		 if(zi->zrate < max_zr )  zi->zrate = max_zr;


		 resize(zi->src, zi->view, Size( round(zi->src.cols * zi->zrate), round(zi->src.rows * zi->zrate) ) );

		 if( zi->pt.x  <  0 ) zi->pt.x = 0;
		 if(zi->pt.x + ws.width > zi->view.cols  ) zi->pt.x = zi->view.cols - ws.width;
		 if( zi->pt.y  <  0 ) zi->pt.y = 0;
		 if(zi->pt.y + ws.height > zi->view.rows  ) zi->pt.y = zi->view.rows - ws.height;

		 Rect nr =  Rect(zi->pt.x, zi->pt.y, ws.width, ws.height);

		 imshow( name, zi->view(nr) );


	 } else if( evt == CV_EVENT_LBUTTONDOWN ) sx  = x, sy = y;
	   else if( evt == CV_EVENT_LBUTTONUP ){

			 zi->pt.x += sx - x ,
			 zi->pt.y += sy - y ;
			 if( zi->pt.x  <  0 ) zi->pt.x = 0;
			 if( zi->pt.y  <  0 ) zi->pt.y = 0;
			 if(zi->pt.x + ws.width > zi->view.cols  ) zi->pt.x = zi->view.cols - ws.width;
			 if(zi->pt.y + ws.height > zi->view.rows  ) zi->pt.y = zi->view.rows - ws.height;

	 } else if( evt == CV_EVENT_MOUSEMOVE  ){

		   if( flags&CV_EVENT_FLAG_LBUTTON){
			 int mx = sx - x, my = sy - y ;
			 Rect nr =  Rect(zi->pt.x + mx, zi->pt.y + my, ws.width, ws.height);

			 if( nr.x < 0 ) nr.x = 0;
			 if( nr.y < 0 ) nr.y = 0;
			 if( nr.x + ws.width > zi->view.cols  ) nr.x = zi->view.cols - ws.width;
			 if( nr.y + ws.height > zi->view.rows  ) nr.y = zi->view.rows - ws.height;

			 imshow( name, zi->view(nr) );
		   } else {
			 int srcx = ( zi->pt.x + x ) / zi->zrate;
			 int srcy = ( zi->pt.y + y ) / zi->zrate;

			Mat tview = zi->view(Rect(zi->pt.x , zi->pt.y, ws.width, ws.height)).clone();
			char  text[20];

			sprintf(text,"%d,%d", srcx, srcy);
			putText(tview, text,Point(10,20),FONT_HERSHEY_SCRIPT_COMPLEX, 0.6, zoomview::textcolor);
			imshow( name, tview );

		  }
	 }

}

zoomview::zoomview() {
	// TODO Auto-generated constructor stub

}

zoomview::~zoomview() {
	// TODO Auto-generated destructor stub
}

} /* namespace sr_lib */
