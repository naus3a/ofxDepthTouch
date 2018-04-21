//
//  PointCloud.h
//  kinectExample
//
//  Created by enrico<naus3a>viola on 4/4/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxKinect.h"

class PointCloud{
public:
    PointCloud();
    ~PointCloud();
    void setup(ofxKinect * _k, int Hstep, int Vstep);
    void clearVertices();
    void setStep(int Hstep, int Vstep);
    void update();
    void draw();
    inline vector<ofVec3f> & getVertices(){return vtx;}
    inline int getNumVertices(){return vtx.size();}
    inline int getKinectHorizontalStep(){return xStep;}
    inline int getKinectVerticalStep(){return yStep;}
protected:
    ofxKinect * kinect;
    vector<ofVec3f> vtx;
    ofVboMesh msh;
    int xStep;
    int yStep;
};