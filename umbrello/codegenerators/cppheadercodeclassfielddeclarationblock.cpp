
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
 *      Date   : Mon Sep 1 2003
 */

#include "cppheadercodeclassfielddeclarationblock.h"

#include "cppcodeclassfield.h"

// Constructors/Destructors
//  

CPPHeaderCodeClassFieldDeclarationBlock::CPPHeaderCodeClassFieldDeclarationBlock ( CodeClassField * parent ) 
    : CodeClassFieldDeclarationBlock ( parent ) 
{
	setOverallIndentationLevel(1);
	updateContent();
};

CPPHeaderCodeClassFieldDeclarationBlock::~CPPHeaderCodeClassFieldDeclarationBlock ( ) { };

//  
// Methods
//  

// Other methods
//  

/**
 */
void CPPHeaderCodeClassFieldDeclarationBlock::updateContent( ) 
{

/*
	CodeClassField * cf = getParentClassField();
	ClassifierCodeDocument * doc = cf->getParentDocument();
	CPPCodeClassField * jcf = (CPPCodeClassField*) cf;
        CPPClassifierCodeDocument* jdoc = (CPPClassifierCodeDocument*) doc;

        // Set the comment
        QString notes = getParentObject()->getDoc();
        getComment()->setText(notes);

        // Set the body
        QString staticValue = getParentObject()->getStatic() ? "static " : "";
        QString scopeStr = jdoc->scopeToCPPDecl(getParentObject()->getScope());

        QString typeName = jcf->getTypeName();
        QString fieldName = jcf->getFieldName();
        QString initialV = jcf->getInitialValue();

        QString body = staticValue+scopeStr+" "+typeName+" "+fieldName;
        if (!initialV.isEmpty())
                body.append(" = " + initialV);
        setText(body+";");
*/
        setText("FIX ME;");


}


