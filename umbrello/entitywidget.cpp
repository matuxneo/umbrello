 /*
  *  copyright (C) 2003-2004
  *  Umbrello UML Modeller Authors <uml-devel@ uml.sf.net>
  */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "entitywidget.h"
#include "entity.h"
#include "entityattribute.h"
#include "classifier.h"
#include "umlclassifierlistitemlist.h"
#include "classifierlistitem.h"
#include "umlview.h"
#include "umldoc.h"
#include "uml.h"
#include "listpopupmenu.h"

#include <kdebug.h>
#include <qpainter.h>

EntityWidget::EntityWidget(UMLView* view, UMLObject* o): UMLWidget(view, o) {
	init();
	if ( !UMLApp::app()->getDocument()->loading() )
	{ // set default size - but only if we aren't loading a XMI file at the
		// moment - then just recreate the saved settings
		setSize(100,30);
		calculateSize();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void EntityWidget::init() {
	UMLWidget::setBaseType(Uml::wt_Entity);
	m_pMenu = 0;
	//set defaults from m_pView
	if (m_pView) {
		//check to see if correct
		const Settings::OptionState& ops = m_pView->getOptionState();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
EntityWidget::~EntityWidget() {}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void EntityWidget::draw(QPainter& p, int offsetX, int offsetY) {
	UMLWidget::draw(p, offsetX, offsetY);
	if(UMLWidget::getUseFillColour())
		p.setBrush(UMLWidget::getFillColour());
	else
		p.setBrush(m_pView -> viewport() -> backgroundColor());

	int w = width();
	int h = height();

	QFontMetrics &fm = getFontMetrics(FT_NORMAL);
	int fontHeight  = fm.lineSpacing();
	QString name = this->getName();

	p.drawRect(offsetX, offsetY, w, h);
	p.setPen(QPen(black));

	QFont font = UMLWidget::getFont();
	font.setBold(true);
	p.setFont(font);
	int y = 0;
	if ( !m_pObject->getStereotype().isEmpty() ) {
		p.drawText(offsetX + ENTITY_MARGIN, offsetY,
		   w - ENTITY_MARGIN * 2,fontHeight,
		   AlignCenter, m_pObject -> getStereotype());
		font.setItalic( m_pObject -> getAbstract() );
		p.setFont(font);
		p.drawText(offsetX + ENTITY_MARGIN, offsetY + fontHeight,
			   w - ENTITY_MARGIN * 2, fontHeight, AlignCenter, name);
		font.setBold(false);
		font.setItalic(false);
		p.setFont(font);
		y = fontHeight * 2;
	} else {
		font.setItalic( m_pObject -> getAbstract() );
		p.setFont(font);
		p.drawText(offsetX + ENTITY_MARGIN, offsetY,
			   w - ENTITY_MARGIN * 2, fontHeight, AlignCenter, name);
		font.setBold(false);
		font.setItalic(false);
		p.setFont(font);

		y = fontHeight;
	}

	UMLWidget::draw(p, offsetX, offsetY);

	p.drawLine(offsetX, offsetY + y, offsetX + w - 1, offsetY + y);

	QFontMetrics fontMetrics(font);
	UMLClassifier *classifier = (UMLClassifier*)m_pObject;
	UMLClassifierListItem* entityattribute = 0;
	UMLClassifierListItemList list = classifier->getFilteredList(Uml::ot_EntityAttribute);
	for (entityattribute = list.first(); entityattribute; entityattribute = list.next()) {
		QString text = entityattribute->getName();
		p.setPen( QPen(black) );
		p.drawText(offsetX + ENTITY_MARGIN, offsetY + y,
			   fontMetrics.width(text), fontHeight, AlignVCenter, text);
		y+=fontHeight;
	}

	if (m_bSelected) {
		drawSelected(&p, offsetX, offsetY);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void EntityWidget::calculateSize() {
	if (!m_pObject)  {
		return;
	}

	int width, height;
	QFont font = UMLWidget::getFont();
	font.setItalic(false);
	font.setUnderline(false);
	font.setBold(false);
	QFontMetrics fm(font);

	int fontHeight = fm.lineSpacing();

	int lines = 1;//always have one line - for name
	if ( !m_pObject->getStereotype().isEmpty() ) {
		lines++;
	}

	int numberOfEntityAttributes = ((UMLEntity*)m_pObject)->entityAttributes();

	height = width = 0;
	//set the height of the entity

	lines += numberOfEntityAttributes;
	if (numberOfEntityAttributes == 0) {
		height += fontHeight / 2; //no entity literals, so just add a bit of space
	}

	height += lines * fontHeight;

	//now set the width of the concept
	//set width to name to start with
	// FIXME spaces to get round beastie with font width,
	// investigate UMLWidget::getFontMetrics()
	width = getFontMetrics(FT_BOLD_ITALIC).boundingRect(" " + getName() + " ").width();

	int w = getFontMetrics(FT_BOLD).boundingRect("�" + m_pObject->getStereotype() + "�").width();

	width = w > width?w:width;

	UMLClassifier* classifier = (UMLClassifier*)m_pObject;
	UMLClassifierListItemList list = classifier->getFilteredList(Uml::ot_EntityAttribute);
	UMLClassifierListItem* listItem = 0;
	for (listItem = list.first(); listItem; listItem = list.next()) {
		int w = fm.width( listItem->getName() );
		width = w > width?w:width;
	}

	//allow for width margin
	width += ENTITY_MARGIN * 2;

	setSize(width, height);
	adjustAssocs( getX(), getY() );//adjust assoc lines
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void EntityWidget::slotMenuSelection(int sel) {
	switch(sel) {
		case ListPopupMenu::mt_EntityAttribute:
			if ( UMLApp::app()->getDocument()->createChildObject(m_pObject, Uml::ot_EntityAttribute) )  {
				UMLApp::app()->getDocument()->setModified();
			}
			break;
	}
	UMLWidget::slotMenuSelection(sel);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool EntityWidget::activate(IDChangeLog* ChangeLog /* = 0 */) {
	bool status = UMLWidget::activate(ChangeLog);
	if (status) {
		calculateSize();
	}
	return status;
}

void EntityWidget::saveToXMI( QDomDocument& qDoc, QDomElement& qElement ) {
	QDomElement conceptElement = qDoc.createElement("entitywidget");
	UMLWidget::saveToXMI(qDoc, conceptElement);
	qElement.appendChild(conceptElement);
}

bool EntityWidget::loadFromXMI( QDomElement & qElement ) {
	if ( !UMLWidget::loadFromXMI(qElement) ) {
		return false;
	}
	return true;
}

