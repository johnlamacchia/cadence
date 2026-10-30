#pragma once

class CCIndexPath;
class TableView;
class TableViewCell;

class TableViewDataSource {
public:
    virtual int numberOfRowsInSection(unsigned int section, TableView* tableView);
    virtual unsigned int numberOfSectionsInTableView(TableView* tableView);
    virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView* tableView, TableViewCellEditingStyle style, CCIndexPath& indexPath);
    virtual TableViewCell* cellForRowAtIndexPath(CCIndexPath& indexPath, TableView* tableView);

};
