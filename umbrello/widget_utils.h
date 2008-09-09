/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef WIDGET_UTILS_H
#define WIDGET_UTILS_H

#include <QtCore/QPoint>
#include <QtXml/QDomDocument>
#include <QtGui/QBrush>
#include <QtGui/QPen>
#include <QtGui/QFont>

#include "umlnamespace.h"
#include "umlwidgetlist.h"
#include "messagewidgetlist.h"

// forward declarations
class QGraphicsRectItem;

/**
 * General purpose widget utilities.
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
namespace Widget_Utils
{

    UMLWidget* findWidget(Uml::IDType id,
                          const UMLWidgetList& widgets,
                          const MessageWidgetList* pMessages = NULL);

    QGraphicsRectItem *decoratePoint(const QPointF& p);

    void drawCrossInEllipse(QPainter *p, const QRectF& ellipse);

    void drawTriangledRect(QPainter *painter, const QRectF& rect, const QSizeF& triSize);

    void drawArrowHead(QPainter *painter, const QPointF& arrowPos,
                       const QSizeF& arrowSize, Qt::ArrowType arrowType,
                       bool solid = false);

    QString pointToString(const QPointF& point);

    QPointF stringToPoint(const QString& str);

    bool loadPixmapFromXMI(const QDomElement &qElement, QPixmap &pixmap);

    void savePixmapToXMI(QDomDocument &qDoc, QDomElement &qElement, const QPixmap& pixmap);

    bool loadGradientFromXMI(const QDomElement &qElement, QGradient *&gradient);

    void saveGradientToXMI(QDomDocument &qDoc, QDomElement &qElement, const QGradient *gradient);

    bool loadBrushFromXMI(const QDomElement &qElement, QBrush &brush);

    void saveBrushToXMI(QDomDocument &qDoc, QDomElement &qElement,
                        const QBrush& brush);

}

#endif
