//
//  dpScoreSceneBodyGlobe.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#include "dpScoreSceneBodyGlobe.h"

DP_SCORE_NAMESPACE_BEGIN

SceneBodyGlobe::Node::Node()
{
    points.clear();
    vertices.clear();
    vertices.assign(kMaxPoints, ofVec3f::zero());
    vbo.setVertexData(&vertices.at(0), vertices.size(), GL_DYNAMIC_DRAW);
}

SceneBodyGlobe::Node::~Node()
{
    vertices.clear();
    points.clear();
    vbo.clear();
}

SceneBodyGlobe::Node& SceneBodyGlobe::Node::operator = (const Node& rhs)
{
    return *this = rhs;
}

void SceneBodyGlobe::Node::update()
{
    ofVec3f p{dir * scale};
    p = p * getOrientationQuat();
    
    points.push_back(p);
    
    while (points.size() > kMaxPoints) {
        points.pop_front();
    }
    
    for (int i=0; i<points.size(); i++) {
        vertices.at(i) = points.at(i);
    }
    
    vbo.updateVertexData(&vertices.at(0), vertices.size());
}

void SceneBodyGlobe::Node::customDraw()
{
    glBegin(GL_LINES);
    glColor4f(1.f, 1.f, 1.f, 0.6f);
    glVertex3fv(ofVec3f::zero().getPtr());
    glColor4f(1.f, 1.f, 1.f, 0.1f);
    glVertex3fv((dir * scale * 1.2f).getPtr());
    glEnd();
}

void SceneBodyGlobe::Node::drawPoints()
{
    glPointSize(3.f);
    ofSetColor(255, 40);
    vbo.draw(GL_POINTS, 0, vertices.size());
}

#pragma mark ___________________________________________________________________
SceneBodyGlobe::Globe::Globe() :
nodes(ofxMot::NUM_JOINTS)
{
    for (auto& p : nodes) {
        p = Node::Ptr(new Node());
    }
}

#pragma mark ___________________________________________________________________
void SceneBodyGlobe::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sphere Scale", 200.f, 600.f, &mScale);
    mUICanvas->addSlider("Rotation Speed X", 0.f, 10.f, &mRotSpdX);
    mUICanvas->addSlider("Rotation Speed Y", 0.f, 10.f, &mRotSpdY);
    mUICanvas->addToggle("Magnify", &mMagnify);
    
    mCam.disableMouseInput();
}

void SceneBodyGlobe::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyGlobe::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
    
    mGlobeMap.clear();
    
    mEnterTime = ofGetElapsedTimef();
}

void SceneBodyGlobe::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
    
    mGlobeMap.clear();
}

void SceneBodyGlobe::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrMotioner) {
        if (mMagnify) {
            mScale += ofGetLastFrameTime() * 2.f;
            if (mScale >= 600.f) mScale = 100.f;
        }
        if (ofGetFrameNum()%60==0) {
            mJointId++;
            mJointId %= ofxMot::NUM_JOINTS;
        }
        
        const float t{ofGetElapsedTimef() - mEnterTime};
        mNumJoints = t / mJointIncrementSpan;
        mNumJoints = ofClamp(mNumJoints, 0, ofxMot::NUM_JOINTS);
    }
}

void SceneBodyGlobe::draw()
{
    mCam.begin();
    ofPushMatrix();
    ofRotateX(ofGetElapsedTimef() * mRotSpdX);
    ofRotateY(ofGetElapsedTimef() * mRotSpdY);
    ofSetLineWidth(1.5f);
    for (auto& it : mGlobeMap) {
        ofPushMatrix();
        ofTranslate(it.second->origin);
        int i{0};
        for (auto p : it.second->nodes) {
            if (i > mNumJoints) break;
            ofEnableAlphaBlending();
            p->drawPoints();
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            p->draw();
            if (ofGetFrameNum()%120 > 0 && ofGetFrameNum()%120 < 30) {
                ofSetColor(255, 128);
                ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
                ofPushMatrix();
                ofTranslate(p->dir * p->scale * 1.f);
                ofDrawBitmapString(ofxMot::getJointNameLower(mJointId),
                                   ofPoint::zero());
                ofPopMatrix();
            }
            i++;
        }
        ofPopMatrix();
    }
    ofPopMatrix();
    mCam.end();
}

#pragma mark ___________________________________________________________________
void SceneBodyGlobe::setupSkeleton(SkeletonPtr skl)
{
    auto globe = Globe::Ptr(new Globe());
    globe->origin = ofVec3f::zero();
    mGlobeMap[skl->getName()] = globe;
}

void SceneBodyGlobe::updateSkeleton(SkeletonPtr skl)
{
    auto& joints = skl->getJoints();
    auto it = mGlobeMap.find(skl->getName());
    if (it != mGlobeMap.end()) {
        auto& vec = it->second->nodes;
        for (int i=0; i<vec.size(); i++) {
            if (i > mNumJoints) break;
            auto p = vec.at(i);
            p->dir = joints.at(i).getPosition();
            p->dir.normalize();
            p->scale = mScale;
            p->setOrientation(joints.at(i).getOrientationQuat());
            p->update();
        }
    }
    else {
        setupSkeleton(skl);
    }
}

void SceneBodyGlobe::exitSkeleton(SkeletonPtr skl)
{
    auto it = mGlobeMap.find(skl->getName());
    if (it != mGlobeMap.end()) {
        mGlobeMap.erase(it);
    }
}

DP_SCORE_NAMESPACE_END