/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef CMDRENAMEUMLOBJECT_H
#define CMDRENAMEUMLOBJECT_H

#include <QUndoCommand>

class UMLObject;

namespace Uml
{
    class CmdRenameUMLObject : public QUndoCommand
    {
        public:
            CmdRenameUMLObject(UMLObject* o, const QString& name);
            ~CmdRenameUMLObject();

            void redo();
            void undo();

        private:
            UMLObject*  m_obj;
            QString     m_name;
            QString     m_oldname;
    };
}

#endif
