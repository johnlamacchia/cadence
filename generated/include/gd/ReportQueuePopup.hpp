#pragma once

class Dropdown;
class cocos2d;

class ReportQueuePopup : public PagedListPopup, public CommentAdminDelegate, public DropdownDelegate {
public:
    virtual ~ReportQueuePopup();
    virtual bool init();
    virtual void rebuildContent();
    virtual int requestPage(int);
    virtual void dropdownSelectionChanged(Dropdown*, int);
    virtual void adminReportQueueFinished(cocos2d::CCArray*, int, int, int);
    virtual void adminReportQueueFailed();

    static ReportQueuePopup* create();

    void /* TodoReturn */ buildRows(cocos2d::CCArray*);
    void onOpenAuthor(cocos2d::CCObject* sender);
    void onOpenReview(cocos2d::CCObject* sender);
    void onRefresh(cocos2d::CCObject* sender);
    void onReportedContentChanged(cocos2d::CCObject* sender);
    void onToggleSort(cocos2d::CCObject* sender);

};
