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

#ifndef SUBSONIC_QUICK_PLUGIN_H
#define SUBSONIC_QUICK_PLUGIN_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

class Subsonic_QuickPlugin : public QDeclarativeExtensionPlugin
{
	Q_OBJECT
	
public:
	void registerTypes(const char *uri);
};

#endif // SUBSONIC_QUICK_PLUGIN_H

