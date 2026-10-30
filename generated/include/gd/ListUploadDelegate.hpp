#pragma once

class GJLevelList;

class ListUploadDelegate {
public:
    virtual void listUploadFinished(GJLevelList* list);
    virtual void listUploadFailed(GJLevelList* list, int response);

};
