#include <QtWidgets/QApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore/QMetaObject>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QTextCodec>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include <QtGui/QFontDatabase>

#include <app/WbApplication.hpp>
#include <core/WbStandardPaths.hpp>
#include <core/WbSimulationState.hpp>
#include <core/WbPreferences.hpp>
#include <core/WbSysInfo.hpp>
#include <core/WbProject.hpp>
#include <wren/WbWrenOpenGlContext.hpp>
#include <gui/RobotVisualizer.hpp>

#include <gui/WbSingleTaskApplication.hpp>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Application Example");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

	QLocale::setDefault(QLocale::c());
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));  // so that all QTextStream use UTF-8 encoding by default

	QWidget mainWin;
	mainWin.setAttribute(Qt::WA_DeleteOnClose, true);

	QVBoxLayout *layout = new QVBoxLayout(&mainWin);
	layout->addWidget(new QLabel("CMES"));

	RobotVisualizer *view3D = new RobotVisualizer();
	//view3D->setAttribute(Qt::WA_DeleteOnClose, false);
	layout->addWidget(view3D);

	layout->setStretch(0, 0);
	layout->setStretch(1, 1);

	//view3D->show();
	mainWin.show();
	mainWin.resize(600, 500);

	view3D->initialize();

	WbPreferences *const prefs = WbPreferences::instance();
	const QString defaultFileName = WbStandardPaths::emptyProjectPath() + "worlds/" + WbProject::newWorldFileName();
	QString mStartWorldName = prefs->value("RecentFiles/file0", defaultFileName).toString();

	view3D->loadWorld(mStartWorldName);
	view3D->setModeRealtime();

	int rc = app.exec();

	delete view3D;

	WbApplication::instance()->simulationQuit(EXIT_SUCCESS);

	return rc;
}
