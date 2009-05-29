/***************************************************************************
 * Copyright (C) 2009 by Gopala Krishna A <krishna.ggk@gmail.com>          *
 *                                                                         *
 * This is free software; you can redistribute it and/or modify            *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2, or (at your option)     *
 * any later version.                                                      *
 *                                                                         *
 * This software is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this package; see the file COPYING.  If not, write to        *
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,   *
 * Boston, MA 02110-1301, USA.                                             *
 ***************************************************************************/

#include "associationspacemanager.h"
#include "newassociationwidget.h"
#include "newlinepath.h"
#include "umlwidget.h"

/**
 * Constructs a new space manager object for given widget.
 */
AssociationSpaceManager::AssociationSpaceManager(UMLWidget *widget)
{
    m_umlWidget = widget;
    Q_ASSERT(widget);
}

/**
 * This method is used to register the AssociationWidget associatied with this UMLWidget along
 * specified region passed.
 *
 * @param  assoc The AssociationWidget to be registered.
 * @param region The region with which the AssociationWidget has to be registered. If region =
 *               Uml::Error, then region is computed using @ref nearestRegion() function.
 *
 * @note region should neither be Uml::Error nor Uml::Center.
 */
Uml::Region AssociationSpaceManager::add(New::AssociationWidget *assoc,
        Uml::Region region)
{
    Q_ASSERT(region != Uml::Center);

    if (registered(assoc)) {
        uDebug() << assoc->name() << " is already registered!";
        return Uml::Error;
    }

    if (region == Uml::Error) {
        region = nearestRegion(assoc);
        Q_ASSERT(region != Uml::Error);
    }

    QList<New::AssociationWidget*> &listRef = m_regionAssociationsMap[region];
    listRef << assoc;
    m_registeredAssociationSet << assoc;

    return region;
}

/**
 * This method unregisters the AssociationWidget by removing it from region specific list.
 * @note The AssociationWidget is however @b not deleted.
 */
void AssociationSpaceManager::remove(New::AssociationWidget *assoc)
{
    if (!registered(assoc)) {
        uDebug() << assoc->name() << " is not registered!";
        return;
    }

    Uml::Region reg = region(assoc);
    Q_ASSERT(reg != Uml::Error);
    Q_ASSERT(reg != Uml::Center);

    QList<New::AssociationWidget*> &listRef = m_regionAssociationsMap[reg];
    listRef.removeOne(assoc);
    m_registeredAssociationSet.remove(assoc);
}

/**
 * This method returns the appropriate end point location in scene coordinates based on whether the
 * association is pointing to widget or pointing away from it (that is whether it is Uml::A or
 * Uml::B respectively).
 *
 * @param assoc The AssociationWidget for which end point should be returned.
 * @return The end point of assoc in scene coordinates which is associated to m_umlWidget.
 *
 * @see AssociationSpaceManager::penultimateEndPoint()
 */
QPointF AssociationSpaceManager::endPoint(New::AssociationWidget *assoc) const
{
    UMLWidget *widA = assoc->widgetForRole(Uml::A);
    UMLWidget *widB = assoc->widgetForRole(Uml::B);
    QPointF retVal;

    if (widA == m_umlWidget) {
        retVal = assoc->associationLine()->point(0);
    }
    else if (widB == m_umlWidget) {
        New::AssociationLine *line = assoc->associationLine();
        retVal = line->point(line->count() - 1);
    }
    else {
        uWarning() << "Passed association " << assoc->name() << " is not related to this AssociationSpaceManager";
    }
    retVal = assoc->mapToScene(retVal);
    return retVal;
}

/**
 * This method returns the appropriate penultimate end point location in scene coordinates based on
 * whether the association is pointing to widget or pointing away from it (that is whether it is
 * Uml::A or Uml::B respectively).
 *
 * @param assoc The AssociationWidget for which penultimate end point should be returned.
 * @return The penultimate end point of assoc in scene coordinates which is associated to
 *         m_umlWidget.
 */
QPointF AssociationSpaceManager::penultimateEndPoint(New::AssociationWidget *assoc) const
{
    UMLWidget *widA = assoc->widgetForRole(Uml::A);
    UMLWidget *widB = assoc->widgetForRole(Uml::B);
    QPointF retVal;

    if (widA == m_umlWidget) {
        retVal = assoc->associationLine()->point(1);
    }
    else if (widB == m_umlWidget) {
        New::AssociationLine *line = assoc->associationLine();
        retVal = line->point(line->count() - 2);
    }
    else {
        uWarning() << "Passed association " << assoc->name() << " is not related to this AssociationSpaceManager";
    }
    retVal = assoc->mapToScene(retVal);
    return retVal;
}

/**
 * This method calculates the region which is closest for the AssociationWidget based on distance
 * of penultimate point from m_umlWidget.
 * The distance calculation happens in scene coordinates.
 */
