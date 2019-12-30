#pragma once

#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
class QProgressDialog;
QT_END_NAMESPACE
class WbRobot;
class WbSimulationView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=nullptr);

    void loadFile(const QString &fileName);

	void savePerspective(bool reloading, bool saveToFile);
	void restorePerspective(bool reloading, bool firstLoad, bool loadingFromMemory);

public slots:
	void restoreRenderingDevicesPerspective();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif
	void updateBeforeWorldLoading(bool reloading);
	void updateAfterWorldLoading(bool reloading, bool firstLoad);
	void createWorldLoadingProgressDialog();
	void deleteWorldLoadingProgressDialog();
	void setWorldLoadingProgress(const int progress);
	void setWorldLoadingStatus(const QString &status);
	void simulationEnabledChanged(bool);
	void reloadWorld();
	void resetWorld();
	void openFileInTextEditor(const QString &);
	void handleNewRobotInsertion(WbRobot *robot);

private:
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
	void setupUi();

    QString curFile;
	WbSimulationView *mSimulationView;
	QProgressDialog *mWorldLoadingProgressDialog;
};
