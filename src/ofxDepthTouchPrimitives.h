//
//  ofxDepthTouchPrimitives.h
//
//  Created by enrico<naus3a>viola on 4/4/17.
//
//

#pragma once
#include "ofMain.h"

namespace ofxDepthTouch{
    class TouchBox{
    public:
        virtual void set(ofVec3f _ctr, float w, float h, float d);
        virtual void draw();
        ofVec3f toMyCoordSpace(ofVec3f & pt);
        bool isInside(ofVec3f & pt);
        
        static void makeBoxMesh(ofVboMesh * _msh, ofVec3f & _ctr, float & _w, float & _h, float & _d);
        static void addFaceIndices(ofVboMesh * _msh, int i0, int i1, int i2, int i3);
        
        virtual void setCenter(ofVec3f _ctr);
        virtual void setCameraDistance(float mm);
        virtual void setWidth(float mm);
        virtual void setHeight(float mm);
        virtual void setDepth(float mm);
        
        virtual ofVec3f getCenter(){return ctr;}
        virtual float getCameraDistance(){return ctr.z;}
        virtual float getWidth(){return width;}
        virtual float getHeight(){return height;}
        virtual float getDepth(){return depth;}
    protected:
        bool isMySpacePtInside(ofVec3f & pt);
        
        ofVboMesh msh;
        
        ofVec3f ctr;
        float width;
        float height;
        float depth;
        float w2;
        float h2;
        float d2;
    };
    
    class TouchPanel : public TouchBox{
    public:
        class Pad{
        public:
            inline Pad(ofVec2f _crn, float w, float h, int _pId){set(_crn,w,h,_pId);bTouched=false;bOldTouched=false;}
            inline void set(ofVec2f _crn, float w, float h, int _pId){corner=_crn;width=w;height=h;pId=_pId;r.set(_crn.x,_crn.y,w,h);}
            ofVboMesh box;
            ofVec2f corner;
            float width;
            float height;
            ofRectangle r;
            int pId;
            bool bTouched;
            bool bOldTouched;
        };
        TouchPanel();
        void draw();
        void addPad(ofVec2f _crn, float w, float h, int _pId);
        void makePadGrid(int _rows, int _cols);
        void unsetAll();
        void testPoint(ofVec3f & pt);
        void checkStateChange();
        inline int getNumActivePads(){return nActivePads;}
        inline int getNumPads(){return pads.size();}
        inline int getNumRows(){return rows;}
        inline int getNumCols(){return cols;}
        inline vector<TouchPanel::Pad> * getPads(){return & pads;}
        
        void setWidth(float mm);
        void setHeight(float mm);
        void setDepth(float mm);
        
        void setRows(int n);
        void setCols(int n);
        
        ofEvent<int> evPadTouched;
        ofEvent<int> evPadReleased;
        
        ofColor colOn;
        ofColor colOff;
    protected:
        vector<TouchPanel::Pad> pads;
        int rows, cols;
        int nActivePads;
    };
};
