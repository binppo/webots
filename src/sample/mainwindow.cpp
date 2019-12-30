#include <QtWidgets/QtWidgets>
#include <QtWidgets/QVBoxLayout>

#include <gui/WbView3D.hpp>
#include <gui/WbSimulationView.hpp>
#include <app/WbApplication.hpp>
#include <app/WbSelection.hpp>
#include <app/WbPerspective.hpp>
#include <core/WbFileUtil.hpp>
#include <core/WbStandardPaths.hpp>
#include <core/WbSimulationState.hpp>
#include <core/WbProject.hpp>
#include <control/WbControlledWorld.hpp>
#include <engine/WbSimulationWorld.hpp>
#include <gui/WbRenderingDeviceWindowFactory.hpp>
#include <gui/WbRobotWindow.hpp>
#include <scene_tree/WbSceneTree.hpp>
#include <nodes/WbRenderingDevice.hpp>
#include <nodes/WbRobot.hpp>
#include <nodes/utils/WbWorld.hpp>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, mWorldLoadingProgressDialog(NULL)
{
	setupUi();

	style()->polish(this);
	QDir::addSearchPath("enabledIcons", ":/resources/icons/dark");
	QDir::addSearchPath("disabledIcons", ":/resources/icons/light");
	QDir::addSearchPath("coreIcons", ":/resources/icons/core");
	style()->polish(this);

	QApplication::setWindowIcon(QIcon("coreIcons:webots.png"));

	connect(WbApplication::instance(), &WbApplication::preWorldLoaded, this, &MainWindow::updateBeforeWorldLoading);
	connect(WbApplication::instance(), &WbApplication::postWorldLoaded, this, &MainWindow::updateAfterWorldLoading);
	connect(WbApplication::instance(), &WbApplication::createWorldLoadingProgressDialog, this,
		&MainWindow::createWorldLoadingProgressDialog);
	connect(WbApplication::instance(), &WbApplication::deleteWorldLoadingProgressDialog, this,
		&MainWindow::deleteWorldLoadingProgressDialog);
	connect(WbApplication::instance(), &WbApplication::worldLoadingHasProgressed, this, &MainWindow::setWorldLoadingProgress);
	connect(WbApplication::instance(), &WbApplication::worldLoadingStatusHasChanged, this, &MainWindow::setWorldLoadingStatus);

	connect(WbSimulationState::instance(), &WbSimulationState::enabledChanged, this, &MainWindow::simulationEnabledChanged);

	connect(WbApplication::instance(), &WbApplication::worldReloadRequested, this, &MainWindow::reloadWorld,
		Qt::QueuedConnection);
	connect(WbApplication::instance(), &WbApplication::simulationResetRequested, this, &MainWindow::resetWorld,
		Qt::QueuedConnection);

	mSimulationView = new WbSimulationView(this, "");
	mSimulationView->setDecorationVisible(false);

	QWidget *container = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout(container);
	layout->addWidget(mSimulationView);
	layout->addWidget(new QLabel("CMES SAMPLE"));
	layout->setStretch(0, 1);
	layout->setStretch(1, 0);
	setCentralWidget(container);
	//mSimulationView->setWindowFlags(mSimulationView->windowFlags() | Qt::Tool);
	//mSimulationView->show();

	connect(mSimulationView, &WbSimulationView::requestSavePerspective, this, &MainWindow::savePerspective);
	connect(mSimulationView, &WbSimulationView::requestRestorePerspective, this, &MainWindow::restorePerspective);

#ifndef QT_NO_SESSIONMANAGER
	QGuiApplication::setFallbackSessionManagementEnabled(false);
	connect(qApp, &QGuiApplication::commitDataRequest,
		this, &MainWindow::commitData);
#endif

	setCurrentFile(QString());
	setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setupUi()
{
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        setCurrentFile(QString());
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

void MainWindow::documentWasModified()
{
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    QAction *saveAsAct = fileMenu->addAction(tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
#ifndef QT_NO_CLIPBOARD
    QAction *cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    QAction *copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    QAction *pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
#endif // !QT_NO_CLIPBOARD
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::maybeSave()
{
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
    }
}
#endif

void MainWindow::updateBeforeWorldLoading(bool reloading)
{
	savePerspective(reloading, true);
	mSimulationView->view3D()->logWrenStatistics();
	mSimulationView->prepareWorldLoading();
}

void MainWindow::updateAfterWorldLoading(bool reloading, bool firstLoad)
{
	const WbWorld *world = WbWorld::instance();

	mSimulationView->setWorld(WbSimulationWorld::instance());

	// update 'view' menu
	const WbPerspective *perspective = world->perspective();
	WbActionManager::instance()->action(WbActionManager::DISABLE_SELECTION)->setChecked(perspective->isSelectionDisabled());
	WbActionManager::instance()->action(WbActionManager::LOCK_VIEWPOINT)->setChecked(perspective->isViewpointLocked());

	WbRenderingDeviceWindowFactory::reset();
	restorePerspective(reloading, firstLoad, false);

	foreach(WbRobot *const robot, WbControlledWorld::instance()->robots())
		handleNewRobotInsertion(robot);

	connect(world, &WbWorld::robotAdded, this, &MainWindow::handleNewRobotInsertion);

	if(!reloading)
		WbActionManager::instance()->resetApplicationActionsState();
	// reset focus widget used to identify the actions target widget
	WbActionManager::instance()->setFocusObject(mSimulationView->view3D());
}

void MainWindow::savePerspective(bool reloading, bool saveToFile)
{
	bool savingIsAllowed = true;

	if(!qgetenv("WEBOTS_DISABLE_SAVE_PERSPECTIVE_ON_CLOSE").isEmpty())
		savingIsAllowed = false;

	if(!savingIsAllowed && saveToFile)
		return;

	const WbWorld *world = WbWorld::instance();
	if(!world || world->isUnnamed() || WbFileUtil::isLocatedInInstallationDirectory(world->fileName()))
		return;

	WbPerspective *perspective = world->perspective();
	if(reloading) {
		// load previous settings
		// for example the perspectives of devices that have been deleted since the
		// last world save have to be loaded from the existing perspective file
		perspective->load(true);
		perspective->clearEnabledOptionalRenderings();
		perspective->clearRenderingDevicesPerspectiveList();
	}
	perspective->setMainWindowState(saveState());
	perspective->setCentralWidgetVisible(mSimulationView->isVisible());
	perspective->setSimulationViewState(mSimulationView->saveState());
	perspective->setDocumentationBook("");
	perspective->setDocumentationPage("");
	perspective->setOrthographicViewHeight(world->orthographicViewHeight());

	QStringList robotWindowNodeNames;
	perspective->setRobotWindowNodeNames(robotWindowNodeNames);

	QStringList centerOfMassEnabledNodeNames, centerOfBuoyancyEnabledNodeNames, supportPolygonEnabledNodeNames;
	world->retrieveNodeNamesWithOptionalRendering(centerOfMassEnabledNodeNames, centerOfBuoyancyEnabledNodeNames,
		supportPolygonEnabledNodeNames);
	perspective->setEnabledOptionalRendering(centerOfMassEnabledNodeNames, centerOfBuoyancyEnabledNodeNames,
		supportPolygonEnabledNodeNames);

	// save rendering devices perspective
	const QList<WbRenderingDevice *> renderingDevices = WbRenderingDevice::renderingDevices();
	foreach(const WbRenderingDevice *device, renderingDevices)
	{
		if(device->overlay() != NULL)
			perspective->setRenderingDevicePerspective(device->computeShortUniqueName(), device->perspective());
	}

	// save rendering devices perspective of external window
	WbRenderingDeviceWindowFactory::instance()->saveWindowsPerspective(*perspective);

	// save our new perspective in the file
	if(savingIsAllowed && saveToFile)
		perspective->save();
}

void MainWindow::restorePerspective(bool reloading, bool firstLoad, bool loadingFromMemory)
{
	WbWorld *world = WbWorld::instance();
	WbPerspective *perspective = world->perspective();
	bool meansOfLoading = false;
	if(loadingFromMemory)
		meansOfLoading = true;
	else {
		meansOfLoading = world->reloadPerspective();
		perspective = world->perspective();
	}

	if(meansOfLoading)
	{
		restoreState(perspective->mainWindowState());
		const int id = perspective->maximizedDockId();
		if(!reloading)
		{
			mSimulationView->setVisible(perspective->centralWidgetVisible());
			mSimulationView->restoreState(perspective->simulationViewState(), firstLoad);
		}
	}
	else if(firstLoad)
		// set default simulation view perspective
		mSimulationView->restoreFactoryLayout();

	const double ovh = perspective->orthographicViewHeight();
	world->setOrthographicViewHeight(ovh);

	mSimulationView->view3D()->restoreOptionalRendering(perspective->enabledCenterOfMassNodeNames(),
		perspective->enabledCenterOfBuoyancyNodeNames(),
		perspective->enabledSupportPolygonNodeNames());

	if(firstLoad)
		connect(mSimulationView->view3D(), &WbView3D::resized, this, &MainWindow::restoreRenderingDevicesPerspective);
	else
		restoreRenderingDevicesPerspective();

	// Refreshing
	mSimulationView->repaintView3D();
}

void MainWindow::restoreRenderingDevicesPerspective()
{
	const WbPerspective *perspective = WbWorld::instance()->perspective();
	const QList<WbRenderingDevice *> devices = WbRenderingDevice::renderingDevices();
	for(int i = 0; i < devices.size(); ++i)
	{
		WbRenderingDevice *device = devices[i];
		QStringList devicePerspective = perspective->renderingDevicePerspective(device->computeShortUniqueName());
		if(!devicePerspective.isEmpty())
			device->restorePerspective(devicePerspective);
	}
	disconnect(mSimulationView->view3D(), &WbView3D::resized, this, &MainWindow::restoreRenderingDevicesPerspective);
}

void MainWindow::createWorldLoadingProgressDialog()
{
	if(mWorldLoadingProgressDialog)
		return;

	if(isMinimized())
		return;

	mWorldLoadingProgressDialog = new QProgressDialog(tr("Opening world file"), tr("Cancel"), 0, 101, this);
	mWorldLoadingProgressDialog->setModal(true);
	mWorldLoadingProgressDialog->setAutoClose(false);
	mWorldLoadingProgressDialog->show();
	mWorldLoadingProgressDialog->setValue(0);
	mWorldLoadingProgressDialog->setWindowTitle(tr("Loading world"));
	connect(mWorldLoadingProgressDialog, &QProgressDialog::canceled, WbApplication::instance(),
		&WbApplication::setWorldLoadingCanceled);

	QApplication::processEvents();
}

void MainWindow::deleteWorldLoadingProgressDialog()
{
	if(mWorldLoadingProgressDialog)
	{
		disconnect(mWorldLoadingProgressDialog, &QProgressDialog::canceled, WbApplication::instance(),
			&WbApplication::setWorldLoadingCanceled);
		delete mWorldLoadingProgressDialog;
		mWorldLoadingProgressDialog = NULL;
	}
}

void MainWindow::setWorldLoadingProgress(const int progress)
{
	if(mWorldLoadingProgressDialog)
	{
		mWorldLoadingProgressDialog->setValue(progress);

		QApplication::processEvents();
	}
}

void MainWindow::setWorldLoadingStatus(const QString &status)
{
	if(mWorldLoadingProgressDialog)
	{
		mWorldLoadingProgressDialog->setLabelText(status);

		QApplication::processEvents();
	}
}

void MainWindow::simulationEnabledChanged(bool e)
{
}

void MainWindow::reloadWorld()
{
}

void MainWindow::resetWorld()
{
}

void MainWindow::openFileInTextEditor(const QString &fileName)
{
}

void MainWindow::handleNewRobotInsertion(WbRobot *robot)
{
	if(robot->isShowWindowFieldEnabled())
	{
		if(robot->windowFile().isEmpty())
			robot->showWindow();
	}
}
