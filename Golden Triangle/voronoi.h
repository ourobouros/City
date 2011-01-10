/*
 *  voronoi.h
 *  Golden Triangle
 *
 *  Created by Joe Redmon on 1/4/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#import <cmath>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <vector>
#import <queue>
#import <list>
#import <set>
#import <iostream>
#import <fstream>

using namespace std;

#define PI 3.1415926536

#define INF 9999999.9

#define FUDGE 0.0001
#define pinf JPoint(INF, INF)


double randf(double high, double low);

struct JPoint{
	double x,y;
	void print(){
		cout << "(" << x << ", " << y << ")\n";
	}
	JPoint(double _x, double _y){
		x = _x;
		y = _y;
	}
	JPoint(){
		x = 0.0;
		y = 0.0;
	}
	bool operator==(const JPoint &q){
		return (x+FUDGE > q.x && x-FUDGE < q.x && y+FUDGE > q.y && y-FUDGE < q.y);
	}
	bool operator!=(const JPoint &q){return !(operator==(q));}
	double distance(JPoint p){
		return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
	}
	double angle(JPoint A, JPoint C){
		double a = (*this).distance(C);
		double b = A.distance(C);
		double c = (*this).distance(A);
		double beta = acos((b*b - a*a - c*c)/(-2*a*c));
		return beta;	
	}
	void rotate(double angle){
		double newx = x*cos(angle) - y*sin(angle);
		double newy = x*sin(angle) + y*cos(angle);
		x = newx;
		y = newy;
	}	
};
struct Line{
	
	JPoint p,q;
	void print(){
		printf("(%f, %f) to (%f, %f)\n", p.x, p.y, q.x, q.y);
	}
	
	void Flip(){
		JPoint t = p;
		p = q;
		q = t;
	}
	
	Line(JPoint _p, JPoint _q){
		p = _p;
		q = _q;
	}
	Line Bisector(){
		return Line(JPoint((p.x+q.x)/2,(p.y+q.y)/2), JPoint((p.x+q.x)/2 + (p.y-q.y), (p.y+q.y)/2 - (p.x - q.x))); 
	}
	JPoint Intersection(Line l){
		if (((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)) == 0){
			return JPoint(INF, INF);
		}
		return JPoint(((p.x*q.y-p.y*q.x)*(l.p.x - l.q.x) - (p.x-q.x)*(l.p.x*l.q.y - l.p.y * l.q.x))/((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)),((p.x*q.y-p.y*q.x)*(l.p.y - l.q.y) - (p.y-q.y)*(l.p.x*l.q.y - l.p.y * l.q.x))/((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)));
	}
	bool isLeft(JPoint t){
		return ((q.x-p.x) * (t.y-p.y) - (q.y-p.y)*(t.x-p.x)) >= 0;
	}
	
};

struct Segment{
	JPoint p,q;
	pair<JPoint,JPoint> control;
	void print(){
		printf("(%f, %f) to (%f, %f)\n", p.x, p.y, q.x, q.y);
	}
	void Flip(){
		JPoint t = p;
		p = q;
		q = t;
	}
	Segment(JPoint _p, JPoint _q, pair<JPoint,JPoint> _c){
		p = _p;
		q = _q;
		control = _c;
	}
	
	Segment(JPoint _p, JPoint _q){
		p = _p;
		q = _q;
		control = make_pair(pinf, pinf);
	}
	Segment(const Segment& s){
		p = (s).p;
		q = (s).q;
		control = s.control;
	}
	JPoint Midd(){
		return JPoint((p.x+q.x)/2, (p.y+q.y)/2);
	}
	Line Bisector(){
		return Line(JPoint((p.x+q.x)/2,(p.y+q.y)/2), JPoint((p.x+q.x)/2 + (p.y-q.y), (p.y+q.y)/2 - (p.x - q.x))); 
	}
	bool operator==(const Segment &s){
		return (p == s.p && q == s.q) || (p == s.q && q == s.p);
	}
	bool sameDir(Segment s){
		return (p.y-q.y)*(s.p.x - s.q.x)+FUDGE >= (p.x-q.x)*(s.p.y - s.q.y) && (p.y-q.y)*(s.p.x - s.q.x)-FUDGE <= (p.x-q.x)*(s.p.y - s.q.y);
	}
	JPoint Intersection(Line l){
		if (((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)) == 0){
			return JPoint(INF, INF);
		}
		JPoint pot(((p.x*q.y-p.y*q.x)*(l.p.x - l.q.x) - (p.x-q.x)*(l.p.x*l.q.y - l.p.y * l.q.x))/((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)),((p.x*q.y-p.y*q.x)*(l.p.y - l.q.y) - (p.y-q.y)*(l.p.x*l.q.y - l.p.y * l.q.x))/((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)));
		if (pot.x+FUDGE >= min(p.x, q.x) && pot.x-FUDGE <= max(p.x, q.x) && pot.y+FUDGE >= min(p.y, q.y) && pot.y-FUDGE <= max(p.y, q.y)){
			return pot;
		}else{
			return JPoint(INF,INF);
		}
	}
	JPoint Intersection(Segment l){
		if (((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)) == 0){
			return JPoint(INF, INF);
		}
		JPoint pot(((p.x*q.y-p.y*q.x)*(l.p.x - l.q.x) - (p.x-q.x)*(l.p.x*l.q.y - l.p.y * l.q.x))/((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)),((p.x*q.y-p.y*q.x)*(l.p.y - l.q.y) - (p.y-q.y)*(l.p.x*l.q.y - l.p.y * l.q.x))/((p.x-q.x)*(l.p.y-l.q.y) - (p.y-q.y)*(l.p.x-l.q.x)));
		if (pot.x+FUDGE >= min(p.x, q.x) && pot.x-FUDGE <= max(p.x, q.x) && pot.y+FUDGE >= min(p.y, q.y) && pot.y-FUDGE <= max(p.y, q.y) 
			&& (pot.x+FUDGE >= min(l.p.x, l.q.x) && pot.x-FUDGE <= max(l.p.x, l.q.x) && pot.y+FUDGE >= min(l.p.y, l.q.y) && pot.y-FUDGE <= max(l.p.y, l.q.y))){
			return pot;
		}else{
			return JPoint(INF,INF);
		}
	}
	bool isLeft(JPoint t){
		return ((q.x-p.x) * (t.y-p.y) - (q.y-p.y)*(t.x-p.x)) >= 0;
	}	
};

struct Voronoi{
	list<Segment> bounds;
	list<JPoint> controlPoints;
	list<Segment> diagram;
	
	void draw(){
		glBegin(GL_LINES);
		glColor3f( 1, 0, 0 );
		for(list<Segment>::iterator sit = diagram.begin(); sit != diagram.end(); ++sit){
			glVertex3d(sit->p.x, -.9, sit->p.y);
			glVertex3d(sit->q.x, -.9, sit->q.y);
		}
		glEnd();
		glColor3f(0,1,1);
		for(list<JPoint>::iterator pit = controlPoints.begin(); pit != controlPoints.end(); ++pit){
			glBegin(GL_QUADS);
			
			glVertex3d(pit->x-.1,-.9,pit->y-.1);
			glVertex3d(pit->x-.1,-.9,pit->y+.1);
			glVertex3d(pit->x+.1,-.9,pit->y+.1);
			glVertex3d(pit->x+.1,-.9,pit->y-.1);
			
			glEnd();
			
		}
	}
	
	void draw2(){
		list<list<JPoint> > pts = getPolygons();
		for(list<list<JPoint> >::iterator ptsi = pts.begin(); ptsi != pts.end(); ++ptsi){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_POLYGON);
			glColor3f(0,1,1);
			for(list<JPoint>::iterator p = (*ptsi).begin(); p != (*ptsi).end(); ++p){
				glVertex3d(p->x, -.9, p->y);
			}
			glEnd();
		}
	}
	
	Voronoi(){
		controlPoints.clear();
		diagram.clear();
		bounds.clear();
		
		JPoint bl( -5.0,  -5.0);
		JPoint br(  5.0,  -5.0);
		JPoint ur(  5.0, -15.0);
		JPoint ul( -5.0, -15.0);
		
		bounds.push_back(Segment(bl,br));
		bounds.push_back(Segment(br,ur));
		bounds.push_back(Segment(ur,ul));
		bounds.push_back(Segment(ul,bl));
		
	}
	Voronoi(list<Segment> _bounds){
		controlPoints.clear();
		diagram.clear();
		bounds = _bounds;		
	}
	
	list<list<JPoint> > getPolygons(){
		cout << "hey\n";
		list<list<JPoint> > polys;
		for(list<JPoint>::iterator pit = controlPoints.begin(); pit != controlPoints.end(); ++pit){
			cout << 1 << endl;
			list<Segment> s;
			cout << "*************\n";
			(*pit).print();
			cout << "*************\n";
			
			for (list<Segment>::iterator sit = diagram.begin(); sit != diagram.end(); ++sit){
				sit->control.first.print();
				sit->control.second.print();
				cout << endl;
				if (sit->control.first == *pit || sit->control.second == *pit){
					cout << 2<<endl;
					
					s.push_back(*sit);
				}
			}
			cout << 2.5<< endl;
			list<JPoint> poly;
			Segment f = s.front();
			cout << 2.6 << endl;
			s.pop_front();
			cout << 2.7 << endl;
			poly.push_front(f.p);
			poly.push_front(f.q);
			cout << 2.8<< endl;
			for(list<Segment>::iterator sit = s.begin(); sit != s.end();){
				cout << 3 << endl;
				if(sit->p == poly.front()){
					poly.push_front(sit->q);
					s.erase(sit);
					sit = s.begin();
				}else if(sit->p == poly.back()){
					poly.push_back(sit->q);
					s.erase(sit);
					sit = s.begin();
				}else if(sit->q == poly.front()){
					poly.push_front(sit->p);
					s.erase(sit);
					sit = s.begin();
				}else if (sit->q == poly.back()) {
					poly.push_back(sit->p);
					s.erase(sit);
					sit = s.begin();
				}else{
					++sit;
				}
			}
			if(poly.front() == poly.back()){
				poly.pop_front();
			}
			polys.push_back(poly);
		}
		cout << 4 << endl;
		return polys;
	}
	
	JPoint closestPoint(JPoint &p){
		float minDist = 9999999.9;
		JPoint minPoint(INF, INF);
		for (list<JPoint>::iterator pit = controlPoints.begin(); pit != controlPoints.end(); ++pit){
			if((pit->x != p.x || pit->y != p.y) && p.distance(*pit)<minDist){
				minPoint = (*pit);
				minDist =  p.distance(*pit);
			}
		}
		return minPoint;
	}
	void addPoint(JPoint a){
		for(list<JPoint>::iterator  pit = controlPoints.begin(); pit != controlPoints.end(); ++pit){
			Line bi = Segment(a, *pit).Bisector();
			JPoint p = pinf, q = pinf;
			for(list<Segment>::iterator bit = bounds.begin(); bit!= bounds.end(); ++bit){
				JPoint t = (*bit).Intersection(bi);
				if(t != pinf){
					if (p != pinf && t != p){
						q = t;
					}else{
						p = t;
					}
				}
			}
			Segment bis = Segment(p,q, make_pair(a,*pit));
			for(list<Segment>::iterator dit = diagram.begin(); dit != diagram.end();){
				JPoint c = (*dit).Intersection(bis);
				if (c != pinf && ((*dit).control.first == *pit || (*dit).control.second == *pit)){
					if((*dit).q != c && ((*dit).p.distance(a)+FUDGE < (*dit).p.distance((*dit).control.first) || (*dit).p == c)){
						diagram.push_front(Segment(c,(*dit).q,(*dit).control));
					}else if((*dit).p != c){
						diagram.push_front(Segment(c,(*dit).p,(*dit).control));
					}
					
					if(bis.p.distance((*dit).control.first)+FUDGE < bis.p.distance(a) || bis.p.distance((*dit).control.second)+FUDGE < bis.p.distance(a)){
						bis.p = c;
					}else if(bis.p != c){
						bis.q = c;
					}
					dit = diagram.erase(dit);
				}else if(((*dit).control.first == *pit || (*dit).control.second == *pit) && (*dit).p.distance(a)-FUDGE < (*dit).p.distance(*pit) && (*dit).q.distance(a)-FUDGE < (*dit).q.distance(*pit)){
					dit = diagram.erase(dit);
				}else{
					++dit;
				}
				
			}
			for(list<JPoint>::iterator  pit2 = controlPoints.begin(); pit2 != controlPoints.end(); ++pit2){
				if((*pit2).distance(bis.p)+FUDGE < bis.p.distance(bis.control.first) || (*pit2).distance(bis.q)+FUDGE < bis.q.distance(bis.control.first)){
					bis.p = bis.q;
				}
			}
			if(bis.p != bis.q) diagram.push_back(bis);
		}
		controlPoints.push_back(a);
	}
};



Voronoi GenerateVoronoi(int seed, int numControl, double minx, double maxx, double miny, double maxy);
