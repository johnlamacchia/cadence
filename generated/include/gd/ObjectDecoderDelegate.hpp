#pragma once

class DS_Dictionary;
class cocos2d;

class ObjectDecoderDelegate {
public:
    virtual cocos2d::CCObject* getDecodedObject(int objectType, DS_Dictionary* data);

};
