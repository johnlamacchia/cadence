#pragma once

class GJUserScore;

class FriendPickerDelegate {
public:
    virtual void friendPicked(GJUserScore*);
    virtual bool isRecipientSelected(int);
    virtual void /* TodoReturn */ selectedRecipientCount();
    virtual void /* TodoReturn */ selectedRecipients();
    virtual void clearRecipients();
    virtual void friendPickerClosed();
    virtual void /* TodoReturn */ pickerRowState(int);
    virtual void /* TodoReturn */ pickerCountBase();
    virtual void friendPickerAdd();

};
