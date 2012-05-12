#include "subsonic-quick_plugin.h"
#include "subsonicmodel.h"
#include "subsonicmediaplayer.h"

#include <QtDeclarative/qdeclarative.h>

void Subsonic_QuickPlugin::registerTypes(const char *uri)
{
	// @uri org.subsonic
	qmlRegisterType<SubsonicModel>(uri, 1, 0, "SubsonicModel");
	qmlRegisterType<SubsonicMediaPlayer>(uri, 1, 0, "SubsonicMediaPlayer");
	qmlRegisterUncreatableType<IndexFolder>(uri, 1, 0, "IndexFolder", "This type is uncreatable");
	qmlRegisterUncreatableType<MusicObject>(uri, 1, 0, "MusicObject", "This type is uncreatable");
}

Q_EXPORT_PLUGIN2(Subsonic_Quick, Subsonic_QuickPlugin)

