#pragma once

class OptionsObject;

class OptionsObjectDelegate {
public:
    virtual void stateChanged(OptionsObject* object);

};
