/*
 * filmScanner.cpp
 *
 *  Created on: 2016. 12. 21.
 *      Author: decoz
 */

#include "linescan.h"

#include <math.h>
#include "srlib.h"


namespace srlib {

typedef Path::iterator Iter_point;


/** @brief 두 점의 거리를 구한다 */
inline float range(Point p1, Point p2 ) {
	return sqrt ( (float) ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y ) );
}

/**
 * @brief 두 rect 가 동일한 rect인가 검사
 */
inline  bool equal_rect(Rect r1, Rect r2){
	if( r1.x == r2.x && r1.y == r2.y
		&& r1.width == r2.width
		&& r1.height == r2.height
	) return true;
	else return false;
}


inline char *tostr(Path p){
	char* buffer = new char[100];
	Point sp = *(p.begin()), ep = *(p.end() - 1);
	sprintf(buffer,"%d,%d-%d,%d", sp.x, sp.y, ep.x, ep.y );
	return buffer;
}


inline char* tostr(Rect r){
	char *buff = new char[20];
	sprintf(buff, "%d,%d,%d,%d",r.x,r.y,r.width,r.height);
	return buff;
}



linescan::linescan() {
	// TODO Auto-generated constructor stub
	debug = false;
	adp_thresh = 10;
	merge_other_obj = true;

	line_max_width = 40;	// line 추출시 넓이 임계값
	line_min_length = 30;	// line 추출시 최소 길이

	assemble_range = 60; // 병합시 점들간의 최대거리
	assemble_thresh = 0.6;	 // 병합 임계값
}

linescan::~linescan() {
	// TODO Auto-generated destructor stub
	bimg.release();
	if(debug) printf("destroy linescan\n"); fflush(stdout);
}


/**
 * 	@brief 굵은 식각 보정
 * 	@detail
 * 	계층마다 여러 노이즈에 의해 분할된 영역(rod)들을 하나로 병합
 */
void merge_rods(Xobj *xobj){
	int gap_limits = 20;

	obj *hobj = xobj->hobj, *vobj = xobj->vobj;

	//printf("before hobj [%d] vobj [%d] \n", xobj->hobj->rods.size(), xobj->vobj->rods.size());
	typedef bunch::iterator  Iter_bunch;

	for(int ri =0;ri<2;ri++){
		obj *tobj = ri>0? xobj->hobj : xobj->vobj;
		bunch nrods;
		//printf("%s\n", ri? "horizontal": "vertical" );
		int mn, mx = -1;
		int c = -1;

		bunch t_layer;

		for(Iter_bunch i = tobj->rods.begin(); i != tobj->rods.end(); i++){

			if( i->layer > c || mx < 0){
				rod _r = {c, mn, mx};
				if(mx > 0) nrods.push_back(_r);
				mn = i->s, mx = i->e,	c = i->layer;
			}
			 else {
				if( i->s < mn ) mn = i->s;
				if( i->e > mx ) mx = i->e;
			}
		}

		rod _r = {c, mn, mx};
		nrods.push_back(_r);
		tobj->rods =  nrods;

	}

}

double line2angle(Point p1, Point p2){
	double ang1 = atan2( double(p2.y - p1.y), double(p2.x - p1.x)) * 180 / CV_PI;
	return ang1;
}


/**
 * @brief 두 선의 연결 접점 체크를 위해 리버싱 여부를 체크
 *
 * @details
 * 두 경로의 시작점을 sp1, sp2, 끝점을 ep1, ep2로 뒀을때
 * 두 경로의 접점위치가 ep1 - sp2  로 이어지도록 하기위해
 * 가장 가까운 거리에 따라  0~3까지를 리턴한다.
 *
 * 리턴값에 따른 4가지 거리와  스왑필요성은 다음과 같다.
 *
 * 0 : ep1, sp2		r: 0, 0
 * 1 : ep1, ep2		r: 0, 1	예)  ep1 ~ sp2 까지가 가장 가까우며 sp2<->ep2스왑이 필요
 * 2 : sp1, sp2		r: 1, 0
 * 3 : sp1, ep2		r: 1, 1
 *
 * 리턴값에 /2 를 하면 sp1, 과 ep1 의 스왑여부가 드러남
 * 리턴값에 %2 를 하면 sp2, 와 ep2 의 스왑여부가 드러남
 */
