#pragma once

class StatsLayer : public GJDropDownLayer {
public:
    virtual ~StatsLayer();
    virtual void customSetup();

    static StatsLayer* create();

    StatsLayer();

};
