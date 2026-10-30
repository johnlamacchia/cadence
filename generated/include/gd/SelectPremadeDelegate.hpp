#pragma once

class SelectPremadeLayer;

class SelectPremadeDelegate {
public:
    virtual void selectPremadeClosed(SelectPremadeLayer* layer, int type);

};
