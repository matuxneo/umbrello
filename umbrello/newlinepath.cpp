/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "newlinepath.h"

#include "associationspacemanager.h"
#include "newassociationwidget.h"
#include "umlwidget.h"

// qt includes
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

namespace New
{
    /**
     * @class HeadSymbol
     *
     * This class provides with various symbols that can be used as
     * "head" at any end of the AssociationLine.  It also provides with
     * convenience methods to align the head to AssociationLine.
     */

    HeadSymbol::SymbolProperty HeadSymbol::symbolTable[Count] =
    {
        {
            QRectF(-6, 0, 12, 10), QPainterPath(), QLineF(0, 0, 0, 10),
            PointPair(QPointF(0, 10), QPointF(0, 10))
        },
        {
            QRectF(-5, -10, 10, 20), QPainterPath(), QLineF(0, -10, 0, 10),
            PointPair(QPointF(0, -10), QPointF(0, 10))
        },
        {
            QRectF(-8, -8, 16, 16), QPainterPath(), QLineF(0, -8, 0, 8),
            PointPair(QPointF(0, -8), QPointF(0, 8))
        }

    };

    /// @internal A convenience method to setup shapes of all symbols.
    void setupSymbolTable()
    {
        HeadSymbol::SymbolProperty &arrow = HeadSymbol::symbolTable[HeadSymbol::Arrow];
        if (arrow.shape.isEmpty()) {
            QRectF rect = arrow.boundRect;
            // Defines a 'V' shape arrow fitting in the bound rect.
            arrow.shape.moveTo(rect.topLeft());
            arrow.shape.lineTo(rect.center().x(), rect.bottom());
            arrow.shape.lineTo(rect.topRight());
        }

        HeadSymbol::SymbolProperty &diamond = HeadSymbol::symbolTable[HeadSymbol::Diamond];
        if (diamond.shape.isEmpty()) {
            QRectF rect = diamond.boundRect;
            // Defines a 'diamond' shape fitting in the bound rect.
            diamond.shape.moveTo(rect.center().x(), rect.top());
            diamond.shape.lineTo(rect.left(), rect.center().y());
            diamond.shape.lineTo(rect.center().x(), rect.bottom());
            diamond.shape.lineTo(rect.right(), rect.center().y());
            diamond.shape.lineTo(rect.center().x(), rect.top());
        }

        HeadSymbol::SymbolProperty &circle = HeadSymbol::symbolTable[HeadSymbol::Circle];
        if (circle.shape.isEmpty()) {
            QRectF rect = circle.boundRect;
            // Defines a circle with a horizontal-vertical cross lines.
            circle.shape.addEllipse(rect);

            circle.shape.moveTo(rect.center().x(), rect.top());
            circle.shape.lineTo(rect.center().x(), rect.bottom());

            circle.shape.moveTo(rect.left(), rect.center().y());
            circle.shape.lineTo(rect.right(), rect.center().y());
        }

    }


    /**
     * Constructs a HeadSymbol with current symbol being \a symbol and
     * parented to \a parent.
     */
    HeadSymbol::HeadSymbol(Symbol symbol, QGraphicsItem *parent) :
        QGraphicsItem(parent),
        m_firstTime(true)
    {
        // Ensure SymbolTable is validly initialized.
        setupSymbolTable();
        setSymbol(symbol);
    }

    /// Destructor
    HeadSymbol::~HeadSymbol()
    {
    }

    /// @return The current symbol being represented.
    HeadSymbol::Symbol HeadSymbol::symbol() const
    {
        return m_symbol;
    }

    /// Sets the current symbol type to \a symbol and updates the geometry.
    void HeadSymbol::setSymbol(Symbol symbol)
    {
        if (m_firstTime) {
            m_firstTime = false;
            // dont crash during this func call in constructor.
        }
        else {
            prepareGeometryChange(); //calls update implicitly
        }
        m_symbol = symbol;
    }

