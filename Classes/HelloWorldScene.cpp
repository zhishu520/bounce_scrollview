#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "BounceScrollView.h"
#include <string>

using namespace std;
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto container = Layer::create();

    Size spriteSize;
    for(int i = 0; i < 2; i++){
        auto png = StringUtils::format("res/%d.png",i+1);
        auto sprite = Sprite::create(png);
        if(i == 0) spriteSize = sprite->getContentSize();

        container->addChild(sprite);
        sprite->setPosition(spriteSize.width/2,  spriteSize.height*i + spriteSize.height*0.5f);
    }
    container->setContentSize(Size(spriteSize.width, spriteSize.height * 2));


    auto bounceScrollView = BounceScrollView::create();
    auto viewSize = Size(spriteSize.width,200);
    bounceScrollView->setViewSize(viewSize);
    bounceScrollView->setContainer(container);
    bounceScrollView->setDirection(BounceScrollView::Direction::VERTICAL);
    this->addChild(bounceScrollView);

    auto winSize = Director::getInstance()->getWinSize();
    bounceScrollView->setPosition((winSize - viewSize)/2);

    // bounceScrollView background
    auto layerColor = LayerColor::create(Color4B::WHITE, viewSize.width, viewSize.height);
    layerColor->setPosition(bounceScrollView->getPosition());
    this->addChild(layerColor, -1);

    return true;
}



