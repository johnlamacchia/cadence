#pragma once

class GJSpiderSprite : public GJRobotSprite {
public:
    virtual ~GJSpiderSprite();

    static GJSpiderSprite* create(int frame);

    bool init(int frame);

};