    /// Draws the current symbol using the QPainterPath stored for the current symbol.
    void HeadSymbol::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *)
    {
        painter->setPen(m_pen);
        painter->setBrush(m_brush);
        painter->drawPath(HeadSymbol::symbolTable[m_symbol].shape);
    }

    /// @return The bound rectangle for this based on current symbol.
    QRectF HeadSymbol::boundingRect() const
    {
        const qreal adj = .5 * m_pen.widthF();
        return HeadSymbol::symbolTable[m_symbol].boundRect.
            adjusted(-adj, -adj, adj, adj);
    }

    /// @return The path for this based on current symbol.
    QPainterPath HeadSymbol::shape() const
    {
        QPainterPath path;
        path.addRect(boundingRect());
        return path;
    }

    /**
     * This method aligns *this* HeadSymbol to the line being
     * passed. That is, it ensures that the axis of this symbol aligns
     * exactly with the \a "to" line passed.
     *
     * Also this item is moved such that the second end point of the
     * PointPair for the current symbol *collides* with the second end
     * point of \a "to" line.
     */
    void HeadSymbol::alignTo(const QLineF& to)
    {
        QLineF toMapped(mapFromParent(to.p1()), mapFromParent(to.p2()));

        QLineF origAxis = axisLine();
        QLineF translatedAxis = origAxis.translated(toMapped.p2() - origAxis.p2());

        qreal angle = translatedAxis.angleTo(toMapped);
        rotate(-angle);

        QPointF delta = to.p2() - mapToParent(symbolEndPoints().second);
        moveBy(delta.x(), delta.y());
    }

    /// @return The axis line for this item based on current symbol.
    QLineF HeadSymbol::axisLine() const
    {
        return HeadSymbol::symbolTable[m_symbol].axisLine;
    }

    /**
     * @return The end points for the symbol.
     *
     *         The first point is where the AssociationLine's visible line is
     *         supposed to end.
     *
     *         The second points is where the actual "head" part is to
     *         appear.
     */
    PointPair HeadSymbol::symbolEndPoints() const
    {
        return HeadSymbol::symbolTable[m_symbol].endPoints;
    }

    /// @return The pen used to draw symbol.
    QPen HeadSymbol::pen() const
    {
        return m_pen;
    }

    /// Sets the pen used to draw the symbol
    void HeadSymbol::setPen(const QPen& pen)
    {
        prepareGeometryChange();
        m_pen = pen;
    }

    /// @return The brush used to fill symbol.
    QBrush HeadSymbol::brush() const
    {
        return m_brush;
    }

    /// Sets the brush used to fill symbol.
    void HeadSymbol::setBrush(const QBrush &brush)
    {
        m_brush = brush;
        update();
    }


    /**
     * @class AssociationLine
     *
     * @short A class to manage a set of connected lines (eg Association line).
     *
     * This class inherits QGraphicsItem and hence can be directly
     * embedded in other QGraphicsItem. It provides interface methods
     * to insert, remove ..  Also it handles mouse events to allow
     * user to insert line points, move line points and also move
     * lines.
     *
     * @author Gopala Krishna
     * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
     */


    // Initialize static variables.
    const qreal AssociationLine::Delta = 5;
    const qreal AssociationLine::SelectedPointDiameter = 8;
    const qreal AssociationLine::SelfAssociationMinimumHeight = 30;


    /**
     * Constructs a AssociationLine item with its parent being \a parent.
     */
    AssociationLine::AssociationLine(New::AssociationWidget *assoc) : m_associationWidget(assoc)
    {
        m_activePointIndex = m_activeSegmentIndex = -1;
        m_startHeadSymbol = m_endHeadSymbol = 0;
        // This tracker is only for debugging and testing purpose.
        tracker = new QGraphicsLineItem;
        tracker->setPen(QPen(Qt::darkBlue, 1));
        tracker->setZValue(100);
        Q_ASSERT(assoc);
    }

    /// Destructor
    AssociationLine::~AssociationLine()
    {
        uDebug() << "Destroyed";
    }

    /// @return The point at given index.
    QPointF AssociationLine::point(int index) const
    {
        return m_points.at(index);
    }

    /**
     * Sets the point value at given index to \a point.  Also changes
     * the bounding rect appropriately.
     */
    void AssociationLine::setPoint(int index, const QPointF& point)
    {
        if (m_points.at(index) == point) {
            return; // Nothing to change
        }
        m_associationWidget->prepareGeometryChange();
        m_points[index] = point;
        calculateBoundingRect();
    }

    /// Shortcut for point(0)
    QPointF AssociationLine::startPoint() const
    {
        return m_points.at(0);
    }

    /// Shortcut for point(count()-1)
    QPointF AssociationLine::endPoint() const
    {
        return m_points.at(m_points.size()-1);
    }

    /**
     * Inserts the passed in \a point at the \a index passed in and
     * recalculates the bounding rect.
     */
    void AssociationLine::insertPoint(int index, const QPointF& point)
    {
        m_associationWidget->prepareGeometryChange();
        m_points.insert(index, point);
        calculateBoundingRect();
    }

    /**
     * Removes the point at \a index passed in.
     * @see removeNonEndPoint
     */
    void AssociationLine::removePoint(int index)
    {
        m_associationWidget->prepareGeometryChange();
        m_points.remove(index);
        calculateBoundingRect();
    }

    /**
     * @return The number of points in the linepath.
     */
    int AssociationLine::count() const
    {
        return m_points.size();
    }

    /**
     * Clears the linepath by removing all the points in the linepath.
     */
    void AssociationLine::clear()
    {
        if (!m_points.isEmpty()) {
            m_associationWidget->prepareGeometryChange();
            m_points.clear();
            calculateBoundingRect();
        }
    }

    /**
     * This method optimizes the number of points in the
     * AssociationLine. This can be used to reduce the clutter caused
     * due to too many points.
     */
    void AssociationLine::optimizeLinePoints()
    {
        int i = 1;
        m_associationWidget->prepareGeometryChange();
        while (i < m_points.size()) {
            if (m_points.at(i) == m_points.at(i-1)) {
                m_points.remove(i);
            }
            else {
                ++i;
            }
        }
        calculateBoundingRect();
        // TODO: Use delta comparison instead of exact comparison.
    }

    /**
     * @retval "Index" of the linepoint closest to the \a point
     *         passed.
     * @rertval -1 If no linepoint is closer to passed in \a point.
     *
     * @param point The point which is to be tested for closeness.
     * @param delta The closeness is measured by "delta" which indicates radius
     *              around the linepoint to be regarded as closer.
     */
    int AssociationLine::closestPointIndex(const QPointF& point, qreal delta) const
    {
        const int sz = m_points.size();
        for(int i = 0; i < sz; ++i) {
            const QPointF& linePoint = m_points.at(i);
            // Apply distance formula to see point closeness.
            qreal deltaXSquare = (point.x() - linePoint.x()) * (point.x() - linePoint.x());
            qreal deltaYSquare = (point.y() - linePoint.y()) * (point.y() - linePoint.y());

            qreal lhs = deltaXSquare + deltaYSquare;
            qreal rhs = delta * delta;

            if (lhs <= rhs) {
                return i;
            }
        }
        return -1;
    }

    /**
     * @retval "Index" of the segment on which the point lies (fuzzy)
     * @retval -1 If no segment contains the point
     *
     * @param point The point to be tested for line-segment closure.
     * @param delta Represents extent of fuzzy region around the line
     *              to be tested for point closure.
     */
    int AssociationLine::segmentIndex(const QPointF& point, qreal delta) const
    {
        QPainterPathStroker stroker;
        stroker.setWidth(delta);

        for(int i = 1; i < m_points.size(); ++i) {
            QLineF segment(m_points[i-1], m_points[i]);

            QPainterPath path;
            path.moveTo(segment.p1());
            path.lineTo(segment.p2());

            path = stroker.createStroke(path);

            if (path.contains(point)) {
                return i-1;
            }
        }

        return -1;
    }

    /**
     * retval True If point at \a index is start or end.
     */
    bool AssociationLine::isEndPointIndex(int index) const
    {
        const int sz = m_points.size();
        Q_ASSERT(index >= 0 && index < sz);

        return (index == 0 || index == (sz - 1));
    }

    /**
     * retval True If segment at \a index is start or end.
     */
    bool AssociationLine::isEndSegmentIndex(int index) const
    {
        // num of seg = num of points - 1
        const int sz = m_points.size() - 1;
        Q_ASSERT(index >= 0 && index < sz);

        return (index == 0 || index == (sz - 1));
    }

    /**
     * Sets the start and end points to passed parameters.
     */
    void AssociationLine::setEndPoints(const QPointF& start, const QPointF& end)
    {
        int size = m_points.size();

        m_associationWidget->prepareGeometryChange();

        if (size == 0) {
            m_points.insert(0, start);
            m_points.insert(1, end);
        }
        else if (size == 1) {
            m_points[0] = start;
            m_points.insert(1, end);
        }
        else {
            m_points[0] = start;
            m_points[size-1] = end;
        }

        calculateBoundingRect();
    }

    /**
     * Sets the HeadSymbol to appear at the first line segment to \a
     * symbol.
     *
     * If symbol == HeadSymbol::None , then it deletes the symbol item.
     *
     * Also this method aligns the head symbols.
     */
    void AssociationLine::setStartHeadSymbol(HeadSymbol::Symbol symbol)
    {
        Q_ASSERT(symbol != HeadSymbol::Count);
        if (symbol == HeadSymbol::None) {
            delete m_startHeadSymbol;
            m_startHeadSymbol = 0;
            return;
        }

        if (m_startHeadSymbol) {
            m_startHeadSymbol->setSymbol(symbol);
        }
        else {
            m_startHeadSymbol = new HeadSymbol(symbol, m_associationWidget);
        }
        m_startHeadSymbol->setPen(pen());
        alignHeadSymbols();
    }

    /**
     * Sets the HeadSymbol to appear at the last line segment to \a
     * symbol.
     *
     * If symbol == HeadSymbol::None , then it deletes the symbol item.
     *
     * Also this method aligns the head symbols.
     */
    void AssociationLine::setEndHeadSymbol(HeadSymbol::Symbol symbol)
    {
        Q_ASSERT(symbol != HeadSymbol::Count);
        if (symbol == HeadSymbol::None) {
            delete m_endHeadSymbol;
            m_endHeadSymbol = 0;
            return;
        }

        if (m_endHeadSymbol) {
            m_endHeadSymbol->setSymbol(symbol);
        }
        else {
            m_endHeadSymbol = new HeadSymbol(symbol, m_associationWidget);
        }
        m_endHeadSymbol->setPen(pen());
        alignHeadSymbols();
    }

    /**
     * This method aligns both the \b "start" and \b "end" symbols to
     * the current angles of the \b "first" and the \b "last" line
     * segment respectively.
     */
    void AssociationLine::alignHeadSymbols()
    {
        const int sz = m_points.size();
        if (sz < 2) {
            // Cannot align if there is no line (one line = 2 points)
            return;
        }

        if (m_startHeadSymbol) {
            QLineF segment(m_points[1], m_points[0]);
            m_startHeadSymbol->alignTo(segment);
        }

        if (m_endHeadSymbol) {
            QLineF segment(m_points[sz-2], m_points[sz - 1]);
            m_endHeadSymbol->alignTo(segment);
        }
    }


    /**
     * Loads AssociationLine information saved in \a qElement XMI element.
     */
    bool AssociationLine::loadFromXMI(QDomElement &qElement)
    {
        if (!qElement.hasChildNodes()) {
            return false;
        }

        QDomNodeList domNodes = qElement.childNodes();
        for (int i = 0; i < domNodes.size(); ++i) {
            QDomElement ele = domNodes.at(i).toElement();
            qreal x = ele.attribute("x", "0").toDouble();
            qreal y = ele.attribute("y", "0").toDouble();
            insertPoint(i, QPointF(x, y));
            ++i;
        }

        return true;
    }

    /**
     * Saves linepath information into XMI element named "linepath".
     */
    void AssociationLine::saveToXMI(QDomDocument &qDoc, QDomElement &qElement)
    {
        QDomElement pathElement = qDoc.createElement("linepath");
        for (int i = 0; i < m_points.size(); ++i ) {
            QDomElement pointElement = qDoc.createElement("point");
            const QPointF &point = m_points.at(i);
            pointElement.setAttribute( "x", point.x() );
            pointElement.setAttribute( "y", point.y() );
            pathElement.appendChild(pointElement);
        }
        qElement.appendChild(pathElement);
    }

    QPen AssociationLine::pen() const
    {
        QPen pen(m_associationWidget->lineColor(),
                 m_associationWidget->lineWidth(),
                 Qt::SolidLine,
                 Qt::RoundCap,
                 Qt::RoundJoin);
        return pen;
    }

    /**
     * Update the pen used to draw head symbol.
     */
    void AssociationLine::updatePenSettings()
    {
        m_associationWidget->prepareGeometryChange();
        QPen changedPen = pen();
        if (m_startHeadSymbol) {
            m_startHeadSymbol->setPen(changedPen);
        }
        if (m_endHeadSymbol) {
            m_endHeadSymbol->setPen(changedPen);
        }
        calculateBoundingRect();
    }

    /// @return The bounding rectangle for the linepath.
    QRectF AssociationLine::boundingRect() const
    {
        return m_boundingRect;
    }

    /// @return The shape of the linepath.
    QPainterPath AssociationLine::shape() const
    {
        return m_shape;
    }

    /// Draws the line path and also takes care of highlighting active point or line.
    void AssociationLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *opt)
    {
        QPen _pen = pen();
        const QColor orig = _pen.color().lighter();
        QColor invertedColor(orig.green(), orig.blue(), orig.red());
        if (invertedColor == _pen.color()) {
            // Ensure different color.
            invertedColor.setRed((invertedColor.red() + 50) % 256);
        }
        invertedColor.setAlpha(150);

        int sz = m_points.size();
        if (sz == 0) {
            return;
        }

        QPointF savedStart = m_points.first();
        QPointF savedEnd = m_points.last();

        // Modify the m_points array not to include the HeadSymbol, the value depends on HeadSymbol.
        if (m_startHeadSymbol) {
            QPointF newStart = m_startHeadSymbol->mapToParent(m_startHeadSymbol->symbolEndPoints().first);
            m_points[0] = newStart;
        }

        if (m_endHeadSymbol) {
            QPointF newEnd = m_endHeadSymbol->mapToParent(m_endHeadSymbol->symbolEndPoints().first);
            m_points[sz - 1] = newEnd;
        }

        painter->setPen(_pen);
        painter->setBrush(Qt::NoBrush);

        painter->drawPolyline(m_points.constData(), m_points.size());

        if (opt->state & QStyle::State_Selected) {
            QRectF ellipse(0, 0, SelectedPointDiameter, SelectedPointDiameter);
            painter->setBrush(_pen.color());
            painter->setPen(Qt::NoPen);
            for (int i = 1; i < sz - 1; ++i) {
                if (i != m_activePointIndex) {
                    ellipse.moveCenter(m_points.at(i));
                    painter->drawEllipse(ellipse);
                }
            }

            if (m_activePointIndex != -1) {
                ellipse.moveCenter(m_points.at(m_activePointIndex));
                painter->setBrush(invertedColor);
                painter->setPen(Qt::NoPen);
                painter->drawEllipse(ellipse);
            }
            else if (m_activeSegmentIndex != -1) {
                painter->setPen(QPen(invertedColor, _pen.widthF() + 1));
                painter->setBrush(Qt::NoBrush);

                QLineF segmentLine(m_points[m_activeSegmentIndex], m_points[m_activeSegmentIndex + 1]);
                painter->drawLine(segmentLine);
            }
        }

        // Now restore the points array
        m_points[0] = savedStart;
        m_points[sz - 1] = savedEnd;
    }

    /// Determines the active point or segment, the latter being given more priority.
    void AssociationLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if (event->buttons() & Qt::LeftButton) {
            m_activePointIndex = closestPointIndex(event->pos());
            if (m_activePointIndex != -1 && isEndPointIndex(m_activePointIndex)) {
                // End points are not drawn and hence not active.
                m_activePointIndex = -1;
            }
            // calculate only if active point index is -1
            m_activeSegmentIndex = (m_activePointIndex != -1) ? -1 : segmentIndex(event->pos());
        }
        else {
            m_activePointIndex = m_activeSegmentIndex = -1;
        }
    }

    /// Moves the point or line if active.
    void AssociationLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_activePointIndex != -1) {
            setPoint(m_activePointIndex, event->pos());
        }
        else if (m_activeSegmentIndex != -1 && !isEndSegmentIndex(m_activeSegmentIndex)) {
            QPointF delta = event->scenePos() - event->lastScenePos();
            setPoint(m_activeSegmentIndex, m_points[m_activeSegmentIndex] + delta);
            setPoint(m_activeSegmentIndex + 1, m_points[m_activeSegmentIndex + 1] + delta);
        } else {
            return;
        }
        calculateEndPoints();
    }

    /// Reset active indices and also push undo command.
    void AssociationLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (event->buttons() & Qt::LeftButton) {
            m_activeSegmentIndex = m_activePointIndex = -1;
        }
    }

    /// Inserts a new point at double click position.
    void AssociationLine::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        int index = closestPointIndex(event->pos());
        // First check if double click was on a non end point.
        if (index != -1 && !isEndPointIndex(index)) {
            if (!m_associationWidget->isSelf() || count() > 4) {
                removePoint(index);
            }
        }
        else {
            // Else insert a new point on the line segment
            index = segmentIndex(event->pos());
            if (index != -1) {
                // Insert after the obtained index.
                insertPoint(index + 1, event->pos());
            }
        }
    }

    /**
     * Calculates the "to be highlighted" point and segment indicies
     * and updates if necessary.
     */
    void AssociationLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
    {
        int oldPointIndex = m_activePointIndex;
        int oldSegmentIndex = m_activeSegmentIndex;

        m_activePointIndex = closestPointIndex(event->pos());
        // End points are not drawn and hence not active.
        if (m_activePointIndex != -1 && isEndPointIndex(m_activePointIndex)) {
            m_activePointIndex = -1;
        }
        // Activate segment index only if point index is -1
        m_activeSegmentIndex = (m_activePointIndex != -1) ? -1 : segmentIndex(event->pos());

        bool isChanged = (oldSegmentIndex != m_activeSegmentIndex || oldPointIndex != m_activePointIndex);
        if (isChanged) {
            m_associationWidget->update();
        }
    }

    /**
     * Calculates the "to be highlighted" point and segment indicies
     * and updates if necessary.
     */
    void AssociationLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
    {
        int oldPointIndex = m_activePointIndex;
        int oldSegmentIndex = m_activeSegmentIndex;

        m_activePointIndex = closestPointIndex(event->pos());
        // End points are not drawn and hence not active.
        if (m_activePointIndex != -1 && isEndPointIndex(m_activePointIndex)) {
            m_activePointIndex = -1;
        }
        // Activate segment index only if point index is -1
        m_activeSegmentIndex = (m_activePointIndex != -1) ? -1 : segmentIndex(event->pos());

        bool isChanged = (oldSegmentIndex != m_activeSegmentIndex || oldPointIndex != m_activePointIndex);
        if (isChanged) {
            m_associationWidget->update();
        }
    }

    /// Reset active indicies and updates.
    void AssociationLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
    {
        Q_UNUSED(event);
        m_activePointIndex = m_activeSegmentIndex = -1;
        m_associationWidget->update();
    }

    /**
     * Calculates the "shape" and also the "bounding rectangle"
     * required by GraphicsView framework.
     */
    void AssociationLine::calculateBoundingRect()
    {
        if (m_points.isEmpty()) {
            m_shape = QPainterPath();
            m_boundingRect = QRectF();
            return;
        }

        QPainterPath path(m_points.first());
        for(int i = 1; i < m_points.size(); ++i) {
            path.lineTo(m_points.at(i));
        }

        QRectF ellipse(0, 0, SelectedPointDiameter, SelectedPointDiameter);
        foreach(QPointF point, m_points) {
            ellipse.moveCenter(point);
            path.addEllipse(ellipse);
        }

        QPainterPathStroker stroker;
        stroker.setWidth(qMax(AssociationLine::Delta, pen().widthF()) + 2); // allow delta region

        m_shape = stroker.createStroke(path);
        m_boundingRect = m_shape.boundingRect();

        alignHeadSymbols();
    }

    /**
     * @internal
     * @short A helper method to return the region of rect intersected by line.
     */
    static Uml::Region intersectedRegion(const QRectF& rect,
            const QLineF& line)
    {
        // This maps the region of rect to QLineF objects representing rects' edges.
        QMap<Uml::Region, QLineF> rectLines;

        // Do the mapping.
        rectLines[Uml::reg_West] = QLineF(rect.topLeft(), rect.bottomLeft());
        rectLines[Uml::reg_North] = QLineF(rect.topLeft(), rect.topRight());
        rectLines[Uml::reg_East] = QLineF(rect.topRight(), rect.bottomRight());
        rectLines[Uml::reg_South] = QLineF(rect.bottomLeft(), rect.bottomRight());

        // This holds whether a given rect edge(represented by QLineF
        // objects) is intersected by line.
        QMap<Uml::Region, bool> intersectionMap;
        for (int i = Uml::reg_West; i <= Uml::reg_South; ++i) {
            Uml::Region r = (Uml::Region)i;
            QPointF temp;
            intersectionMap[r] = (line.intersect(rectLines[r], &temp) ==
                    QLineF::BoundedIntersection);
        }

        // Do intersection mapping separately for corner regions.
        intersectionMap[Uml::reg_NorthWest] = (intersectionMap[Uml::reg_North] == true
                && intersectionMap[Uml::reg_West] == true);
        intersectionMap[Uml::reg_NorthEast] = (intersectionMap[Uml::reg_North] == true
                && intersectionMap[Uml::reg_East] == true);
        intersectionMap[Uml::reg_SouthWest] = (intersectionMap[Uml::reg_South] == true
                && intersectionMap[Uml::reg_West] == true);
        intersectionMap[Uml::reg_SouthEast] = (intersectionMap[Uml::reg_South] == true
                && intersectionMap[Uml::reg_East] == true);

        Uml::Region intersection = Uml::reg_Error;
        for (int i = Uml::reg_West; i <= Uml::reg_SouthWest; ++i) {
            const Uml::Region reg = (Uml::Region)i;
            if (intersectionMap[reg] == true) {
                intersection = reg;
            }
        }

        return intersection;
    }

    /**
     * This important method is responsible for determining the appropriate
     * regions of UMLWidget to be occupied by this AssociationLine ends.
     *
     * After determining, AssociationSpaceManager::arrange() is invoked for
     * the UMLWidget's modified region space to set the actual end points of
     * this AssociationLine.
     */
    void AssociationLine::calculateEndPoints()
    {
        if (m_associationWidget->isSelf()) {
            calculateSelfEndPoints();
            return;
        }
        UMLWidget *widA = m_associationWidget->widgetForRole(Uml::A);
        UMLWidget *widB = m_associationWidget->widgetForRole(Uml::B);

        QRectF aRect = widA->sceneRect();
        QRectF bRect = widB->sceneRect();

        // aLine and bLine are QLineF objects whose intersection with the
        // rectangle edges determine the AssociationLine's endings regions
        // occupied in both the UMLWidgets.
        // Intially for count <= 2, this is line joining centres of two
        // rects.
        // For count > 2, we set the AssociationLine's penultimate point from
        // corresponding widgets, as line's other end.
        QLineF  aLine = QLineF(aRect.center(), bRect.center());
        QLineF bLine = aLine;
        if (count() > 2) {
            aLine.setP2(m_associationWidget->mapToScene(point(1)));
            bLine.setP1(m_associationWidget->mapToScene(point(count()-2)));
        }

#if 0
        if (!tracker->scene()) {

            //widA->scene()->addItem(tracker);
        }
        tracker->setLine(aLine);
#endif


        // Though a pair is used, only the first value is used in case of
        // non self associations.
        RegionPair aNewRegion = intersectedRegion(aRect, aLine);
        RegionPair bNewRegion = intersectedRegion(bRect, bLine);
        if (aNewRegion.first == Uml::reg_Error) {
            aNewRegion.first = Uml::reg_North; // defaults to North
        }
        if (bNewRegion.first == Uml::reg_Error) {
            bNewRegion.first = Uml::reg_North; // defaults to North
        }

        AssociationSpaceManager *aSpaceManager = widA->associationSpaceManager();
        AssociationSpaceManager *bSpaceManager = widB->associationSpaceManager();

        // Now move the AssociationWidget to proper regions.
        RegionPair aPrevRegions = aSpaceManager->remove(m_associationWidget);
        aSpaceManager->add(m_associationWidget, aNewRegion);
        RegionPair bPrevRegions = bSpaceManager->remove(m_associationWidget);
        bSpaceManager->add(m_associationWidget, bNewRegion);

        // Finally call AssociationSpaceManager::arrange to do the actual
        // placement of line endings of this AssociationLine.
        aSpaceManager->arrange(aPrevRegions);
        aSpaceManager->arrange(aNewRegion);
        bSpaceManager->arrange(bPrevRegions);
        bSpaceManager->arrange(bNewRegion);
    }

    /**
     * This method is responsible for determining the appropriate
     * regions of UMLWidget to be occupied by this self AssociationLine ends.
     *
     * After determining, AssociationSpaceManager::arrange() is invoked for
     * the UMLWidget's modified region space to set the actual end points of
     * this AssociationLine.
     */
    void AssociationLine::calculateSelfEndPoints()
    {
        Q_ASSERT(m_associationWidget->isSelf());
        UMLWidget *wid = m_associationWidget->widgetForRole(Uml::A);
        AssociationSpaceManager *spaceManager = wid->associationSpaceManager();

        QRectF rect = wid->sceneRect();

        if (count() < 4) {
            RegionPair newRegions;
            for (int i = count(); i < 4; ++i) {
                insertPoint(i, QPointF());
            }
            qreal left = rect.left() + (.25 * rect.width());
            QRectF r(0, 0, rect.width() * .5,
                    SelfAssociationMinimumHeight);
            bool drawAbove = rect.top() >= SelfAssociationMinimumHeight;
            if (drawAbove) {
                r.moveBottomLeft(QPointF(left, rect.top()));
                r = m_associationWidget->mapFromScene(r).boundingRect();

                setPoint(0, r.bottomLeft());
                setPoint(1, r.topLeft());
                setPoint(2, r.topRight());
                setPoint(3, r.bottomRight());

                newRegions = RegionPair(Uml::reg_North, Uml::reg_North);
            } else {
                r.moveTopLeft(QPointF(left, rect.bottom()));
                r = m_associationWidget->mapFromScene(r).boundingRect();

                setPoint(0, r.topLeft());
                setPoint(1, r.bottomLeft());
                setPoint(2, r.bottomRight());
                setPoint(3, r.topRight());

                newRegions = RegionPair(Uml::reg_South, Uml::reg_South);
            }

            RegionPair prevRegions = spaceManager->remove(m_associationWidget);
            spaceManager->add(m_associationWidget, newRegions);
            spaceManager->arrange(prevRegions);
            spaceManager->arrange(newRegions);
        } else {
            QLineF aLine(rect.center(), m_associationWidget->mapToScene(point(1)));
            QLineF bLine(m_associationWidget->mapToScene(point(count()-2)),
                    rect.center());

            RegionPair intersectedRegions;
            intersectedRegions.first = intersectedRegion(rect, aLine);
            intersectedRegions.second = intersectedRegion(rect, bLine);

            RegionPair oldIntersections = spaceManager->remove(m_associationWidget);
            spaceManager->add(m_associationWidget, intersectedRegions);

            spaceManager->arrange(oldIntersections);
            spaceManager->arrange(intersectedRegions);
        }
    }

    /**
     * This method simply ensures presence of two points and delegates
     * calculation to AssociationLine::calculateEndPoints()
     */
    void AssociationLine::calculateInitialEndPoints()
    {
        if (count() < 2) {
            setEndPoints(QPointF(), QPointF());
        }

        calculateEndPoints();
    }
}

