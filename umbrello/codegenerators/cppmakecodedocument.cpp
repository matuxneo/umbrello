
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by: 
 *      Author : thomas
 *      Date   : Tue Jun 24 2003
 */

#include "cppcodegenerator.h"

#include <qregexp.h>

// Constructors/Destructors
//  

CPPMakefileCodeDocument::CPPMakefileCodeDocument ( CPPCodeGenerator * gen ) 
    : CodeDocument ( (CodeGenerator *)gen ) 
{
	setFileName("Makefile"); // default name
	setFileExtension("");
	setID("Makefile_DOC"); // default id tag for this type of document
};

CPPMakefileCodeDocument::~CPPMakefileCodeDocument ( ) { };

//  
// Methods
//  

// Other methods
//  

/** set the class attributes of this object from
 * the passed element node.
 */
void CPPMakefileCodeDocument::setAttributesFromNode ( QDomElement & root)
{

        // superclass save
        CodeDocument::setAttributesFromNode(root);

        // now set local attributes
// setPackage(root.attribute("package",""));

}

/**
 * load params from the appropriate XMI element node.
 */
void CPPMakefileCodeDocument::loadFromXMI ( QDomElement & root ) {
        setAttributesFromNode(root);
}

/** set attributes of the node that represents this class
 * in the XMI document.
 */
void CPPMakefileCodeDocument::setAttributesOnNode ( QDomDocument & doc, QDomElement & docElement)
{

        // superclass call
        CodeDocument::setAttributesOnNode(doc,docElement);

        // now set local attributes/fields
//FIX 
}

/**
 * Save the XMI representation of this object
 * @return      bool    status of save
 */
bool CPPMakefileCodeDocument::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
        bool status = true;

        QDomElement docElement = doc.createElement( "cppmakecodedocument" );

        setAttributesOnNode(doc, docElement);

        root.appendChild( docElement );

        return status;
}

// we add in our code blocks that describe how to generate
// the project here...
void CPPMakefileCodeDocument::updateContent( ) {
   // FIX : fill in content
}

/**
 * @return	QString
 */
QString CPPMakefileCodeDocument::toString ( ) {
	return "# cpp make build document";
}

// We overwritten by CPP language implementation to get lowercase path
QString CPPMakefileCodeDocument::getPath ( )
{

        QString path = getPackage();

        // Replace all white spaces with blanks
        path.simplifyWhiteSpace();

        // Replace all blanks with underscore
        path.replace(QRegExp(" "), "_");

        path.replace(QRegExp("\\."),"/");

        path.lower();

        return path;

}

