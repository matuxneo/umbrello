
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

#ifndef CPPHEADERCODECLASSFIELDDECLARATIONBLOCK_H
#define CPPHEADERCODECLASSFIELDDECLARATIONBLOCK_H

#include <qstring.h>

#include "../codeclassfielddeclarationblock.h"

class CPPHeaderCodeClassFieldDeclarationBlock : public CodeClassFieldDeclarationBlock
{
	Q_OBJECT
public:

	// Constructors/Destructors
	//  

	/**
	 * Constructor
	 */
	CPPHeaderCodeClassFieldDeclarationBlock ( CodeClassField * parent );

	/**
	 * Empty Destructor
	 */
	virtual ~CPPHeaderCodeClassFieldDeclarationBlock ( );

protected:

	// this will be called by syncToParent whenever the parent object is "modified" 
       	void updateContent ( );

private:


};

#endif // CPPHEADERCODECLASSFIELDDECLARATIONBLOCK_H
