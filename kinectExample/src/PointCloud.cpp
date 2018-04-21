//
//  PointCloud.cpp
//  kinectExample
//
//  Created by enrico<naus3a>viola on 4/4/17.
//
//

#include "PointCloud.h"

PointCloud::PointCloud(){
    kinect = NULL;
    xStep = 1;
    yStep = 1;
    msh.setUsage(GL_DYNAMIC_DRAW);
    msh.setMode(OF_PRIMITIVE_POINTS);
}

PointCloud::~PointCloud(){
    clearVertices();
}

void PointCloud::clearVertices(){
    vtx.clear();
}

void PointCloud::setStep(int Hstep, int Vstep){
    xStep=Hstep;
    yStep=Vstep;
    clearVertices();
    for(int y=0;y<480;y+=yStep){
        for(int x=0;x<640;x+=xStep){
            vtx.push_back(ofVec3f(-320+x,-240+y,0));
        }
    }
}

void PointCloud::setup(ofxKinect * _k, int Hstep, int Vstep){
    kinect = _k;
    setStep(Hstep, Vstep);
}

void PointCloud::update(){
    msh.clear();
    int vId = 0;
    for(int y=479;y>=0;y-=yStep){
        for(int x=0;x<640;x+=xStep){
            vtx[vId].z = kinect->getDistanceAt(x, y);
            msh.addVertex(vtx[vId]);
            msh.addColor(kinect->getColorAt(x, y));
            vId++;
        }
    }
}

void PointCloud::draw(){
    ofPushStyle();
    glPointSize(3);
    msh.drawVertices();
    glPointSize(1);
    ofPopStyle();
}