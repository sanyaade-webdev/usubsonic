/**************************************************************************
*   Copyright (C) 2012 Intel Corporation                                  *
*                                                                         *
*                                                                         *
*   This file is free software: you can redistribute it and/or modify     *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation, either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   It is distributed in the hope that it will be useful                  *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   If not, see <http://www.gnu.org/licenses/>.                           *
***************************************************************************/

#include "songobject.h"

#include <QMetaProperty>
#include <QtDebug>

MusicObject::MusicObject(QObject *parent) :
	QObject(parent)
{
}

MusicObject::MusicObject(MusicObject *other)
{
	for(int i=0; i<metaObject()->propertyCount(); i++)
	{
		qDebug()<<"writing property: "<<i<<" "<<metaObject()->property(i).name();
		metaObject()->property(i).write(this,other->metaObject()->property(i).read(other));
	}
}
