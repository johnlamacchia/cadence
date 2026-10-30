#pragma once

class ColorAction;
class cocos2d;

class ColorChannelSprite : public cocos2d::CCSprite {
public:
    virtual ~ColorChannelSprite();
    virtual bool init();

    static ColorChannelSprite* create();

    void updateBlending(bool enabled);
    void updateCopyLabel(int channelID, bool copyColorLoop);
    void updateOpacity(float alpha);
    void updateValues(ColorAction* action);

    cocos2d::CCLabelBMFont* m_copyLabel;
    cocos2d::CCLabelBMFont* m_opacityLabel;
    cocos2d::CCLabelBMFont* m_blendingLabel;

};