int order_points(Point &sp1, Point &ep1, Point &sp2, Point &ep2 ){

	Point p[4] = 	{ep1, sp1, sp2, ep2};

	float r, min = range(p[0], p[2]);

	int mi = 0;
	for(int i=1;i<4;i++){
		float r = range(p[i/2],  p[2+i%2]);
		if( min > r ) min = r, mi = i; // 가장 가까운 경로는 몇번째?
	}

	Point tp;
	if(mi/2) tp = sp1, sp1 = ep1, ep1 = tp;
	if(mi%2) tp = sp2, sp2 = ep2, ep2 = tp;

	return mi;
}

/** @brief 두 선의 연결 접점 체크를 위해 리버싱 여부를 체크  */
int order_paths(Path  &ph1, Path &ph2){

	//printf("%s:%s ",tostr(ph1), tostr(ph2));
	Point 	p1 = *ph1.begin(), p2 = *(ph1.end()-1),
			p3 = *ph2.begin(), p4 = *(ph2.end()-1);

	int order = order_points(p1,p2,p3,p4);

	if(order/2) 	reverse(ph1.begin(), ph1.end());
	if(order%2) 	reverse(ph2.begin(), ph2.end());

	//printf(" => %s:%s\n",tostr(ph1), tostr(ph2));

	return order;
}


/**
 * @brief 두 패스를 합친다.
 * @param hReverse (head reverse)
 * 		ph1 의 순서를 꺼꾸로 변환
 * 	@param tReverse (tail reverse)
 * 		ph2 의 순서를 꺼꾸로 변환
 */

Path linescan::merge_path(Path ph1, Path ph2 , bool hReverse , bool tReverse){

	bool debug = false;
	Path nph;

	if( debug) {
		Point 	sp1 = *ph1.begin(),  	sp2 = *ph2.begin(),
				ep1 = *(ph1.end()-1), 	ep2 = *(ph2.end() - 1);

		printf("  %s << %s \n", tostr(ph1), tostr(ph2));
		printf("%d,%d - %d,%d >> \n", sp2.x, sp2.y , ep2.x, ep2.y );
	}

	Point 	sp1 = *ph1.begin(),  	sp2 = *ph2.begin(),
			ep1 = *(ph1.end()-1), 	ep2 = *(ph2.end() - 1);

	int o = order_points(sp1, ep1,sp2, ep2);
	hReverse = o/2, tReverse = o%2;

	if(hReverse) reverse(ph1.begin(), ph1.end());
	if(tReverse) reverse(ph2.begin(), ph2.end());

	for(Iter_point i = ph1.begin(); i != ph1.end(); i++) 	nph.push_back(*i);
	for(Iter_point i = ph2.begin(); i != ph2.end(); i++)		nph.push_back(*i);

	return nph;
}



int  scan_line(Mat map,  Point p1, Point p2){
// p1 과 p2 사이의 픽셀값을 합산해서 리턴
	double ang = line2angle(p1, p2);

	Vec2f v2 = theta2vec( ang * CV_PI / 180);
	float vx = v2[0], vy = v2[1];

	int l = abs(vx) > abs(vy)? abs(p1.x - p2.x) : abs(p1.y - p2.y);
	int sum = 0;

	for(int i=0; i<l; i++){
		int nx = i*vx + p1.x, ny = i*vy + p1.y;
		uchar v = map.at<uchar>(ny, nx);
		//printf("   %d,%d : %d\n", nx, ny, v);
		sum += v;
	}

	return sum;

}

/**
 * @brief 두 직선의 각도의 벡터곱을 구한다.
 * @details
 *
 * 두 벡터의 각도의 차이가 0인경우 1 이  되고
 * 두 벡터의 각도의 차이가 180도인 경우 0 이 된다.
 *
 */
float	ang_sim(Path ph1 , Path ph2, int order){

	Point p1,p2,p3,p4;
	if( order / 2 )  p1 = *( ph1.begin() + 1  ), p2 = *(ph1.begin());	// ph1 의 앞부분이 접합부분
	else p1 = *(ph1.end() -2 ) , p2 = *(ph1.end() - 1);					// ph1 의 뒷부분이 접합부분

	if( order % 2 )  	p3 = *( ph2.end() -1  ), p4 = *(ph2.end() - 2);	// ph2 의 뒷부분이 접합부분
	else p3 = *(ph2.begin()) , p4 = *(ph2.begin() + 1);				// ph2 의 앞부분이 접합부분


	float 	ang = abs( line2ang(p3,p4) - line2ang(p1,p2));
	return cos( ang * CV_PI / 360 );												// 각도 유사도

}


