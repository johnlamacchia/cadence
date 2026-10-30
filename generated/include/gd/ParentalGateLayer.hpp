#pragma once

class cocos2d;

class ParentalGateLayer : public NumberInputLayer {
public:
    virtual ~ParentalGateLayer();
    virtual bool init();
    virtual void onDone(cocos2d::CCObject* sender);

    static ParentalGateLayer* create();

    ParentalGateLayer();
    void generateChallenge();

    int m_solution;

};
