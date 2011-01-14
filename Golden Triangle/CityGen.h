//
//  CityGen.h
//  Golden Triangle
//
//  Created by Alex Bullard on 1/6/11.
//  Copyright 2011 Middlebury College. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BuildingObject.h"
#import "PlaneObject.h"
#import "RoadObject.h"
#import "voronoi.h"

using namespace std;

#define MINX -100
#define MAXX 100
#define MINZ -200
#define MAXZ 0
#define RANDSEED 0
#define NUMCONTROL 10
@interface CityGen : NSObject {

}

+ (NSMutableArray *) masterGenerate;
+ (void) addPlane:(NSMutableArray *)polygons;
+ (void) addCityBuildings:(NSMutableArray *) polygons3D diagram:(std::list<std::list<JPoint> >)polys centerX:(double)cx z:(double)cz maxDist:(double)mD;
@end