/**
 * @ 두 점이 백색영역으로 이어져있는가를 판별한다.
 */
bool check_connected(Mat map, Path ph1, Path ph2, float thresh){

	Point 	tp1 = *ph1.begin(), tp2 = *(ph1.end()-1),
			tp3 = *ph2.begin(), tp4 = *(ph2.end()-1);

	order_points(tp1, tp2, tp3, tp4);

	Point p1 = tp2, p2 = tp3;

	int pixel_sum = scan_line(map, p1, p2);
	int xd = abs(p1.x - p2.x), yd = abs(p1.y - p2.y);
	int l  = xd>yd? xd : yd;
	double dens = (double)  pixel_sum / l / 255;

	//	if(thresh < 0.8 )	printf("\t %d,%d  - %d,%d :  con_dens:%.2f\n", p1.x, p1.y,  p2.x, p2.y, dens);

	if(dens < thresh) return false;
	else return true;
}


/**
 * @brief 두 패스의 접합 가능성을 검사 ( 0~1 )
 * @detail
 *  min_range 로 미니멈 거리를 주지 안으면  정확한 각도 계산을 위해 더글라스 패커를 사용하므로
 *  오버헤드가 크다.
 *
 */
float  linkability(Path ph1 , Path ph2, float max_range){

	//float angle_weight = 4;
	Point 	tp1 = *ph1.begin(), tp2 = *(ph1.end()-1),
			tp3 = *ph2.begin(), tp4 = *(ph2.end()-1);

	int order = order_points(tp1, tp2, tp3, tp4);

	// adp 로 선을 간략화
	float 	d_sum = range(tp1,tp2)  + range(tp3, tp4),
			dx = range(tp2,tp3);

	Point p1,p2,p3,p4;

	if( order / 2 )  p1 = *( ph1.begin() + 1  ), p2 = *(ph1.begin());	// ph1 의 앞부분이 접합부분
	else p1 = *(ph1.end() -2 ) , p2 = *(ph1.end() - 1);					// ph1 의 뒷부분이 접합부분

	if( order % 2 )  	p3 = *( ph2.end() -1  ), p4 = *(ph2.end() - 2);	// ph2 의 뒷부분이 접합부분
	else p3 = *(ph2.begin()) , p4 = *(ph2.begin() + 1);				// ph2 의 앞부분이 접합부분

	if(dx > max_range) return 0;
	// 단절 영역의 각도를 반영하여 distance 를 1에서 3까지 가중치를  부여한다.
	float th1 = line2ang(p1,p2), thx  = line2ang(p2,p3), th2 =  line2ang(p3,p4);
	float w = sin( abs(thx - th1) * CV_PI / 360 ) + sin( abs(th2 - thx) * CV_PI / 360 );
	dx *= 1+w;

	/*
	printf("     [%d,%d] [%d,%d] - [%d,%d] [%d,%d] : / %.0f, %.0f, %.0f :: %.2f, \n",
		p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y,
		th1, thx, th2, w
	);
	*/


	float angv = ang_sim(ph1, ph2, order);
	float dv =  d_sum / ( d_sum + dx );

	//printf("\t%s ~ %s : %.2f * %.2f = %.2f \n", tostr(ph1), tostr(ph2), angv, dv, angv * dv);

	return  abs(dv * angv);
}



/**
 * @brief 서로 떨어진 path 경로들을 조합
 *
 * @detail
 * 가까운 시작, 끝점을 공유하는 두 선을 찾아  l_thresh 이내의 거리일 경우 이를 합침
 * 단 역류( 접합점의 반대단 양 끝점이 각 라인의 길이보다 가까울 경우 )시 이를 무시
 */

