//
//  ofxDepthTouchPad.cpp
//  kinectExample
//
//  Created by nausea on 4/4/17.
//
//

#include "ofxDepthTouch.h"

using namespace ofxDepthTouch;

void DepthGeneric::setupSensor(float fovW, float fovH){
    mshSensor.clear();
    mshSensor.setUsage(GL_STATIC_DRAW);
    mshSensor.setMode(OF_PRIMITIVE_LINES);
    float rayL = 3000;
    ofVec3f rayCtr(0,0,rayL);
    
    ofVec3f ray0 = rayCtr;
    ray0.rotate(-fovW/2, ofVec3f(0,1,0));
    ray0.rotate(-fovH/2, ofVec3f(1,0,0));
    mshSensor.addVertex(ofVec3f(0,0,0));
    mshSensor.addVertex(ray0);
    
    ofVec3f ray1 = rayCtr;
    ray1.rotate(fovW/2, ofVec3f(0,1,0));
    ray1.rotate(-fovH/2, ofVec3f(1,0,0));
    mshSensor.addVertex(ofVec3f(0,0,0));
    mshSensor.addVertex(ray1);
    
    ofVec3f ray2 = rayCtr;
    ray2.rotate(fovW/2, ofVec3f(0,1,0));
    ray2.rotate(fovH/2, ofVec3f(1,0,0));
    mshSensor.addVertex(ofVec3f(0,0,0));
    mshSensor.addVertex(ray2);
    
    ofVec3f ray3 = rayCtr;
    ray3.rotate(-fovW/2, ofVec3f(0,1,0));
    ray3.rotate(fovH/2, ofVec3f(1,0,0));
    mshSensor.addVertex(ofVec3f(0,0,0));
    mshSensor.addVertex(ray3);
}

void DepthGeneric::drawDebug(){
    ofPushStyle();
    ofSetColor(ofColor::yellow);
    mshSensor.drawWireframe();
    ofPopStyle();
}

void DepthGeneric::testPoints(vector<ofVec3f>& pts){
    for(int i=0;i<pts.size();i++){
        iTestPoint(pts[i]);
    }
}

void DepthGeneric::testPoint(ofVec3f & pt){
    iTestPoint(pt);
}

DepthPanel::DepthPanel(){
    //by default let's make a good ol' kinect
    setupSensor(58.5, 46.6);
    tPanel.set(ofVec3f(0,0,1000), 1000, 100, 1000);
}

void DepthPanel::setupGrid(int rows, int cols){
    tPanel.makePadGrid(rows, cols);
}

void DepthPanel::testPoints(vector<ofVec3f> &pts){
    tPanel.unsetAll();
    for(int i=0;i<pts.size();i++){
        if(tPanel.getNumActivePads()>=tPanel.getNumPads())return;
        iTestPoint(pts[i]);
    }
    tPanel.checkStateChange();
}

void DepthPanel::iTestPoint(ofVec3f & pt){
    tPanel.testPoint(pt);
}

void DepthPanel::drawDebug(){
    DepthGeneric::drawDebug();
    ofPushStyle();
    ofSetColor(255, 255, 255, 100);
    tPanel.draw();
    ofPopStyle();
}

DynamicPanel::DynamicPanel(){
    DepthPanel::DepthPanel();
    prePanel.set(ofVec3f(0,-100,1000), 1000, 100, 1000);
    prePanel.colOff.set(255, 255, 255, 25);
    prePanel.colOn.set(255, 255, 0, 255);
}

