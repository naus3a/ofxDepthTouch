//
//  ofxDepthTouchPrimitives.cpp
//  kinectExample
//
//  Created by nausea on 4/4/17.
//
//

#include "ofxDepthTouchPrimitives.h"

using namespace ofxDepthTouch;

void TouchBox::set(ofVec3f _ctr, float w, float h, float d){
    ctr = _ctr;
    width = w;
    height = h;
    depth = d;
    
    w2=w/2;
    h2=h/2;
    d2=d/2;
    
    makeBoxMesh(&msh, ctr, w, h, d);
}

void TouchBox::setCenter(ofVec3f _ctr){
    ofVec3f diff = _ctr-ctr;
    ctr = _ctr;
    for(int i=0;i<msh.getNumVertices();i++){
        ofVec3f v = msh.getVertex(i);
        v += diff;
        msh.setVertex(i, v);
    }
}

void TouchBox::setCameraDistance(float mm){
    setCenter(ofVec3f(0,0,mm));
}

void TouchBox::setWidth(float mm){
    width = mm;
    w2=width/2;
    
    float minX = ctr.x-w2;
    float maxX = ctr.x+w2;
    
    const int nVtx = 4;
    const int minIdx[nVtx] = {3,0,7,4};
    const int maxIdx[nVtx] = {2,1,6,5};
    
    for(int i=0;i<nVtx;i++){
        ofVec3f vMin = msh.getVertex(minIdx[i]);
        ofVec3f vMax = msh.getVertex(maxIdx[i]);
        vMin.x = minX;
        vMax.x = maxX;
        msh.setVertex(minIdx[i], vMin);
        msh.setVertex(maxIdx[i], vMax);
    }
}

void TouchBox::setHeight(float mm){
    height = mm;
    h2 = height/2;
    
    float minY = ctr.y-h2;
    float maxY = ctr.y+h2;
    
    const int nVtx = 4;
    const int minIdx[nVtx] = {3,0,2,1};
    const int maxIdx[nVtx] = {7,4,6,5};
    
    for(int i=0;i<nVtx;i++){
        ofVec3f vMin = msh.getVertex(minIdx[i]);
        ofVec3f vMax = msh.getVertex(maxIdx[i]);
        vMin.y = minY;
        vMax.y = maxY;
        msh.setVertex(minIdx[i], vMin);
        msh.setVertex(maxIdx[i], vMax);
    }
}

void TouchBox::setDepth(float mm){
    depth = mm;
    d2 = depth/2;
    
    float minZ = ctr.z-d2;
    float maxZ = ctr.z+d2;
    
    const int nVtx = 4;
    const int minIdx[nVtx] = {3,2,7,6};
    const int maxIdx[nVtx] = {0,1,4,5};
    
    for(int i=0;i<nVtx;i++){
        ofVec3f vMin = msh.getVertex(minIdx[i]);
        ofVec3f vMax = msh.getVertex(maxIdx[i]);
        vMin.z = minZ;
        vMax.z = maxZ;
        msh.setVertex(minIdx[i], vMin);
        msh.setVertex(maxIdx[i], vMax);
    }
}

void TouchBox::makeBoxMesh(ofVboMesh * _msh, ofVec3f & _ctr, float & _w, float & _h, float & _d){
    _msh->clear();
    _msh->setUsage(GL_STATIC_DRAW);
    _msh->setMode(OF_PRIMITIVE_TRIANGLES);
    
    float minX = _ctr.x-_w/2;
    float maxX = _ctr.x+_w/2;
    float minY = _ctr.y-_h/2;
    float maxY = _ctr.y+_h/2;
    float minZ = _ctr.z-_d/2;
    float maxZ = _ctr.z+_d/2;
    
    //    3-0 7-4
    // <  |t| |b|
    //    2-1 6-5
    
    ofVec3f vTLB(minX,maxY,minZ);
    ofVec3f vTRB(maxX,maxY,minZ);
    ofVec3f vTRF(maxX,maxY,maxZ);
    ofVec3f vTLF(minX,maxY,maxZ);
    
    ofVec3f vBLB(minX,minY,minZ);
    ofVec3f vBRB(maxX,minY,minZ);
    ofVec3f vBRF(maxX,minY,maxZ);
    ofVec3f vBLF(minX,minY,maxZ);
    
    _msh->addVertex(vTLB);
    _msh->addVertex(vTRB);
    _msh->addVertex(vTRF);
    _msh->addVertex(vTLF);
    _msh->addVertex(vBLB);
    _msh->addVertex(vBRB);
    _msh->addVertex(vBRF);
    _msh->addVertex(vBLF);
    
    addFaceIndices(_msh, 0, 1, 2, 3);//top
    addFaceIndices(_msh, 4, 5, 6, 7);//bottom
    addFaceIndices(_msh, 0, 1, 5, 4);//back
    addFaceIndices(_msh, 3, 2, 6, 7);//front
    addFaceIndices(_msh, 0, 3, 7, 4);//left
    addFaceIndices(_msh, 2, 1, 5, 6);//right
}

