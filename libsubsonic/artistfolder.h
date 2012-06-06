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

#ifndef ARTISTFOLDER_H
#define ARTISTFOLDER_H

#include <QObject>

class IndexFolder : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString id READ id WRITE setId)
	Q_PROPERTY(QString name READ name WRITE setName)
public:
	explicit IndexFolder(QObject *parent = 0);
	void setId(QString i) { mId = i; }
	QString id() { return mId; }

	void setName(QString n) { mName = n; }
	QString name() { return mName; }

private:
	QString mId;
	QString mName;
};

#endif // ARTISTFOLDER_H