bool DynamicPanel::setupFromXml(string _pth){
    ofxXmlSettings xml;
    if(xml.load(_pth)){
        if(xml.tagExists("panel")){
            xml.pushTag("panel");
            int _rows = xml.getValue("rows", 8);
            int _cols = xml.getValue("cols", 8);
            cout<<"DynamicPanel::setupFromXml: "<<endl;
            cout<<" rows: "<<_rows<<" cols: "<<_cols<<endl;
            ofVec3f _ctr(0,0,1000);
            float _w = 1000;
            float _h = 100;
            float _d = 1000;
            if(xml.tagExists("touch")){
                xml.pushTag("touch");
                cout<<" Touch:"<<endl;
                if(xml.tagExists("colors")){
                    xml.pushTag("colors");
                    if(xml.tagExists("on")){
                        tPanel.colOn.r = xml.getAttribute("on", "r", 255);
                        tPanel.colOn.g = xml.getAttribute("on", "g", 0);
                        tPanel.colOn.b = xml.getAttribute("on", "b", 0);
                        tPanel.colOn.a = xml.getAttribute("on", "a", 255);
                        cout<<"     color on: "<<tPanel.colOn<<endl;
                    }
                    if(xml.tagExists("off")){
                        tPanel.colOff.r = xml.getAttribute("off", "r", 255);
                        tPanel.colOff.g = xml.getAttribute("off", "g", 255);
                        tPanel.colOff.b = xml.getAttribute("off", "b", 255);
                        tPanel.colOff.a = xml.getAttribute("off", "a", 0);
                        cout<<"     color off: "<<tPanel.colOff<<endl;
                    }
                    xml.popTag();
                }
                if(xml.tagExists("center")){
                    _ctr.x = xml.getAttribute("center", "x", _ctr.x);
                    _ctr.y = xml.getAttribute("center", "y", _ctr.y);
                    _ctr.z = xml.getAttribute("center", "z", _ctr.z);
                }
                _w = xml.getValue("width", 1000);
                _h = xml.getValue("height", 100);
                _d = xml.getValue("depth", 1000);
                cout<<"     panel center: "<<_ctr<<endl;
                cout<<"     panel size: "<<_w<<"x"<<_h<<"x"<<_d<<endl;
                tPanel.set(_ctr, _w, _h, _d);
                xml.popTag();
            }
            float preTouchDpt = 100;
            if(xml.tagExists("preTouch")){
                xml.pushTag("preTouch");
                cout<<" preTouch:"<<endl;
                if(xml.tagExists("colors")){
                    xml.pushTag("colors");
                    if(xml.tagExists("on")){
                        prePanel.colOn.r = xml.getAttribute("on", "r", 255);
                        prePanel.colOn.g = xml.getAttribute("on", "g", 0);
                        prePanel.colOn.b = xml.getAttribute("on", "b", 0);
                        prePanel.colOn.a = xml.getAttribute("on", "a", 255);
                        cout<<"     color on: "<<tPanel.colOn<<endl;
                    }
                    if(xml.tagExists("off")){
                        prePanel.colOff.r = xml.getAttribute("off", "r", 255);
                        prePanel.colOff.g = xml.getAttribute("off", "g", 255);
                        prePanel.colOff.b = xml.getAttribute("off", "b", 255);
                        prePanel.colOff.a = xml.getAttribute("off", "a", 0);
                        cout<<"     color off: "<<tPanel.colOff<<endl;
                    }
                    xml.popTag();
                }
                preTouchDpt = xml.getValue("thickness", preTouchDpt);
                cout<<"     thickness: "<<preTouchDpt<<endl;
                xml.popTag();
            }
            prePanel.set(ofVec3f(tPanel.getCenter().x, tPanel.getCenter().y-preTouchDpt, tPanel.getCenter().z),
                         tPanel.getWidth(), tPanel.getHeight(), tPanel.getDepth());
            setupGrid(_rows, _cols);
            xml.popTag();
            return true;
        }else{
            ofLogError("DynamicPanel::setupFromXml")<<"malformed document; cannot find <panel tag> ";
            return false;
        }
    }else{
        ofLogError("DynamicPanel::setupFromXml")<<"cannot open "<<_pth;
        return false;
    }
}

void DynamicPanel::setupGrid(int rows, int cols){
    DepthPanel::setupGrid(rows, cols);
    prePanel.makePadGrid(rows, cols);
}

void DynamicPanel::iTestPoint(ofVec3f &pt){
    DepthPanel::iTestPoint(pt);
    //prePanel.testPoint(pt);
}

void DynamicPanel::testPoints(vector<ofVec3f> &pts){
    DepthPanel::testPoints(pts);
    
    prePanel.unsetAll();
    for(int i=0;i<pts.size();i++){
        if(prePanel.getNumActivePads()>=prePanel.getNumPads())return;
        prePanel.testPoint(pts[i]);
    }
    prePanel.checkStateChange();
}

void DynamicPanel::drawDebug(){
    DepthPanel::drawDebug();
    prePanel.draw();
}
