#ifndef __BOUNCESCROLLVIEW_H__
#define __BOUNCESCROLLVIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"

class BounceScrollView: public cocos2d::extension::ScrollView {
public:
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
    void setContainer(Node * pContainer);

    CREATE_FUNC(BounceScrollView);
private:
    void drawNodeDrawSolitQuadBezier(const cocos2d::Vec2 &origin, const cocos2d::Vec2 &control,
            const cocos2d::Vec2 &destination, unsigned int segments, const cocos2d::Color4F &color);

    void drawBounceBezier(cocos2d::Vec2 dragPosition,cocos2d::Vec2 touchLocation);
    bool isForbiddenDrag();
    virtual bool init() override;

    cocos2d::Vec2 totalDistance;
    cocos2d::DrawNode* drawNode;
};

#endif


