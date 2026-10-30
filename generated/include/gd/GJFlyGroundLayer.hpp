#pragma once

class GJFlyGroundLayer : public GJGroundLayer {
public:
    virtual bool init();

    static GJFlyGroundLayer* create();

};
