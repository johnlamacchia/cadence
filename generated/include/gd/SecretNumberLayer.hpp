#pragma once

class SecretNumberLayer : public cocos2d::CCLayer {
public:
    virtual ~SecretNumberLayer();
    virtual bool init();

    static SecretNumberLayer* create();

    void playNumberEffect(int secretNumber);

};