void linescan::assemble_path(Xobj *xobj){


	bool debug = false;
	int l_thresh =  assemble_range ;

	typedef struct {	// 관계 계산용 구조체
		Iter_path iph;
		float r;
	} Rel;

	Rel *rels = new Rel[xobj->paths.size()];

	for(Iter_path i = xobj->paths.begin(); i != xobj->paths.end();  ){

		Point ih = *(i->begin()), it = *(i->end() -1 ) ;
		int hReverse = 0, tReverse = 0; 	// 0: right , 1:left

		// 가까운 공유점을 가진 라인들 탐색
		int c = 0;
		for(Iter_path j = xobj->paths.begin(); j != xobj->paths.end(); j++){
			if(  i == j ) continue;

			Point jh = *(j->begin()), jt = *(j->end() -1 ) ;
			int o = order_points(ih, it, jh, jt);
			float r = range(it,jh);

			Rel _rel = {j,r};
			//Rel _rel;

			if( r <  assemble_range) rels[c++] = _rel ;

		}

		// 정렬

		Rel trel;
		for(int n=0; n<c; n++)	for(int m=0; m<c-n-1; m++ )
			if( rels[m].r > rels[m+1].r ) trel = rels[m], rels[m] = rels[m+1], rels[m+1] = trel;

		if(debug){
			//printf("%s \n", tostr(*i));
			for(int n=0; n<c; n++) {
				Path ph = *(rels[n].iph);
				//printf(" - %s : %.2f\n", tostr(ph), rels[n].r );
			}
		}



		// 가장 가까운 점부터 연결 가능 점검 후 가능시 연결
		int n=0;
		for(; n<c; n++){
			Iter_path iph = rels[n].iph;
			//printf(" ~ %s : %.2f\n", tostr(*iph), rels[n].r );

			Point jh = *(iph->begin());

			if( rels[n].r < assemble_range &&
				( rels[n].r < 5 || check_connected(bimg,  *i, *iph, 0.9 ) ) &&
				linkability( *i, *iph, assemble_range ) > assemble_thresh )
			{
					*i = merge_path(*i, *iph, hReverse, tReverse);
					xobj->paths.erase( iph );
					break;
			}
		}

		// 연결 실패시 다음 라인으로
		if( n == c ) i++;

	}

	if(debug){
		printf("\t");
		for(Iter_path j = xobj->paths.begin(); j != xobj->paths.end(); j++){
			printf("%d-", j->size());
		} printf("\n");
	}

}




/*
 * @brief paths 리스트에 새로운 점을 추가한다.
 * @detail
 * paths 들중 점과 가장 가까운 멤버를 끝에 보유한 패스를 선정해
 * 그 패스끝에 점을 추가한다.
 */
bool linescan::link_point(vector <Path> &paths, Point p, int dir){

		int max_l = line_max_width / 2;
	bool  debug = true;
	double min_d = -1;
	Iter_path i_minpath;
	for(Iter_path i = paths.begin(); i != paths.end(); i++){

		Point tp = *(i->end() - 1);
		int tl = dir? tp.x : tp.y, cl = dir? p.x : p.y;

		float r = range(tp, p);
		if( tl != cl && (  r < min_d || min_d < 0  ) ) min_d = r, i_minpath = i;
	}

	if(min_d > 0) 	Point lp = *( i_minpath->end() - 1) ;

	if(min_d < max_l && min_d  > 0) {
		i_minpath->push_back(p);
		return true;
	} else {
		Path npath;
		npath.push_back(p);
		paths.push_back(npath);
		return false;
	}

}


/**
 *	@brief xobj의 패스를 추출
 *
 */

