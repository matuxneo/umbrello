/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2007                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "entityattribute.h"
// qt/kde includes
#include <qregexp.h>
#include <kdebug.h>
// app includes
#include "umlcanvasobject.h"
#include "umldoc.h"
#include "uml.h"
#include "dialogs/umlentityattributedialog.h"
#include "object_factory.h"

UMLEntityAttribute::UMLEntityAttribute( UMLObject *parent, const QString& name,
                                        Uml::IDType id, Uml::Visibility s,
                                        UMLObject *type, const QString& iv )
        : UMLAttribute(parent, name, id, s, type, iv) {
    init();
    if (m_pSecondary) {
        m_pSecondary->setBaseType(Uml::ot_Entity);
    }
}

UMLEntityAttribute::UMLEntityAttribute(UMLObject *parent) : UMLAttribute(parent) {
    init();
}

UMLEntityAttribute::~UMLEntityAttribute() { }

/**
 * Initialize members of this class.
 * Auxiliary method used by constructors.
 */
void UMLEntityAttribute::init() {
    m_BaseType = Uml::ot_EntityAttribute;
    m_indexType = Uml::None;
    m_autoIncrement = false;
    m_null = false;
}

/**
 * Returns the value of the UMLEntityAttribute's attributes property.
 *
 * @return  The value of the UMLEntityAttribute's attributes property.
 */
QString UMLEntityAttribute::getAttributes() const{
    return m_attributes;
}

/**
 * Sets the UMLEntityAttribute's attributes property.
 *
 * @param attributes  The new value for the attributes property.
 */
void UMLEntityAttribute::setAttributes(const QString& attributes) {
    m_attributes = attributes;
}

/**
 * Returns the UMLEntityAttribute's length/values property.
 *
 * @return  The new value of the length/values property.
 */
QString UMLEntityAttribute::getValues() const{
    return m_values;
}

/**
 * Sets the UMLEntityAttribute's length/values property.
 *
 * @param values    The new value of the length/values property.
 */
void UMLEntityAttribute::setValues(const QString& values) {
    m_values = values;
}

/**
 * Returns the UMLEntityAttribute's auto_increment boolean
 *
 * @return  The UMLEntityAttribute's auto_increment boolean
 */
bool UMLEntityAttribute::getAutoIncrement() const{
    return m_autoIncrement;
}

/**
 * Sets the UMLEntityAttribute's auto_increment boolean
 *
 * @param autoIncrement  The UMLEntityAttribute's auto_increment boolean
 */
void UMLEntityAttribute::setAutoIncrement(const bool autoIncrement) {
    m_autoIncrement = autoIncrement;
}

/**
 * Returns the UMLEntityAttribute's index type property.
 *
 * @return  The value of the UMLEntityAttribute's index type property.
 */
Uml::DBIndex_Type UMLEntityAttribute::getIndexType() const{
    return m_indexType;
}

/**
 * Sets the initial value of the UMLEntityAttribute's index type property.
 *
 * @param indexType  The initial value of the UMLEntityAttribute's index type property.
 */
void UMLEntityAttribute::setIndexType(const Uml::DBIndex_Type indexType) {
    m_indexType = indexType;
}

/**
 * Returns the UMLEntityAttribute's allow null value.
 *
 * @return  The UMLEntityAttribute's allow null value.
 */
bool UMLEntityAttribute::getNull() const{
    return m_null;
}

/**
 * Sets the initial value of the UMLEntityAttribute's allow null value.
 *
 * @param null        The initial value of the UMLEntityAttribute's allow null value.
 */
void UMLEntityAttribute::setNull(const bool nullIn) {
    m_null = nullIn;
}

/**
 * Returns a string representation of the UMLEntityAttribute.
 *
 * @param sig               If true will show the entityattribute type and
 *                  initial value.
 * @return  Returns a string representation of the UMLEntityAttribute.
 */
QString UMLEntityAttribute::toString(Uml::Signature_Type sig) {
    QString s;
    //FIXME

    if(sig == Uml::st_ShowSig || sig == Uml::st_NoSig) {
        s=m_Vis.toString(true) + ' ';
    }

    if(sig == Uml::st_ShowSig || sig == Uml::st_SigNoVis) {
        QString string = s + getName() + " : " + getTypeName();
        if(m_InitialValue.length() > 0)
            string += " = " + m_InitialValue;
        return string;
    } else
        return s + getName();
}

bool UMLEntityAttribute::operator==(const UMLEntityAttribute &rhs) {
    if( this == &rhs )
        return true;

    if( !UMLObject::operator==( rhs ) )
        return false;

    // The type name is the only distinguishing criterion.
    // (Some programming languages might support more, but others don't.)
    if (m_pSecondary != rhs.m_pSecondary)
        return false;

    return true;
}

/**
 * Copy the internal presentation of this object into the UMLEntityAttribute
 * object.
 */
void UMLEntityAttribute::copyInto(UMLObject *lhs) const
{
    UMLEntityAttribute *target = static_cast<UMLEntityAttribute*>(lhs);

    // call the parent first.
    UMLClassifierListItem::copyInto(target);

    // Copy all datamembers
    target->m_pSecondary = m_pSecondary;
    target->m_SecondaryId = m_SecondaryId;
    target->m_InitialValue = m_InitialValue;
    target->m_ParmKind = m_ParmKind;
}

/**
 * Make a clone of the UMLEntityAttribute.
 */
UMLObject* UMLEntityAttribute::clone() const
{
    UMLEntityAttribute* clone = new UMLEntityAttribute( (UMLEntityAttribute*)parent() );
    copyInto(clone);

    return clone;
}


/**
 * Creates the <UML:EntityAttribute> XMI element.
 */
void UMLEntityAttribute::saveToXMI( QDomDocument & qDoc, QDomElement & qElement ) {
    QDomElement entityattributeElement = UMLObject::save("UML:EntityAttribute", qDoc);
    if (m_pSecondary == NULL) {
        uDebug() << m_Name << ": m_pSecondary is NULL, using local name "
            << m_SecondaryId << endl;
        entityattributeElement.setAttribute( "type", m_SecondaryId );
    } else {
        entityattributeElement.setAttribute( "type", ID2STR(m_pSecondary->getID()) );
    }
    entityattributeElement.setAttribute( "initialValue", m_InitialValue );
    entityattributeElement.setAttribute( "dbindex_type", m_indexType );
    entityattributeElement.setAttribute( "values", m_values );
    entityattributeElement.setAttribute( "attributes", m_attributes );
    entityattributeElement.setAttribute( "auto_increment", m_autoIncrement );
    entityattributeElement.setAttribute( "allow_null", m_null );
    qElement.appendChild( entityattributeElement );
}

/**
 * Loads the <UML:EntityAttribute> XMI element.
 */
bool UMLEntityAttribute::load( QDomElement & element ) {
    if (! UMLAttribute::load(element))
        return false;
    int indexType = element.attribute( "dbindex_type", "1100" ).toInt();
    m_indexType = ( Uml::DBIndex_Type )indexType;
    m_values = element.attribute( "values", "" );
    m_attributes = element.attribute( "attributes", "" );
    m_autoIncrement = ( bool )element.attribute( "auto_increment", "" ).toInt();
    m_null = ( bool )element.attribute( "allow_null", "" ).toInt();
    return true;
}

/**
 * Display the properties configuration dialog for the entityattribute.
 */
bool UMLEntityAttribute::showPropertiesDialog(QWidget* parent) {
    UMLEntityAttributeDialog dialog(parent, this);
    return dialog.exec();
}

#include "entityattribute.moc"

