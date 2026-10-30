#pragma once

class SongsLayer : public GJDropDownLayer {
public:
    virtual ~SongsLayer();
    virtual void customSetup();

    static SongsLayer* create();

};
