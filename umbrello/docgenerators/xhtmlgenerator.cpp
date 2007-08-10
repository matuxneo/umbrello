/***************************************************************************
 *                        xhtmlgenerator.cpp  -  description               *
 *                           -------------------                           *
 *  copyright            : (C) 2006 by Gael de Chalendar (aka Kleag)       *
 *    (C) 2006 Umbrello UML Modeller Authors <uml-devel@uml.sf.net>        *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "xhtmlgenerator.h"

#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kio/netaccess.h>
#include <kio/job.h>

#include <qfile.h>
#include <qregexp.h>
#include <qtextstream.h>

#include "docbook2xhtmlgeneratorjob.h"
#include "uml.h"
#include "umldoc.h"
#include "umlviewimageexportermodel.h"
#include "docbookgenerator.h"

XhtmlGenerator::XhtmlGenerator()
{
  umlDoc = UMLApp::app()->getDocument();
  m_pStatus = true;
}

XhtmlGenerator::~XhtmlGenerator(){}

bool XhtmlGenerator::generateXhtmlForProject()
{
  KUrl url = umlDoc->url();
  QString fileName = url.fileName();
  fileName.replace(QRegExp(".xmi$"),"");
  url.setFileName(fileName);
  kDebug()<<k_funcinfo<< "Exporting to directory: " << url;
  return generateXhtmlForProjectInto(url);
}

bool XhtmlGenerator::generateXhtmlForProjectInto(const KUrl& destDir)
{
    kDebug() <<k_funcinfo<< "First convert to docbook";
  m_destDir = destDir;
//   KUrl url(QString("file://")+m_tmpDir.name());
  DocbookGenerator* docbookGenerator = new DocbookGenerator;
  docbookGenerator->generateDocbookForProjectInto(destDir);

  kDebug() <<k_funcinfo<< "Connecting...";
  connect(docbookGenerator, SIGNAL(finished(bool)), this, SLOT(slotDocbookToXhtml(bool)));
  return true;
}

void XhtmlGenerator::slotDocbookToXhtml(bool status)
{
  kDebug() << "Now convert docbook to html...";
  if ( !status )
  {
      kDebug()<<k_funcinfo<<"Error in converting to docbook";
      m_pStatus = false;
      return;
  } else {

    KUrl url = umlDoc->url();
    QString fileName = url.fileName();
    fileName.replace(QRegExp(".xmi$"),".docbook");
    url.setPath(m_destDir.path());
    url.addPath(fileName);

    umlDoc->writeToStatusBar( i18n( "Generating XHTML..." ) );
    Docbook2XhtmlGeneratorJob* d2xg  = new Docbook2XhtmlGeneratorJob( url, this );
    connect( d2xg, SIGNAL( xhtmlGenerated( const QString& ) ), this, SLOT( slotHtmlGenerated(const QString&) ) );
    connect( d2xg, SIGNAL( finished() ), this, SLOT( cleanUpAndExit() ) );
    kDebug()<<k_funcinfo<<"Threading";
    d2xg->start();
  }
}

void XhtmlGenerator::slotHtmlGenerated(const QString& tmpFileName)
{

    kDebug() << "HTML Generated"<<tmpFileName;
    KUrl url = umlDoc->url();
    QString fileName = url.fileName();
    fileName.replace(QRegExp(".xmi$"),".html");
    url.setPath(m_destDir.path());
    url.addPath(fileName);

    KIO::Job* htmlCopyJob = KIO::file_copy( KUrl::fromPath( tmpFileName ), url, -1, true, true, false );
    if ( KIO::NetAccess::synchronousRun( htmlCopyJob, (QWidget*)UMLApp::app() ) ) {
        umlDoc->writeToStatusBar(i18n("XHTML Generation Complete..."));
    } else {
        m_pStatus = false;
        return;
    }

    umlDoc->writeToStatusBar(i18n("Copying CSS..."));

    QString cssFileName(KGlobal::dirs()->findResource("appdata","xmi.css"));
    KUrl cssUrl = m_destDir;
    cssUrl.addPath("xmi.css");
    KIO::Job* cssJob = KIO::file_copy(cssFileName,cssUrl,-1,true,true,false);

    if ( KIO::NetAccess::synchronousRun( cssJob, (QWidget*)UMLApp::app() ) ) {
        umlDoc->writeToStatusBar(i18n("Finished Copying CSS..."));
        m_pStatus = true;
    } else {
        umlDoc->writeToStatusBar(i18n("Failed Copying CSS..."));
        m_pStatus = false;
    }

}

void XhtmlGenerator::cleanUpAndExit() {
    emit finished( m_pStatus );
}

#include "xhtmlgenerator.moc"
