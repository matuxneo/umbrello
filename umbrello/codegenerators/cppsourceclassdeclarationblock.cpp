
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

#include "cppsourceclassdeclarationblock.h"
#include "cppcodecomment.h"

// Constructors/Destructors
//  

CPPSourceClassDeclarationBlock::CPPSourceClassDeclarationBlock ( CPPSourceCodeDocument * parentDoc, QString startText, QString endText, QString comment)
    : OwnedHierarchicalCodeBlock((UMLObject*) parentDoc->getParentClassifier(), (CodeDocument*)parentDoc, startText, endText, comment)
{
	init(parentDoc, comment);
}

CPPSourceClassDeclarationBlock::~CPPSourceClassDeclarationBlock ( ) { }

//  
// Methods
//  

/**
 * Save the XMI representation of this object
 * @return      bool    status of save
 */
bool CPPSourceClassDeclarationBlock::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
        bool status = true;

        QDomElement blockElement = doc.createElement( "cppsourceclassdeclarationblock" );

        setAttributesOnNode(doc, blockElement);

        root.appendChild( blockElement );

        return status;
}

/**
 * load params from the appropriate XMI element node.
 */
void CPPSourceClassDeclarationBlock::loadFromXMI ( QDomElement & root ) {
        setAttributesFromNode(root);
}


// Accessor methods
//  

// Other methods
//  

/**
 * update the start and end text for this hierarchicalcodeblock.
 */
void CPPSourceClassDeclarationBlock::updateContent ( ) 
{

/*
	CPPSourceCodeDocument *parentDoc = (CPPSourceCodeDocument*)getParentDocument();
	UMLClassifier *c = parentDoc->getParentClassifier();
        CodeGenerator *g = parentDoc->getParentGenerator();
	QString endLine = parentDoc->getNewLineEndingChars();
        bool isInterface = parentDoc->parentIsInterface(); // a little shortcut
        QString CPPSourceClassName = parentDoc->getCPPSourceClassName(c->getName());

	// COMMENT
        if(isInterface)
        	getComment()->setText("Interface "+CPPSourceClassName+endLine+c->getDoc());
        else
        	getComment()->setText("Class "+CPPSourceClassName+endLine+c->getDoc());

        if(g->forceDoc() || !c->getDoc().isEmpty())
		getComment()->setWriteOutText(true);
	else
		getComment()->setWriteOutText(false);


	// Now set START/ENDING Text
        QString startText = "";
        if (c->getAbstract())
                startText.append("abstract ");

        if (c->getScope() != Uml::Public) {
                // We should probably emit a warning in here .. cpp doesnt like to allow
                // private/protected classes. The best we can do (I believe)
                // is to let these declarations default to "package visibility"
                // which is a level between traditional "private" and "protected"
                // scopes. To get this visibility level we just print nothing..
        } else
                startText.append("public ");

        if(parentDoc->parentIsInterface())
                startText.append("interface ");
        else
                startText.append("class ");

        startText.append(CPPSourceClassName);

        // write inheritances out
        UMLClassifier *concept;
        UMLClassifierList superclasses =
                        c->findSuperClassConcepts(parentDoc->getParentGenerator()->getDocument());

        if(superclasses.count()>0)
                startText.append(" extends ");

        int i = 0;
        for (concept= superclasses.first(); concept; concept = superclasses.next())
        {
                startText.append(parentDoc->cleanName(concept->getName()));
                if(i>0)
                        startText.append(", ");
                i++;
        }

// FIX
// Q: Where is 'implements' ??

        // Set the header and end text for the hier.codeblock
        setText(startText+" {");

	// setEndText("}"); // not needed 
*/

}

void CPPSourceClassDeclarationBlock::init (CPPSourceCodeDocument *parentDoc, QString comment) 
{

	setComment(new CPPCodeComment(parentDoc));
	getComment()->setText(comment);

	setStartText(" class CLASS : ??? {");
	setEndText("}");

	updateContent(); 

}

