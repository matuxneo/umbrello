/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2007                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef CLASSIFIERLISTPAGE_H
#define CLASSIFIERLISTPAGE_H

//qt  includes
#include <qwidget.h>
#include <q3groupbox.h>
#include <q3listbox.h>
#include <q3textedit.h>
//kde includes
#include <karrowbutton.h>
#include <kdialogbuttonbox.h>

//app includes
#include "../listpopupmenu.h"
#include "../umlclassifierlistitemlist.h"

class UMLObject;
class UMLClassifier;
class UMLDoc;


/**
 * A dialog page to display classifier list properties.  This is not normally setup
 * by you.  It is used by the @ref ClassPropDlg and new class wizard.
 *
 * @short A dialog page to display classifier properties.
 * @author Paul Hensgen, Jonathan Riddell
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class ClassifierListPage : public QWidget {
    Q_OBJECT
public:
    /**
     *  Sets up the ClassifierListPage
     *
     *  @param parent   The parent to the ClassAttPage.
     *  @param classifier       The Concept to display the properties of.
     *      @param doc The UMLDoc document
     *      @param type The type of listItem this handles
     */
    ClassifierListPage(QWidget* parent, UMLClassifier* classifier, UMLDoc* doc, Uml::Object_Type type);

    /**
     *  Standard deconstructor.
     */
    ~ClassifierListPage();

    /**
     *  Will move information from the dialog into the object.
     *  Call when the ok or apply button is pressed.
     */
    void updateObject();


private:

    /**
     * Sets up the page
     */
    void setupPage();

    /**
     *  Set the state of the widgets on the page with the given value.
     *
     *  @param  state   The state to set the widgets as.
     */
    void enableWidgets(bool state);

    /**
     * Saves the documentation for the currently selected item
     */
    void saveCurrentItemDocumentation();

    /**
     * Get classifier list items
     */
    UMLClassifierListItemList getItemList();

    /**
     * Attempts to add classifier to the appropriate list
     *
     * @param classifier        Pointer to the classifier to add.
     * @param position  Index at which to insert into the list.
     * @return true if the classifier could be added
     *
     */
    bool addClassifier(UMLClassifierListItem* classifier, int position = -1);

    /**
     * Take a classifier's subordinate item.
     * Ownership of the classifier list item is transferred to the caller.
     * @param listitem        UMLClassifierListItem to take.
     * @param seekPeerBefore  True if a peer index should be sought which
     *                        is smaller than the current listitem's index.
     * @param peerIndex       Return value: Index in the UMLClassifier's
     *                        item list at which a peer item, i.e. another
     *                        UMLClassifierListItem of the same type as
     *                        listItem, is found.  If no such item exists
     *                        then return -1.
     * @return   True for success.
     */
    bool takeItem(UMLClassifierListItem* listitem,
                  bool seekPeerBefore, int &peerIndex);

    /**
     * Utility for debugging, prints the current item list.
     * Only effective if VERBOSE_DEBUGGING is defined.
     */
    void printItemList(const QString &prologue);

    Q3GroupBox* m_pDocGB;
    Q3GroupBox* m_pItemListGB;
    Q3TextEdit* m_pDocTE;
    Q3ListBox* m_pItemListLB;

    KArrowButton* m_pTopArrowB;
    KArrowButton* m_pUpArrowB;
    KArrowButton* m_pDownArrowB;
    KArrowButton* m_pBottomArrowB;

    UMLDoc* m_pDoc;
    ListPopupMenu* m_pMenu;
    UMLClassifierListItem* m_pOldListItem;

protected:

    /** Loads the Item nList Box
     *
     */
    void reloadItemListBox();

    /** 
     * Sets the visibility of the arrow buttons
     * @param hide true hides the arrow buttons
     */
    void hideArrowButtons(bool hide);

    virtual int calculateNewIndex(UMLClassifierListItem* listItem);
    
    Uml::Object_Type m_itemType;
    UMLClassifier* m_pClassifier;
    UMLClassifierListItem* m_pLastObjectCreated;

    QPushButton* m_pNewClassifierListItemButton;
    QPushButton* m_pDeleteListItemButton;
    QPushButton* m_pPropertiesButton;

    bool m_bSigWaiting;

public slots:
    /**
     * called when list view is clicked on
     * calls enableWidgets()
     */
    void slotClicked(Q3ListBoxItem* item);

    /**
    * Called when an item is selected in a right click menu
    */
    void slotPopupMenuSel(int id);

    void slotListItemCreated(UMLObject* object);
    void slotListItemModified();
    void slotRightButtonClicked(Q3ListBoxItem* item, const QPoint& p);
    void slotRightButtonPressed(Q3ListBoxItem* item, const QPoint& p);

    /**
     * shows properties dialog for the attribute clicked on
     */
    void slotDoubleClick(Q3ListBoxItem* item);


    /**
     * moves selected attribute to the top of the list
     */
    void slotTopClicked();

    /**
     * moves selected attribute up in list
     */
    void slotUpClicked();

    /**
     * moved selected attribute down in list
     */
    void slotDownClicked();

    /**
     * moved selected attribute to the bottom of the list
     */
    void slotBottomClicked();

    /**
     * shows dialog for new attribute
     */
    void slotNewListItem();

    /**
     * removes currently seleted attribute
     */
    void slotDelete();

    /**
     * shows properties dialog for currently selected attribute
     */
    void slotProperties();
};

#endif
