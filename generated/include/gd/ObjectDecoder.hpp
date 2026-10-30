#pragma once

class DS_Dictionary;
class ObjectDecoderDelegate;
class cocos2d;

class ObjectDecoder : public cocos2d::CCNode {
public:
    virtual bool init();

    static ObjectDecoder* sharedDecoder();

    cocos2d::CCObject* getDecodedObject(int objectType, DS_Dictionary* data);
    ObjectDecoderDelegate* getDelegate();
    void setDelegate(ObjectDecoderDelegate* delegate);

    ObjectDecoderDelegate* m_delegate;

};
