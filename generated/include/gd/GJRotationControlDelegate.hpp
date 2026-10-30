#pragma once

class GJRotationControlDelegate {
public:
    virtual void angleChanged(float, bool);
    virtual void angleChangeBegin();
    virtual void angleChangeEnded();

};
