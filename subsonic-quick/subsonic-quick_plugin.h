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

