#include "BounceScrollView.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define BOUNCE_BACK_FACTOR 0.35
#define MOVE_INCH            7.0f/160.0ff
#define MAX_DISTANCE 40.0f

static float convertDistanceFromPointToInch(float pointDis)
{
    auto glview = Director::getInstance()->getOpenGLView();
    float factor = ( glview->getScaleX() + glview->getScaleY() ) / 2;
    return pointDis * factor / Device::getDPI();
}

void BounceScrollView::setContainer(Node * pContainer)
{
    if (nullptr == pContainer)
        return;

    this->removeChildByTag(9, true);
    this->_container = pContainer;

    this->_container->setIgnoreAnchorPointForPosition(false);
    this->_container->setAnchorPoint(Vec2(0.0f, 0.0f));

    this->addChild(this->_container);
    this->_container->setTag(9);

    this->setViewSize(this->_viewSize);
}

bool BounceScrollView::init(){
    if(!ScrollView::init())
        return false;

    drawNode = DrawNode::create(5);
    this->addChild(drawNode,100);
    drawNode->setGlobalZOrder(999);

    this->setBounceable(false);
    return true;
}


void BounceScrollView::drawNodeDrawSolitQuadBezier(const Vec2 &origin, const Vec2 &control, const Vec2 &destination, unsigned int segments, const Color4F &color)
{
    Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
    if( ! vertices )
        return;

    float t = 0.0f;
    for(unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 2) * origin.x + 2.0f * (1 - t) * t * control.x + t * t * destination.x;
        vertices[i].y = powf(1 - t, 2) * origin.y + 2.0f * (1 - t) * t * control.y + t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;

    drawNode->drawSolidPoly(vertices, segments+1,  color);

    CC_SAFE_DELETE_ARRAY(vertices);
}


void BounceScrollView::drawBounceBezier(Vec2 dragPosition,Vec2 touchLocation)
{
    drawNode->clear();
    auto touchPosition = convertToNodeSpace(touchLocation);

    if(dragPosition.y < - MAX_DISTANCE) dragPosition.y = - MAX_DISTANCE;
    if(dragPosition.y > MAX_DISTANCE) dragPosition.y = MAX_DISTANCE;

    float v = std::abs(dragPosition.y) / MAX_DISTANCE;
    float opacity = v * 0.5;
    float c = (1.0f - v) * 0.5;
    Color4F color(c,c,c,opacity);

    Vec2 controlPos(touchPosition.x, dragPosition.y);
    drawNode->setVisible(!dragPosition.isZero());

    if(dragPosition.y > 0.0001){
        drawNodeDrawSolitQuadBezier(Vec2(-15,0), controlPos, Vec2(_viewSize.width+15,0), 10, color);
    }else if(dragPosition.y < -0.0001){
        controlPos.y = _viewSize.height + controlPos.y;
        drawNodeDrawSolitQuadBezier(Vec2(-15, _viewSize.height), controlPos, Vec2(_viewSize.width+15,_viewSize.height), 10, color);
    }
}


bool BounceScrollView::isForbiddenDrag()
{
    if(_direction == Direction::VERTICAL)
        return this->totalDistance.y * _scrollDistance.y >= 0.0f;
    else
        return this->totalDistance.x * _scrollDistance.x >= 0.0f;

    return false;
}


void BounceScrollView::onTouchMoved(Touch* touch, Event* event)
{
    Vec2 preOffset = _container->getPosition();
    ScrollView::onTouchMoved(touch, event);


    if(isForbiddenDrag()) {
        Vec2 step = (MAX_DISTANCE - std::abs(this->totalDistance.y)) / MAX_DISTANCE * _scrollDistance;
        this->totalDistance += step;
    } else
        this->totalDistance = Vec2::ZERO;

    Vec2 currentOffset = preOffset + this->totalDistance;

    const Vec2 minOffset = this->minContainerOffset();
    const Vec2 maxOffset = this->maxContainerOffset();

    Vec2 limitedOffset;
    limitedOffset.x = MAX(minOffset.x, MIN(maxOffset.x, currentOffset.x));
    limitedOffset.y = MAX(minOffset.y, MIN(maxOffset.y, currentOffset.y));

    drawBounceBezier(currentOffset - limitedOffset, touch->getLocation());
}

void BounceScrollView::onTouchEnded(Touch* touch, Event* event)
{
    ScrollView::onTouchEnded(touch, event);
    this->drawNode->setVisible(false);
    this->totalDistance = Vec2::ZERO;
}