Uml::Region AssociationSpaceManager::nearestRegion(New::AssociationWidget *assoc) const
{
    QPointF penultimate = penultimateEndPoint(assoc);
    QRectF widRect = m_umlWidget->mapToScene(m_umlWidget->rect()).boundingRect();
    bool left = false, top = false;
    qreal horDist = 0., verDist = 0.;

    if (qAbs(widRect.left() - penultimate.x()) < qAbs(widRect.right() - penultimate.x())) {
        left = true;
        horDist = qAbs(widRect.left() - penultimate.x());
    }
    else {
        left = false;
        horDist = qAbs(widRect.right() - penultimate.x());
    }

    if (qAbs(widRect.top() - penultimate.y()) < qAbs(widRect.bottom() - penultimate.y())) {
        top = true;
        verDist = qAbs(widRect.top() - penultimate.y());
    }
    else {
        top = false;
        verDist = qAbs(widRect.bottom() - penultimate.y());
    }

    if (qFuzzyCompare(horDist, verDist)) {
        if (left) {
            return top ? Uml::NorthWest : Uml::SouthWest;
        }
        return top ? Uml::NorthEast : Uml::SouthEast;
    }
    else if (horDist < verDist) {
        return left ? Uml::West : Uml::East;
    }
    else {
        return top ? Uml::North : Uml::South;
    }
}


void sortAssociationPointList(QList<QPair<New::AssociationWidget*, QPointF> > &list,
        qreal other, bool xCoordBasis)
{
    for (int i = 0; i < list.size()-1; ++i) {
        qreal minVal = qAbs(other - (xCoordBasis ? list[i].second.x() : list[i].second.y()));
        int minInd = i;

        for (int j = i + 1; j < list.size(); ++j) {
            qreal dist = qAbs(other - (xCoordBasis ? list[j].second.x() : list[j].second.y()));
        }
    }
}
/**
 * This method arranges the AssociationWidget line end points for a given region based on its
 * distance of penultimate point from edge.
 */
void AssociationSpaceManager::arrange(Uml::Region region)
{
    //TODO: Yet to complete.
#if 0
    Q_ASSERT(region != Uml::Error);
    Q_ASSERT(region != Uml::Center);

    QList<New::AssociationWidget*> &listRef = m_regionAssociationsMap[region];
    QList<QPair<New::AssociationWidget*, QPointF> > assocPenuls;
    foreach (New::AssociationWidget* assoc, listRef) {
        QPointF p = penultimateEndPoint(assoc);
        assocPenuls.append(qMakePair(assoc, p));
    }

    QRectF rect = m_umlWidget->mapToScene(m_umlWidget->rect()).boundingRect();
    QPointF other;
    bool xBasis = false;
    switch (region) {
        case Uml::North:
        case Uml::NorthWest:
            other = rect.topLeft(); xBasis = true; break;

        case Uml::NorthEast:
            other = rect.topRight(); break;

        case Uml::East:
        case Uml::SouthEast:
            other = rect.bottomRight(); break;

        case Uml::South:
        case Uml::SouthWest:
            xBasis = true;
            // intended fall through
        case Uml::West:
            other = rect.bottomLeft(); break;
    }

    for (int i = 0; i < assocPenuls.size() - 1; ++i) {
        qreal minVal = xBasis ? assocPenuls[i].second.x() : assocPenuls[i].second.y();
        qreal minInd = i;
        for (int j = 0; j < assocPenuls.size(); ++j) {
            qreal val = xBasis ? assocPenuls[i].second.x() : assocPenuls[i].second.y();
            if (val < minVal) {
                minVal = val;
                minInd = j;
            }
        }

        if (minInd == i) continue;
        qSwap(assocPenuls[i], assocPenuls[minInd]);
        qSwap(listRef[i], listRef[minInd]);
    }
#endif
}

/**
 * This utility method arranges the AssociationWidget line end points for a all regions based on
 * its distance of penultimate point from edge.
 */
void AssociationSpaceManager::arrangeAllRegions()
{
    arrange(Uml::North);
    arrange(Uml::East);
    arrange(Uml::South);
    arrange(Uml::West);
    arrange(Uml::NorthEast);
    arrange(Uml::SouthEast);
    arrange(Uml::SouthWest);
    arrange(Uml::NorthWest);
}

/**
 * @return The Uml::Region where assoc's end point resides.
 */
Uml::Region AssociationSpaceManager::region(New::AssociationWidget *assoc) const
{
    if (!registered(assoc)) {
        return Uml::Error;
    }
    QMapIterator<Uml::Region, QList<New::AssociationWidget*> > it(m_regionAssociationsMap);
    while (it.hasNext()) {
        it.next();
        if (it.value().contains(assoc)) {
            return it.key();
        }
    }
    return Uml::Error;
}

/**
 * @return Registration status of assoc.
 */
bool AssociationSpaceManager::registered(New::AssociationWidget* assoc) const
{
    return m_registeredAssociationSet.contains(assoc);
}

