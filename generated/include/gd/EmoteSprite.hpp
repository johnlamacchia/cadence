#pragma once

class EmoteDef;
class EmoteToken;
class cocos2d;

class EmoteSprite : public cocos2d::CCSprite {
public:
    virtual ~EmoteSprite();
    virtual void setColor(cocos2d::ccColor3B const&);

    static EmoteSprite* createWithDef(EmoteDef const*);
    static EmoteSprite* createWithName(std::string const&);
    static void attachToLabel(cocos2d::CCLabelBMFont*, cocos2d::CCNode*, std::vector<EmoteToken> const&, float);
    static EmoteDef* defForName(std::string const&);

    void /* TodoReturn */ characterCount(std::string const&);
    void /* TodoReturn */ charIndexForByte(std::string const&, int);
    void /* TodoReturn */ defAtIndex(int);
    void /* TodoReturn */ defCount();
    void /* TodoReturn */ effectiveLength(std::string const&, int);
    void /* TodoReturn */ fitToHeight(float, float);
    void /* TodoReturn */ heightFactor();
    bool initWithDef(EmoteDef const*);
    void /* TodoReturn */ parseTokens(std::string const&, std::vector<EmoteToken>*, int);
    void /* TodoReturn */ replaceTokens(std::string const&, std::vector<EmoteToken>&, float);
    void /* TodoReturn */ translateUnicodeEmoji(std::string const&);
    float jumboMaxAspect();
    float jumboMultiScale();
    float jumboScale();
    int maxEmotesComment();
    int maxEmotesMessage();

    EmoteDef* m_emoteDef;

};
