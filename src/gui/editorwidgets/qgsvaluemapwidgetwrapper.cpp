/***************************************************************************
    qgsvaluemapwidgetwrapper.cpp
     --------------------------------------
    Date                 : 5.1.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsvaluemapwidgetwrapper.h"
#include "qgsvaluemapconfigdlg.h"
#include "qgsvaluemapfieldformatter.h"

#include <QSettings>

QgsValueMapWidgetWrapper::QgsValueMapWidgetWrapper( QgsVectorLayer *vl, int fieldIdx, QWidget *editor, QWidget *parent )
  : QgsEditorWidgetWrapper( vl, fieldIdx, editor, parent )

{
}


QVariant QgsValueMapWidgetWrapper::value() const
{
  QVariant v;

  if ( mComboBox )
    v = mComboBox->currentData();

  if ( v == QgsValueMapFieldFormatter::NULL_VALUE )
    v = QVariant( field().type() );

  return v;
}

void QgsValueMapWidgetWrapper::showIndeterminateState()
{
  if ( mComboBox )
  {
    whileBlocking( mComboBox )->setCurrentIndex( -1 );
  }
}

QWidget *QgsValueMapWidgetWrapper::createWidget( QWidget *parent )
{
  return new QComboBox( parent );
}

void QgsValueMapWidgetWrapper::initWidget( QWidget *editor )
{
  mComboBox = qobject_cast<QComboBox *>( editor );

  if ( mComboBox )
  {
    QgsValueMapConfigDlg::populateComboBox( mComboBox, config(), false );
    connect( mComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             this, static_cast<void ( QgsEditorWidgetWrapper::* )()>( &QgsEditorWidgetWrapper::emitValueChanged ) );
  }
}

bool QgsValueMapWidgetWrapper::valid() const
{
  return mComboBox;
}

void QgsValueMapWidgetWrapper::setValue( const QVariant &value )
{
  QString v;
  if ( value.isNull() )
    v = QgsValueMapFieldFormatter::NULL_VALUE;
  else
    v = value.toString();

  if ( mComboBox )
    mComboBox->setCurrentIndex( mComboBox->findData( v ) );
}
