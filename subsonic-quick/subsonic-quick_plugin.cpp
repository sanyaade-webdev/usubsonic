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

#include "subsonic-quick_plugin.h"
#include "subsonicmodel.h"

#include <QtDeclarative/qdeclarative.h>

void Subsonic_QuickPlugin::registerTypes(const char *uri)
{
	// @uri org.subsonic
	qmlRegisterType<SubsonicModel>(uri, 1, 0, "SubsonicModel");
	qmlRegisterUncreatableType<IndexFolder>(uri, 1, 0, "IndexFolder", "This type is uncreatable");
	qmlRegisterUncreatableType<MusicObject>(uri, 1, 0, "MusicObject", "This type is uncreatable");
}

Q_EXPORT_PLUGIN2(Subsonic_Quick, Subsonic_QuickPlugin)

