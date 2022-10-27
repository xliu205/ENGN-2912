//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2021-11-14 21:46:10 taubin>
//------------------------------------------------------------------------
//
// WrlToolsWidget.cpp
//
// Software developed for the Fall 2021 Brown University course
// ENGN 2912B Scientific Computing in C++
// Copyright (c) 2021, Gabriel Taubin
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

#include <iostream>
#include "WrlToolsWidget.hpp"
#include "WrlMainWindow.hpp"
#include "wrl/SceneGraphProcessor.hpp"

#ifdef _WIN32
int WrlToolsWidget::_lDPI = 96;
#endif

//////////////////////////////////////////////////////////////////////
WrlToolsWidget::WrlToolsWidget(QWidget* parent):
  QWidget(),
  _mainWindow() {
  (void) parent;
  setupUi(this);
  const QObjectList & list = this->children();
  for(int i=0; i<list.size(); ++i) {
    QObject* obj = list.at(i);
    if(obj->isWidgetType()) {
      QWidget* child = static_cast<QWidget*>(obj);
      int childHeight = child->height();
#ifdef _WIN32
      if(_lDPI>96) {
        if(_lDPI>=192)
          childHeight = 2*childHeight;
        else if(_lDPI>=144)
          childHeight = 3*childHeight/2;
        child->setMinimumHeight(childHeight);
        child->setMaximumHeight(childHeight);
      }
#endif
      cout << qPrintable(child->objectName()) << "->height() = " << childHeight << endl;
    }
  }
}

//////////////////////////////////////////////////////////////////////
WrlToolsWidget::~WrlToolsWidget() {
}

//////////////////////////////////////////////////////////////////////
void WrlToolsWidget::setMainWindow(WrlMainWindow *mw) {
  _mainWindow = mw;
}

//////////////////////////////////////////////////////////////////////
void WrlToolsWidget::updateState() {
  edit3DCanvasWidth->setText
    ("  "+QString::number(_mainWindow->getGLWidgetWidth()));
  edit3DCanvasHeight->setText
    ("  "+QString::number(_mainWindow->getGLWidgetHeight()));

  WrlViewerData& data  = _mainWindow->getData();
  
  SceneGraph* wrl = data.getSceneGraph();
  if(wrl==(SceneGraph*)0) {

    pushButtonSceneGraphNormalNone->setEnabled(false);
    pushButtonSceneGraphNormalPerVertex->setEnabled(false);
    pushButtonSceneGraphNormalPerFace->setEnabled(false);
    pushButtonSceneGraphNormalPerCorner->setEnabled(false);
    pushButtonSceneGraphNormalInvert->setEnabled(false);

  } else /* if(wrl!=(SceneGraph*)0) */ {

    SceneGraphProcessor processor(*wrl);

    bool hasNormal = false;
    bool hasFaces  = processor.hasIndexedFaceSetFaces();
    bool value     = processor.hasIndexedFaceSetNormalNone();
    pushButtonSceneGraphNormalNone->setEnabled(!value);
    value = processor.hasIndexedFaceSetNormalPerVertex();
    hasNormal |= value;
    pushButtonSceneGraphNormalPerVertex->setEnabled(hasFaces && !value);
    value = processor.hasIndexedFaceSetNormalPerFace();
    hasNormal |= value;
    pushButtonSceneGraphNormalPerFace->setEnabled(hasFaces && !value);
    value = processor.hasIndexedFaceSetNormalPerCorner();
    hasNormal |= value;
    pushButtonSceneGraphNormalPerCorner->setEnabled(hasFaces && !value);
    pushButtonSceneGraphNormalInvert->setEnabled(hasNormal);
  }
}


void WrlToolsWidget::on_pushButtonSceneGraphNormalInvert_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.normalInvert();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalNone_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.normalClear();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalPerVertex_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.computeNormalPerVertex();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalPerFace_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.computeNormalPerFace();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalPerCorner_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.computeNormalPerCorner();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