void TouchBox::addFaceIndices(ofVboMesh * _msh, int i0, int i1, int i2, int i3){
    _msh->addTriangle(i0, i1, i3);
    _msh->addTriangle(i3, i1, i2);
}

void TouchBox::draw(){
    msh.drawWireframe();
}

ofVec3f TouchBox::toMyCoordSpace(ofVec3f &pt){
    //todo: add rotations
    ofVec3f np = pt-ctr;
    return np;
}

bool TouchBox::isInside(ofVec3f & pt){
    ofVec3f iPt = toMyCoordSpace(pt);
    return isMySpacePtInside(iPt);
}

bool TouchBox::isMySpacePtInside(ofVec3f &pt){
    return (pt.x>(-w2) && pt.x<(w2) && pt.y>(-h2) && pt.y<h2 && pt.z>(-d2) && pt.x<d2);
}

TouchPanel::TouchPanel(){
    nActivePads = 0;
    colOn.set(255,0,0,255);
    colOff.set(255, 255, 255, 50);
}

void TouchPanel::makePadGrid(int _rows, int _cols){
    pads.clear();
    rows = _rows;
    cols = _cols;
    float padW = width/(rows);
    float padH = depth/(cols);
    int _pId = 0;
    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            addPad(ofVec2f((float(c)*padW)-w2,-d2+(float(r)*padH)), padW, padH, _pId);
            _pId++;
        }
    }
}

void TouchPanel::addPad(ofVec2f _crn, float w, float h, int _pId){
    pads.push_back(TouchPanel::Pad(_crn,w,h,_pId));
    ofVec3f pCtr = ofVec3f(_crn.x+w/2,ctr.y,_crn.y+h/2);
    makeBoxMesh(&pads[pads.size()-1].box, pCtr, w, height, h);
}

void TouchPanel::unsetAll(){
    nActivePads = 0;
    for(int i=0;i<pads.size();i++){
        pads[i].bTouched = false;
    }
}

void TouchPanel::testPoint(ofVec3f & pt){
    ofVec3f iPt = toMyCoordSpace(pt);
    if(isMySpacePtInside(iPt)){
        for(int i=0;i<pads.size();i++){
            if(!pads[i].bTouched){
                if(pads[i].r.inside(iPt.x, iPt.z)){
                    pads[i].bTouched=true;
                    nActivePads++;
                    return;
                }
            }
        }
    }
}

void TouchPanel::checkStateChange(){
    for(int i=0;i<pads.size();i++){
        if(pads[i].bTouched){
            if(!pads[i].bOldTouched){
                ofNotifyEvent(evPadTouched, i, this);
            }
        }else{
            if(pads[i].bOldTouched){
                ofNotifyEvent(evPadReleased, i, this);
            }
        }
        pads[i].bOldTouched = pads[i].bTouched;
    }
}

void TouchPanel::draw(){
    TouchBox::draw();
    ofPushMatrix();
    ofTranslate(ctr.x, ctr.y, ctr.z);
    ofPushStyle();
    for(int i=0;i<pads.size();i++){
        if(pads[i].bTouched){
            ofSetColor(colOn);
        }else{
            ofSetColor(colOff);
        }
        pads[i].box.drawWireframe();
    }
    ofPopStyle();
    ofPopMatrix();
}

void TouchPanel::setRows(int n){
    makePadGrid(n,n);
}

void TouchPanel::setCols(int n){
    makePadGrid(n, n);
}

void TouchPanel::setWidth(float mm){
    TouchBox::setWidth(mm);
    makePadGrid(rows, cols);
}

void TouchPanel::setHeight(float mm){
    TouchBox::setHeight(mm);
    makePadGrid(rows, cols);
}

void TouchPanel::setDepth(float mm){
    TouchBox::setDepth(mm);
    makePadGrid(rows, cols);
}