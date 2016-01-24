//
//  dpScoreNodeHakoniwa.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreNodeHakoniwa.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"

DP_SCORE_NAMESPACE_BEGIN

NodeHakoniwa::NodeHakoniwa()
{
    setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 50.f, 0.f, 400.f - NodeHakoniwa::getDepth());
    addAimingOffset(ofVec3f(NodeHakoniwa::getWidth() * 0.5f, Desk::getHeight(), NodeHakoniwa::getDepth() * 0.5f));
}

NodeHakoniwa::~NodeHakoniwa()
{

}

void NodeHakoniwa::customDraw()
{
    ScopedStyle s;
    setStyle(*this);
	for (auto i : rep(kNumX)) {
		for (auto j : rep(kNumY)) {
			ScopedTranslate t(i * mDesk.getDimension(), 0.f, j * (mDesk.getDimension() + 90.f));
			mDesk.draw();
            drawBox(ofVec3f(Desk::getDimension() - 5.f, Desk::getHeight(), Desk::getDimension() - 9.f), 5.f, 75.f, 9.f);
		}
	}
}

DP_SCORE_NAMESPACE_END

