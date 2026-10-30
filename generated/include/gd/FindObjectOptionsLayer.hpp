#pragma once

class OptionsLayer;

class FindObjectOptionsLayer : public GJOptionsLayer {
public:
    virtual ~FindObjectOptionsLayer();
    virtual bool init();
    virtual void setupOptions();

    static FindObjectOptionsLayer* create();

};
