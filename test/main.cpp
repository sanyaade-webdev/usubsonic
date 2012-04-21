#include "../libsubsonic/subsonic.h"
#include <QCoreApplication>

#include "testclass.h"

int main(int argc, char** argv)
{
	QCoreApplication app(argc,argv);

	TestClass test;

	return app.exec();
}
