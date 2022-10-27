//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 23:05:24 taubin>
//------------------------------------------------------------------------
//
// IfsMainWindow.cpp
//
// Software developed for the Fall 2021 Brown University course
// ENGN 2912B Scientific Computing in C++
// Copyright (c) 2015-2021, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// #include <iostream>

#include "IfsMainWindow.hpp"
#include "IfsGLWidget.hpp"
#include "IfsAboutDialog.hpp"

#include <QApplication>
#include <QMenuBar>
#include <QGroupBox>
#include <QStatusBar>
#include <QFileDialog>

#include <ifs/IfsLoaderWrl.hpp>
#include <ifs/IfsSaverWrl.hpp>

int IfsMainWindow::_timerInterval = 20;

//////////////////////////////////////////////////////////////////////
void IfsMainWindow::showStatusBarMessage(const QString & message) {
  _statusBar->showMessage(message);
}

//////////////////////////////////////////////////////////////////////
IfsMainWindow::IfsMainWindow() {

  setWindowIcon(QIcon("qt.icns"));

  IfsLoaderWrl* wrlLoader = new IfsLoaderWrl();
  _loader.add(wrlLoader);

  IfsSaverWrl* wrlSaver = new IfsSaverWrl();
  _saver.add(wrlSaver);

  QColor clearColor    = qRgb(75, 100, 150);
  QColor materialColor = qRgb(225, 150, 75);
  _glWidget =  new IfsGLWidget(this, clearColor, materialColor);
  QWidget * widget = new QWidget(this);
  setCentralWidget(widget);
  QGridLayout * layout = new QGridLayout(widget);
  layout->setContentsMargins(5,5,5,5);
  layout->addWidget(_glWidget,0,0,1,1);
  widget->setLayout(layout);

  _statusBar = new QStatusBar(this);
  QFont font;
  font.setPointSize(9);
  _statusBar->setFont(font);
  setStatusBar(_statusBar);

  QMenu *fileMenu = menuBar()->addMenu("&File");
  QAction *exit = new QAction("E&xit" , fileMenu);
  fileMenu->addAction(exit);
  connect(exit, SIGNAL(triggered(bool)),
          this, SLOT(onMenuFileExit()));
  
  QAction *load = new QAction("Load" , fileMenu);
  fileMenu->addAction(load);
  connect(load, SIGNAL(triggered(bool)),
          this, SLOT(onMenuFileLoad()));
  
  QAction *save = new QAction("Save" , fileMenu);
  fileMenu->addAction(save);
  connect(save, SIGNAL(triggered(bool)),
          this, SLOT(onMenuFileSave()));
  
  QAction *qtLogo = new QAction("Qt Logo" , fileMenu);
  fileMenu->addAction(qtLogo);
  connect(qtLogo, SIGNAL(triggered(bool)),
          _glWidget, SLOT(setQtLogo()));

  QMenu *helpMenu = menuBar()->addMenu("&Help");
  QAction *about_action = new QAction("About QtOpenGL",helpMenu);
  helpMenu->addAction(about_action);
  connect(about_action, SIGNAL(triggered(bool)),
                  this, SLOT(onMenuHelpAbout()));

  // for animation
  _timer = new QTimer(this);
  _timer->setInterval(_timerInterval);
  connect(_timer, SIGNAL(timeout()), _glWidget, SLOT(update()));

  showStatusBarMessage("");
}

//////////////////////////////////////////////////////////////////////
void IfsMainWindow::onMenuFileLoad() {

  std::string filename;

  // stop animation
  _timer->stop();

  QFileDialog fileDialog(this);
  fileDialog.setFileMode(QFileDialog::ExistingFile); // allowed to select only one 
  fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog.setNameFilter(tr("VRML Files (*.wrl)"));
  QStringList fileNames;
  if(fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles();
    if(fileNames.size()>0)
      filename = fileNames.at(0).toStdString();
  }

  if (filename.empty()) {
    showStatusBarMessage("load filename is empty");
  } else {

    static char str[1024];

    sprintf(str,"Loading \"%s\" ...",filename.c_str());
    showStatusBarMessage(QString(str));

    Ifs* pIfs = new Ifs();
    if(_loader.load(filename.c_str(),*pIfs)) { // if success

      // TODO
      // should read from file or get default Material.diffuseColor
      QColor materialColor(200,150,100);

      _glWidget->setIfs(pIfs,materialColor);

      sprintf(str,"Loadead \"%s\" nCoord=%d nFaces=%d",
              filename.c_str(),
              pIfs->getNumberOfCoord(),
              pIfs->getNumberOfFaces());
    } else {
      delete pIfs;
      sprintf(str,"Unable to load \"%s\"",filename.c_str());
    }

    showStatusBarMessage(QString(str));
  } 

  // restart animation
  _timer->start(_timerInterval);
}

//////////////////////////////////////////////////////////////////////
void IfsMainWindow::onMenuFileSave() {

  std::string filename;

  // stop animation
  _timer->stop();

  QFileDialog fileDialog(this);
  fileDialog.setFileMode(QFileDialog::AnyFile); // allowed to select only one 
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setNameFilter(tr("VRML Files (*.wrl)"));
  QStringList fileNames;
  if(fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles();
    if(fileNames.size()>0)
      filename = fileNames.at(0).toStdString();
  }

  // restart animation
  _timer->start(_timerInterval);

  if (filename.empty()) {
    showStatusBarMessage("save filename is empty");
  } else {

    static char str[1024];

    sprintf(str,"Saving \"%s\" ...",filename.c_str());
    showStatusBarMessage(QString(str));

    Ifs* pIfs = _glWidget->getIfs();

    if(_saver.save(filename.c_str(),*pIfs)) {
      sprintf(str,"Saved \"%s\" nCoord=%d nFaces=%d",
              filename.c_str(),
              pIfs->getNumberOfCoord(),
              pIfs->getNumberOfFaces());
    } else {
      sprintf(str,"Unable to save \"%s\"",filename.c_str());
    }

    showStatusBarMessage(QString(str));
  }
}

//////////////////////////////////////////////////////////////////////
void IfsMainWindow::onMenuHelpAbout() {
  IfsAboutDialog dialog(this);
  dialog.exec();
}

//////////////////////////////////////////////////////////////////////
void IfsMainWindow::onMenuFileExit() {
  close();
}
