/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef UNIQUECONSTRAINT_H
#define UNIQUECONSTRAINT_H

#include "entityconstraint.h"
#include "classifierlistitem.h"
#include "umlnamespace.h"
#include "umlclassifierlist.h"
#include "umlentityattributelist.h"

/**
 * This class is used to set up information for a unique entity constraint.
 *
 * @short Sets up Unique entity constraint information.
 * @author Sharan Rao
 * @see UMLObject UMLClassifierListItem UMLEntityConstraint
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class UMLUniqueConstraint : public UMLEntityConstraint {
public:

    /**
     * Sets up a constraint.
     *
     * @param parent    The parent of this UMLUniqueConstraint.
     * @param name      The name of this UMLUniqueConstraint.
     * @param id        The unique id given to this UMLUniqueConstraint.
     */
    UMLUniqueConstraint(UMLObject *parent, const QString& name,
                        Uml::IDType id = Uml::id_None );

    /**
     * Sets up a constraint.
     *
     * @param parent    The parent of this UMLUniqueConstraint.
     */
    UMLUniqueConstraint(UMLObject *parent);

    /**
     * Overloaded '==' operator
     */
    bool operator==(const UMLUniqueConstraint &rhs);

    /**
     * destructor.
     */
    virtual ~UMLUniqueConstraint();

    virtual void copyInto(UMLObject *lhs) const;

    virtual UMLObject* clone() const;

    QString toString(Uml::Signature_Type sig = Uml::st_NoSig);

    QString getFullyQualifiedName(const QString& separator = QString(),
                                  bool includeRoot = false) const;

    void saveToXMI( QDomDocument & qDoc, QDomElement & qElement );

    virtual bool showPropertiesDialog(QWidget* parent);

    bool hasEntityAttribute(UMLEntityAttribute* attr);

    bool addEntityAttribute(UMLEntityAttribute* attr);

    bool removeEntityAttribute(UMLEntityAttribute* attr);


    UMLEntityAttributeList getEntityAttributeList() const {
        return m_EntityAttributeList;
    }

    void clearAttributeList();

protected:

    bool load( QDomElement & element );

private:

    void init();

    /**
     * The list of entity attributes that together make up the unique constraint
     */
    UMLEntityAttributeList m_EntityAttributeList;


};

#endif
