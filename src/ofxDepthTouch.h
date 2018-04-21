//
//  ofxDepthTouchPad.h
//
//  Created by enrico<naus3a>viola on 4/4/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxDepthTouchPrimitives.h"

namespace ofxDepthTouch{
    class DepthGeneric{
    public:
        virtual void setupSensor(float fovW, float fovH);
        virtual void drawDebug();
        virtual void testPoints(vector<ofVec3f> & pts);
        virtual void testPoint(ofVec3f & pt);
        virtual TouchBox * getBoundingVolume()=0;
    protected:
        virtual void iTestPoint(ofVec3f & pt)=0;
        
        ofVboMesh mshSensor;
    };

    class DepthPanel : public DepthGeneric{
    public:
        DepthPanel();
        virtual void setupGrid(int rows, int cols);
        virtual void drawDebug();
        virtual void testPoints(vector<ofVec3f> & pts);
        TouchBox * getBoundingVolume(){return (TouchBox *)&tPanel;}
        TouchPanel * getPanel(){return & tPanel;}
    protected:
        virtual void iTestPoint(ofVec3f & pt);
        
        TouchPanel tPanel;
    };
    
    class DynamicPanel : public DepthPanel{
    public:
        DynamicPanel();
        bool setupFromXml(string _pth);
        void setupGrid(int rows, int cols);
        void drawDebug();
        void testPoints(vector<ofVec3f> & pts);
        TouchPanel * getPrePanel(){return & prePanel;}
    protected:
        void iTestPoint(ofVec3f & pt);
        
        TouchPanel prePanel;
    };
}
