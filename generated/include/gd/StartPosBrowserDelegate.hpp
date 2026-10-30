#pragma once

class StartPosBrowser;
class StartPosObject;

class StartPosBrowserDelegate {
public:
    virtual void startPosSelected(StartPosBrowser*, int, StartPosObject*);

};
