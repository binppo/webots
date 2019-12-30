// Copyright 1996-2019 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "WbDocumentation.hpp"

#include "WbClipboard.hpp"
#include "WbDesktopServices.hpp"
#include "WbStandardPaths.hpp"

#include <QtCore/QUrl>

#include <cassert>

#include <QtGui/QContextMenuEvent>
#include <QtWidgets/QMenu>

#include <QtWebEngineWidgets/QWebEngineView>

/*
// Debug code: uncomment to show a web inspector for QtWebKit.
#include <QtWebKitWidgets/QWebInspector>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
*/

class DocumentationWebView : public QWebEngineView {
public:
  explicit DocumentationWebView(QWidget *parent = NULL) : QWebEngineView(parent) { setObjectName("DocumentationWebView"); }

protected:
#ifdef _WIN32
  // QWebEngineView::focusInEvent is not working: https://bugreports.qt.io/browse/QTBUG-67852
  // Meanwhile, QWebEnginePage::Copy action is used.
  virtual void focusInEvent(QFocusEvent *event) override {
    // update application actions
    WbActionManager *actionManager = WbActionManager::instance();
    actionManager->enableTextEditActions(false);
    actionManager->setFocusObject(this);
    actionManager->setEnabled(WbActionManager::COPY, false);
    actionManager->setEnabled(WbActionManager::CUT, false);
    actionManager->setEnabled(WbActionManager::PASTE, false);
    actionManager->setEnabled(WbActionManager::SELECT_ALL, false);
    actionManager->setEnabled(WbActionManager::UNDO, false);
    actionManager->setEnabled(WbActionManager::REDO, false);
    actionManager->setEnabled(WbActionManager::FIND, false);
    actionManager->setEnabled(WbActionManager::FIND_NEXT, false);
    actionManager->setEnabled(WbActionManager::FIND_PREVIOUS, false);
  }
#endif

  virtual void contextMenuEvent(QContextMenuEvent *event) override {
    QMenu menu(this);
    menu.addAction(pageAction(QWebEnginePage::Back));
    menu.addAction(pageAction(QWebEnginePage::Forward));
    menu.addSeparator();
    menu.addAction(pageAction(QWebEnginePage::Copy));
    menu.exec(event->globalPos());
  }

  // Open the external links (for example `<a href="https://www.w3schools.com" target="_blank">`) in the system browser.
  virtual QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override {
    // create a dummy view until the url is actually set, and remove it later.
    QWebEngineView *view = new QWebEngineView(this);
    connect(view->page(), &QWebEnginePage::urlChanged, qobject_cast<WbDocumentation *>(parent()),
            &WbDocumentation::openUrlInSystemBrowser);
    connect(view->page(), &QWebEnginePage::urlChanged, view, &QObject::deleteLater);
    return view;
  }
};

static const QStringList cBooks = QStringList() << "guide"
                                                << "Webots user guide"
                                                << "reference"
                                                << "Webots reference manual"
                                                << "automobile"
                                                << "Webots for automobiles";

WbDocumentation *WbDocumentation::cInstance = NULL;

WbDocumentation::WbDocumentation(QWidget *parent) : WbDockWidget(parent), mWebView(NULL) {
  assert(cInstance == NULL);
  cInstance = this;

  setWindowTitle("Documentation");
  setTabbedTitle("Documentation");
  setObjectName("Documentation");

  mWebView = new DocumentationWebView(this);
  setWidget(mWebView);
}

WbDocumentation::~WbDocumentation() {
  cInstance = NULL;
  delete mWebView;
}

void WbDocumentation::open(const QString &book, const QString &page, bool visible) {
  /*
  // Debug code: uncomment to show a web inspector for QtWebKit.
  static QWebInspector *inspector = NULL;
  if (inspector == NULL) {
    mWebView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    inspector = new QWebInspector;
    inspector->setPage(mWebView->page());

    QDialog *dialog = new QDialog();
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(inspector);
    dialog->show();
  }
  */

  QUrl url = QUrl::fromLocalFile(WbStandardPaths::webotsHomePath() + "docs/index.html");
  url.setUrl(QString("%1?url=&book=%2&page=%3").arg(url.toString()).arg(book).arg(page));
  mWebView->load(url);

  for (int i = 0; i < cBooks.size(); i += 2)
    if (cBooks[i] == book) {
      setWindowTitle(cBooks[i + 1]);
      break;
    }
  if (visible)
    show();
  else
    hide();
}

const QString WbDocumentation::book() const {
  const QString url = mWebView->url().toString();
  int start = url.lastIndexOf("&book=") + 6;
  int length = url.lastIndexOf("&page=") - start;
  return url.mid(start, length);
}

const QString WbDocumentation::page() const {
  const QString url = mWebView->url().toString();
  return url.mid(url.lastIndexOf("&page=") + 6);
}

void WbDocumentation::updateCopyAction() {
  WbActionManager::instance()->setEnabled(WbActionManager::COPY, !mWebView->selectedText().isEmpty());
}

void WbDocumentation::handleUserCommand(WbActionManager::WbActionKind actionKind) {
  if (actionKind == WbActionManager::COPY) {
    const QString selectedText = mWebView->selectedText();
    if (!selectedText.isEmpty())
      WbClipboard::instance()->setString(selectedText);
  }
}

void WbDocumentation::openUrlInSystemBrowser(const QUrl &url) {
  WbDesktopServices::openUrl(url.toString());
}