void linescan::calc_path(Xobj *xobj){

	bool debug = false;
	// layer index 생성
	int sw = bimg.cols, sh = bimg.rows;

	int min_size = 5;
	xobj->paths.clear();
	typedef bunch::iterator  Iter_bunch;
	obj *hobj = xobj->hobj, *vobj = xobj->vobj;

	int 	hs =  hobj->rods.begin()->layer,			// horizontal start
			vs =  vobj->rods.begin()->layer,			// horizontal end
			he = 	(hobj->rods.end() -1 )->layer,		// vertical start
			ve = 	(vobj->rods.end() -1 )->layer;		// vertical end

	if( hobj->rods.size() * vobj->rods.size() == 0  ) return;


	if(debug) {
		printf("hsp (%d,%d) %d:\n", hs,  hobj->rods.begin()->s, hobj->rods.size()); fflush(stdout);
		printf("h:%d-%d v:%d-%d\n", hs, he, vs, ve); fflush(stdout);
	}

	Iter_bunch hlayer[MAX_W], vlayer[MAX_H];
	//Iter_bunch hlayer[he-hs], vlayer[ve-vs]; // vs 에선 안먹어서 상위 코드로 바꿈...fuck you ms!

	// layer index 생성
	int c = hs;
	for(Iter_bunch i = hobj->rods.begin(); i != hobj->rods.end(); i++)
		if( i->layer >= c ) 	hlayer[i->layer - hs] = i, c++;
	c = vs;
	for(Iter_bunch i = vobj->rods.begin(); i != vobj->rods.end(); i++)
		if( i->layer >= c ) 	vlayer[i->layer - vs] = i, c++;

	// 비교 trace

	Path path;
	int omiss_thresh = 5, min_path = 0;

	vector <Path> paths;


	for(int dir=0; dir < 2; dir++){
		// dir 0: hscan ,  1: vscan
		paths.clear();
		obj* 			tobj 		= dir>0? 	hobj : vobj;
		Iter_bunch* 	o_layers 	= dir>0?  vlayer: hlayer;
		int 			 s			= dir>0? 	vs : hs;
		int omiss = 0, c = s;

		for(Iter_bunch i = tobj->rods.begin(); i != tobj->rods.end(); i++){
			int  	m = ( i->s +  i->e ) / 2,		// median
					ml =  i->layer,					// median layer(x)
					w = i->e -  i->s, 	w2 = 0;	// width, height


			if( !w ||  i->s == 0 ||  i->e == (dir?  sh : sw ) - 1) continue;

			for(Iter_bunch j = o_layers[m-s]; j->layer == m; j++){ // find cross direction width
				if(j->s < ml && j->e > ml) {
					w2 = j->e - j->s;
					break;
				}
			}
			Point p = dir>0? Point(ml,m) : Point(m,ml);
			if(w < w2 && w < line_max_width ) link_point(paths, p, dir) ;
		}

		for(Iter_path i = paths.begin(); i != paths.end(); i++)
			if(i->size() > line_min_length) {
				draw_path(dbgimg, *i, Scalar(255));
				Point p1 = *(i->begin()), p2 = *(i->end()-1);
				xobj->paths.push_back(*i);
			}
	}
	if(debug) printf("\tline cnt : [%d] \n",xobj->paths.size());

}


/**
 *	@brief obj 간의 path를 추출..
 *
 *
 */
vector <Path>  linescan::assemble_expath(vector <Path> paths){

	if(debug) printf("assemble_expath for %d paths \n", paths.size());
	bool debug = false;
	int l_thresh =  assemble_range ;

	typedef struct {	// 관계 계산용 구조체
		Iter_path iph;
		float r;
	} Rel;

	Rel *rels = new Rel[paths.size()];


	for(Iter_path i = paths.begin(); i != paths.end();  ){

		Point ih = *(i->begin()), it = *(i->end() -1 ) ;
		int hReverse = 0, tReverse = 0; 	// 0: right , 1:left

		// 가까운 공유점을 가진 라인들 탐색
		int c = 0;
		for(Iter_path j = paths.begin(); j != paths.end(); j++){
			if(  i == j ) continue;

			Point jh = *(j->begin()), jt = *(j->end() -1 ) ;
			int o = order_points(ih, it, jh, jt);
			float r = range(it,jh);

			Rel _rel = {j,r};
			//Rel _rel;

			if( r <  assemble_range) rels[c++] = _rel ;

		}

		// 정렬

		Rel trel;
		for(int n=0; n<c; n++)	for(int m=0; m<c-n-1; m++ )
			if( rels[m].r > rels[m+1].r ) trel = rels[m], rels[m] = rels[m+1], rels[m+1] = trel;

		if(debug){
			printf("%s \n", tostr(*i));
			for(int n=0; n<c; n++) {
				Path ph = *(rels[n].iph);
				//printf(" - %s : %.2f\n", tostr(ph), rels[n].r );
			}
		}


		// 가장 가까운 점부터 연결 가능 점검 후 가능시 연결
		int n=0;
		for(; n<c; n++){
			Iter_path iph = rels[n].iph;
			//printf(" ~ %s : %.2f\n", tostr(*iph), rels[n].r );

			if( rels[n].r < assemble_range && linkability( *i, *iph, assemble_range ) > assemble_thresh )
			{
					*i = merge_path(*i, *iph, hReverse, tReverse);
					paths.erase( iph );
					break;
			}
		}

		// 연결 실패시 다음 라인으로
		if( n == c ) i++;

	}

	if(debug){
		printf("\t");
		for(Iter_path j = paths.begin(); j != paths.end(); j++){
			printf("%d-", j->size());
		} printf("\n");
	}

	return paths;;


}

