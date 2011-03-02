/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2009-2011                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef ASSOCIATIONWIDGET_H
#define ASSOCIATIONWIDGET_H

#include "linkwidget.h"
#include "messagewidgetlist.h"
#include "umlobject.h"
#include "umlwidgetlist.h"
#include "widgetbase.h"

class AssociationLine;
class AssociationWidget;
class ClassifierWidget;
class FloatingTextWidget;
class UMLAssociation;
class UMLAttribute;
class UMLClassifierListItem;
class UMLWidget;

struct WidgetRole
{
    FloatingTextWidget *multiplicityWidget;
    FloatingTextWidget *changeabilityWidget;
    FloatingTextWidget *roleWidget;

    UMLWidget *umlWidget;

    // The following are used only in case of absence of UMLObject
    Uml::Visibility         visibility;
    Uml::Changeability_Type changeability;
    QString                 roleDocumentation;

    WidgetRole();
    ~WidgetRole();

    void initFloatingWidgets(Uml::Role_Type role, AssociationWidget *parent);
};

class AssociationWidget : public WidgetBase, public LinkWidget
{
    Q_OBJECT
    public:
        AssociationWidget();
        AssociationWidget(UMLWidget *widgetA, Uml::AssociationType type,
                UMLWidget *widgetB, UMLObject *obj = 0);
        virtual ~AssociationWidget();

        virtual void setUMLObject(UMLObject *obj, bool notifyAsSlot = false);

        //---------- LinkWidget Interface methods implemementation from now on.

        virtual void lwSetFont (QFont font);
        virtual UMLClassifier *operationOwner();

        virtual UMLOperation *operation();
        virtual void setOperation(UMLOperation *op);

        virtual QString customOpText();
        virtual void setCustomOpText(const QString &opText);

        virtual void resetTextPositions();

        virtual void setMessageText(FloatingTextWidget *ft);
        virtual void setText(FloatingTextWidget *ft, const QString &newText);

        virtual void showPropertiesDialog();

        virtual UMLClassifier* seqNumAndOp(QString& seqNum, QString& op);
        virtual void setSeqNumAndOp(const QString &seqNum, const QString &op);

        virtual void constrainTextPos(qreal &textX, qreal &textY, qreal textWidth, qreal textHeight,
                Uml::TextRole tr);

        virtual void calculateNameTextSegment();

        //---------- End LinkWidget Interface methods implemementation.

        UMLAssociation* association() const;
        UMLAttribute* attribute() const;

        bool isEqual(AssociationWidget *other) const;

        FloatingTextWidget* multiplicityWidget(Uml::Role_Type role) const;
        QString multiplicity(Uml::Role_Type role) const;
        void setMultiplicity(const QString& text, Uml::Role_Type role);

        Uml::Visibility visibility(Uml::Role_Type role) const;
        void setVisibility(Uml::Visibility v, Uml::Role_Type role);

        FloatingTextWidget* changeabilityWidget(Uml::Role_Type role) const;
        Uml::Changeability_Type changeability(Uml::Role_Type role) const;
        void setChangeability(Uml::Changeability_Type c, Uml::Role_Type role);

        FloatingTextWidget* nameWidget() const;

        FloatingTextWidget* roleWidget(Uml::Role_Type role) const;
        QString roleName(Uml::Role_Type role) const;
        void setRoleName (const QString &strRole, Uml::Role_Type role);

        QString roleDocumentation(Uml::Role_Type role) const;
        void setRoleDocumentation(const QString& doc, Uml::Role_Type role);

        bool associates(UMLWidget *a, UMLWidget *b) const;
        bool containsWidget(UMLWidget *widget) const;

        FloatingTextWidget* textWidgetByRole(Uml::TextRole tr) const;

        Uml::Role_Type roleForWidget(UMLWidget *widget) const;
        bool isSelf() const;

        ClassifierWidget* associationClass() const;
        void setAssociationClass(ClassifierWidget *classifier);

        UMLWidget* widgetForRole(Uml::Role_Type role) const;
        void setWidgetForRole(UMLWidget *widget, Uml::Role_Type role);
        void setWidgets(UMLWidget *widgetA, UMLWidget *widgetB);

        Uml::IDType widgetIDForRole(Uml::Role_Type role) const;

        Uml::AssociationType associationType() const;
        void setAssociationType(Uml::AssociationType type);

        bool isCollaboration() const;

        QString toString() const;

        AssociationLine* associationLine() const;

        virtual bool activate();
        virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

        bool loadFromXMI(QDomElement& element, const UMLWidgetList &list,
                         const MessageWidgetList* messages);
        virtual bool loadFromXMI(QDomElement& element);
        virtual void saveToXMI(QDomDocument &qDoc, QDomElement &qElement);

        virtual void contextMenu(const QPointF& pos, Qt::KeyboardModifiers modifiers, const QPoint& screenPos);

    public Q_SLOTS:
        virtual void slotMenuSelection(QAction *trigger);

    protected Q_SLOTS:
        virtual void slotUMLObjectDataChanged();
        virtual void slotClassifierListItemRemoved(UMLClassifierListItem* classifierItem);

    protected:
        virtual void updateGeometry();
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
        virtual QVariant attributeChange(WidgetAttributeChange change, const QVariant& oldValue);

        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

        virtual void umlObjectChanged(UMLObject *old);

    private:
        void init();
        void setFloatingText(Uml::TextRole tr, const QString& text, FloatingTextWidget* ft);
        QPointF calculateTextPosition(Uml::TextRole tr);
        void setTextPosition(Uml::TextRole tr);
        void updateNameWidgetRole();

        friend class AssociationLine;

        AssociationLine *m_associationLine;
        ClassifierWidget *m_associationClass;
        WidgetRole m_widgetRole[2];
        FloatingTextWidget *m_nameWidget;
        int m_nameSegmentIndex;

        Uml::AssociationType m_associationType;
        bool m_slotUMLObjectDataChangedFirstCall;
};

#endif
