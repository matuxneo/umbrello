/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef TOOLBARSTATEOTHER_H
#define TOOLBARSTATEOTHER_H

#include "toolbarstatepool.h"

/**
 * Other tool creates almost all the objects (except associations and messages).
 * Objects are created when left button is released, no matter if it was
 * released on an association, on a widget or on an empty space.
 *
 * Associations and widgets aren't taken into account, and are treated as empty
 * spaces.
 */
class ToolBarStateOther : public ToolBarStatePool {
    Q_OBJECT
public:

    /**
     * Creates a new ToolBarStateOther.
     *
     * @param umlView The UMLView to use.
     */
    ToolBarStateOther(UMLScene *umlScene);

    /**
     * Destroys this ToolBarStateOther.
     */
    virtual ~ToolBarStateOther();

private:

    virtual void setCurrentElement();

    virtual void mouseReleaseEmpty();

    Uml::Object_Type getObjectType();

    bool newWidget();

};

#endif //TOOLBARSTATEOTHER_H