void linescan::draw_path(Mat img, Path ph, Scalar color, uchar drawflag){

	///bool show_guide = false;
	bool draw_point =  drawflag & flag_draw_point > 0 ? true : false;
	bool draw_edge =  drawflag & flag_draw_edge > 0 ? true : false;


	Vec3b v(color.val[0], color.val[1], color.val[2] );
	Point lp = *ph.begin();
	for(Iter_point i = ph.begin()+1; i != ph.end(); i++){
		if(lp.x >= 0) 	line(img, lp, *i, color, 1);

  		Point m = Point( (lp.x + i->x) / 2, (lp.y + i->y) / 2 );
		if(draw_point) circle(img, *i, 2, color );
		lp = *i;
	}
	if( /*ph.size() > 5 && */ draw_edge){
		circle(img, *ph.begin(), 7, color );
		circle(img, *(ph.end()-1), 7, color );
	}

}


/** @brief 전체 오브젝트의 라인 그리기  */
void linescan::DrawPaths(Mat img){
	/*
	for(Iter_xobj i=xobjs.begin(); i != xobjs.end(); i++)
		for(Iter_path ip = (*i)->paths.begin();  ip != (*i)->paths.end(); ip++)
			draw_path(img, *ip, Scalar(150,150,100));
	*/
	for(Iter_path ip = paths.begin();  ip != paths.end(); ip++)
		draw_path(img, *ip, Scalar(255,255,255), flag_draw_point | flag_draw_edge);

}


void filter_path(vector <Path> &paths, int min){
	for(Iter_path i = paths.begin(); i!=paths.end();i++){
		if(i->size()  < min )  {
			Iter_path j = i-1;
			paths.erase(i);
			i = j;
		}
	}
}


/** @brief 패스의 넓이값을 저장하기 위한 구조체 */
typedef struct{
	Point p;
	float w;
} Pw;



/** @brief cross scan 으로 Xobject 생성 */
vector <Xobj*> linescan::scanX(Mat gimg){

	objects.clear();
	loadImage(gimg);

	vector <obj> hobjs, vobjs;
	vector <Xobj*> xobjs;

	scan(dir_h);
	hobjs = objects;
	objects.clear();

	scan(dir_v);
	vobjs = objects;
	objects.clear();


	for(Iter_obj i=hobjs.begin(); i!=hobjs.end();  i++){
		Rect ir = i->getRect();
		for(Iter_obj j=vobjs.begin(); j !=vobjs.end();  j++){
			Rect jr = j->getRect();

			if( equal_rect(ir, jr) ) {
				Xobj*  x = new Xobj;
				obj *ho = new obj, *vo = new obj;
				*ho = *i, *vo = *j;
				x->hobj = ho,	x->vobj = vo;

				xobjs.push_back(x);
				break;
			}
		}
	}

	if(debug) printf("scan result:%d obj\n", xobjs.size());

	return xobjs;
}



/** @brief 패스 처리루틴을 통해 통합 패스를 생성 */

vector <Path>  linescan::scanPath(vector <Xobj*> xobjs, bool mode ){ // mode 0 : etch, 1: con
	//vector <Path>  paths;
	paths.clear();
	for(Iter_xobj ix = xobjs.begin(); ix != xobjs.end(); ix++ ){

		Xobj *xobj = *ix;

		calc_path(xobj);

		vector <Path> dpaths;
		for(Iter_path i = xobj->paths.begin(); i != xobj->paths.end(); i++ )
			dpaths.push_back(srlib::douglasPeucker(*i, adp_thresh , srlib::MODE_ADP ) );
		xobj->paths = dpaths;

		assemble_path(xobj);

		for(Iter_path i = xobj->paths.begin(); i != xobj->paths.end(); i++ )
			paths.push_back(*i);

	}

	if(merge_other_obj) paths = assemble_expath(paths);


	if(debug) printf("extracted path count:%d\n", paths.size());
	return paths;

}


/**
 * @brief sector 정보를 이미지로부터  스캔하여 정보를 생성
 * @details
 *
 * 생성되는 정보는 etching 영역, conductor 영역,
 *
 */
void 	linescan::scanline(Mat gimg){

	threshold(gimg, bimg, 5, 255, THRESH_BINARY);
	if(debug) dbgimg =  gimg / 2;

	xobjs = scanX(gimg);
	paths = scanPath(xobjs, 0);
	filter_path(paths, 2);


	if(debug) imshow("dbgimg", dbgimg);

}


} /* namespace filmInspector */


