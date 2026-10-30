#pragma once

class DS_Dictionary;
class ObjectDecoder;
class cocos2d;

class GJObjectDecoder : public cocos2d::CCNode, public ObjectDecoderDelegate {
public:
    virtual ~GJObjectDecoder();
    virtual bool init();
    virtual cocos2d::CCObject* getDecodedObject(int type, DS_Dictionary* dict);

    static GJObjectDecoder* sharedDecoder();

};
