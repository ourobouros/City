//
//  BoundingPolygon.h
//  Golden Triangle
//
//  Created by Alex Bullard on 1/6/11.
//  Copyright 2011 Middlebury College. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <vector>
#import "CityMath.h"

using namespace std;

struct CityNormal{
	float x,y,z;
	CityNormal(){
		
	};
	CityNormal(double _x, double _y, double _z){
		x = _x; y=_y; z=_z;
	}
};

struct CityVertex{
	float x,y,z;
	vector<int> faces;
	CityNormal vertexNormal;
	CityVertex(double _x, double _y, double _z){
		x = _x; y =_y; z = _z;
		faces = vector<int>();
	}
	CityVertex(){}
};

struct CityPolygon {
	vector<int> vertexList;
	CityNormal faceNormal;
	GLfloat diffuseLight[4];
	GLfloat specularLight[4];
	GLfloat emissiveLight[4];

	CityPolygon() {
		vertexList = vector<int>();
	}
	CityPolygon(vector <int> &vl, GLfloat dl[], GLfloat sl[], GLfloat el[]) {
		vertexList = vector<int>(vl);
		for(int i=0; i<4; i++){
			diffuseLight[i] = dl[i];
			specularLight[i] = sl[i];
			emissiveLight[i] = el[i];
		}
	}	
};

struct CityPolyObject {
	vector<CityVertex> * vp;
	vector<CityPolygon> * pp;
	vector<CityVertex>  vertices;
	vector<CityPolygon>  polygons;
	
	CityPolyObject(vector<CityVertex> &cv,vector<CityPolygon> &cp) {
		vp = &cv;
		pp = &cp;
		vertices = *vp;
		polygons = *pp;
//		vertices = vector<CityVertex>(cv);
//		polygons = vector<CityPolygon>(cp);
		generateNormals();
	}
	CityPolyObject(){
		
	}
	void generateNormals () {
		// Generate face normals
		for (int i=0; i<polygons.size(); i++) {
			CityVertex a = vertices[polygons[i].vertexList[0]]; 
			CityVertex b = vertices[polygons[i].vertexList[1]]; 
			CityVertex c = vertices[polygons[i].vertexList[2]];
			double normx = (a.z-b.z)*(c.y-b.y)-(a.y-b.y)*(c.z-b.z);
			double normy = (a.x-b.x)*(c.z-b.z)-(a.z-b.z)*(c.x-b.x);
			double normz = (a.y-b.y)*(c.x-b.x)-(a.x-b.x)*(c.y-b.y);
			double normlength = sqrt(normx*normx+normy*normy+normz*normz);
			polygons[i].faceNormal = CityNormal(normx /= normlength,normy /= normlength,normz /= normlength);
		
			// Add faces to vertex definitions
			for (int j=0; j<polygons[i].vertexList.size(); j++) {
				vertices[polygons[i].vertexList[j]].faces.push_back(i);
			}
		}
		// Generate vertex normals NOT NORMALIZED!
		for (int i=0; i<vertices.size(); i++){
			double tx=0.0, ty=0.0,tz=0.0;
			for (int j=0; j<vertices[i].faces.size(); j++) {
				tx += polygons[vertices[i].faces[j]].faceNormal.x;
				ty += polygons[vertices[i].faces[j]].faceNormal.y;
				tz += polygons[vertices[i].faces[j]].faceNormal.z;
			}
			vertices[i].vertexNormal = CityNormal(tx/vertices[i].faces.size(),ty/vertices[i].faces.size(),tz/vertices[i].faces.size());
		}
	}
};


@interface BoundingPolygon : NSObject {
	NSArray * coordinates;
	GLfloat emissive[4];
	float red;
	float green;
	float blue;
	bool border;
}

-(BoundingPolygon *) initWithCoord:(NSArray *)coord andColorRed:(float)r green:(float)g blue:(float)b border:(bool)bor;
-(BoundingPolygon *) initWithCoord:(NSArray *)coord andColorRed:(float)r green:(float)g blue:(float)b;
-(NSArray *) coordinates;
-(float) red;
-(float) green;
-(float) blue;
-(bool) border;
-(void) dealloc;

@end
