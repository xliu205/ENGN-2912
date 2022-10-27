//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 23:05:01 taubin>
//------------------------------------------------------------------------
//
// IfsGLWidget.cpp
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
#include <string.h>
#include <math.h>

#include <QPainter>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QCoreApplication>

#include "IfsMainWindow.hpp"
#include "QtLogo.hpp"
#include "IfsGLBuffer.hpp"

#ifdef near
# undef near
#endif
#ifdef far
# undef far
#endif

int   IfsGLWidget::_borderUp         =     20;
int   IfsGLWidget::_borderDown       =     20;
int   IfsGLWidget::_borderLeft       =     20;
int   IfsGLWidget::_borderRight      =     20;
float IfsGLWidget::_angleHomeX       =  10.0f; // 0.0f;
float IfsGLWidget::_angleHomeY       =  10.0f; // 0.0f;
float IfsGLWidget::_angleHomeZ       =   0.00f;

// void printQMatrix4x4(const string& name, const QMatrix4x4& M) {
//   string str;
//   static char cstr[128];
//   std::cout << name << " = [\n";
//   sprintf(cstr,"%10.4f %10.4f %10.4f %10.4f", M(0,0),M(0,1),M(0,2),M(0,3));
//   str = cstr; cout << cstr << "\n";
//   sprintf(cstr,"%10.4f %10.4f %10.4f %10.4f", M(1,0),M(1,1),M(1,2),M(1,3));
//   str = cstr; cout << cstr << "\n";
//   sprintf(cstr,"%10.4f %10.4f %10.4f %10.4f", M(2,0),M(2,1),M(2,2),M(2,3));
//   str = cstr; cout << cstr << "\n";
//   sprintf(cstr,"%10.4f %10.4f %10.4f %10.4f", M(3,0),M(3,1),M(3,2),M(3,3));
//   str = cstr; cout << cstr << "\n";
//   std::cout << "]\n";
// }

//////////////////////////////////////////////////////////////////////
IfsGLWidget::IfsGLWidget
(IfsMainWindow *mw, const QColor &background, const QColor &material):
  _mainWindow(mw),
  _bbox((BBox*)0),
  _eye(0,0,0),
  _center(0,0,1),
  _up(0,1,0),
  _mouseZone(-1),
  _mouseInside(false),
  _mousePressed(true),
  _buttons(),
  _prevMouseX(0),
  _prevMouseY(0),
  _zone4enabled(true),
  _translateStep(0.010f),
  _cameraTranslation(0,0,0),
  _animationOn(true),
  _fAngle(0),
  _shader((IfsGLShader*)0),
  _background(background),
  _material(material),
  _lightSource(0.0, 0.3, -1.0) {

  setMinimumSize(400,400);
  _data.setInitialized(false);
  _data.setIfs((Ifs*)0);
  _viewRotation.setToIdentity();
  _projectionMatrix.setToIdentity();
  setMouseTracking(true);
}

//////////////////////////////////////////////////////////////////////
IfsGLWidget::~IfsGLWidget() {
  makeCurrent();
  delete _shader;
  delete _handles;
  doneCurrent();
}

//////////////////////////////////////////////////////////////////////
Ifs* IfsGLWidget::getIfs() {
  return _data.getIfs();
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::setIfs(Ifs* pIfs, QColor& materialColor) {
  _data.setIfs(pIfs);
  if(_data.getIfs()!=(Ifs*)0) {
    vector<float>& coord = pIfs->getCoord();
    _bbox = new BBox(3,coord,true);

    IfsGLBuffer* ifsb = new IfsGLBuffer(pIfs, materialColor);
    QOpenGLBuffer* vbo = _shader->getVertexBuffer();
    _shader->setVertexBuffer(ifsb);
    if(vbo) { vbo->destroy(); delete vbo; }

    float diameter = 2.0f;
    if(_bbox!=(BBox*)0) {
      _center.setX(_bbox->getCenter(0));
      _center.setY(_bbox->getCenter(1));
      _center.setZ(_bbox->getCenter(2));
      diameter = _bbox->getDiameter();
    } else {
      _center.setX(0);
      _center.setY(0);
      _center.setZ(0);
    }

    _translateStep = 0.001f*diameter;

    _setHomeView(false);
    _setProjectionMatrix();
  }
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::setQtLogo() {
  QtLogo* qtLogo = new QtLogo();;
  setIfs(qtLogo, _material);
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::invertNormal() {
  Ifs* pIfs = _data.getIfs();
  if(pIfs!=(Ifs*)0) {
    vector<float> &normal = pIfs->getNormal();    
    float n0,n1,n2;
    for(unsigned i=0;i<normal.size();i+=3) {
      n0 = normal[i+0]; n1 = normal[i+1]; n2 = normal[i+2];
      normal[i+0] = -n0; normal[i+1] = -n1; normal[i+2] = -n2;
    }
    // this is very inefficient
    IfsGLBuffer* ifsb = new IfsGLBuffer(pIfs, _material);
    QOpenGLBuffer* vbo = _shader->getVertexBuffer();
    _shader->setVertexBuffer(ifsb);
    if(vbo) { vbo->destroy(); delete vbo; }
  }
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  _mainWindow->timerStart();
  _animationOn = false;
  _mousePressed = true;
  _mainWindow->timerStop();
  _shader  = new IfsGLShader(_material,_lightSource);
  _handles = new IfsGLHandles(); // TODO !!!
  QtLogo* qtLogo = new QtLogo();;
  setIfs(qtLogo, _material);
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::paintGL() {

  QPainter painter;
  painter.begin(this);
  painter.beginNativePainting();

  glClearColor(_background.redF(), _background.greenF(), _background.blueF(),1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // paint

  glFrontFace(GL_CW);
  glCullFace(GL_FRONT);
  // glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  QMatrix4x4& mvp = _shader->getMVPMatrix();

  QMatrix4x4 cameraTranslationMatrix;
  cameraTranslationMatrix.setToIdentity();
  cameraTranslationMatrix.translate(_cameraTranslation);

  // void QMatrix4x4::lookAt
  //   (const QVector3D & eye, const QVector3D & center, const QVector3D & up);
  //
  // Multiplies this matrix by a viewing matrix derived from an eye
  // point. The center value indicates the center of the view that the
  // eye is looking at. The up value indicates which direction should
  // be considered up with respect to the eye.

  QMatrix4x4 viewMatrix;
  viewMatrix = cameraTranslationMatrix;
  viewMatrix.lookAt(_eye,_center,_up);

  mvp = _projectionMatrix * viewMatrix;

  mvp.translate(_center.x(),_center.y(),_center.z());
  mvp.rotate(    _fAngle, 0.0f, 1.0f, 0.0f);
  mvp *= _viewRotation;
  mvp.translate(-_center.x(),-_center.y(),-_center.z());

  _shader->paint(*this);

  glDisable(GL_DEPTH_TEST);
  // glDisable(GL_CULL_FACE);

  if(_animationOn) _fAngle += 0.25f;

  // paint mouse handles

  float w    =                    (float)width();
  float h    =                   (float)height();
  float x0   =                              0.0f;
  float x1   =            ((float)_borderLeft)/w;
  float x2   = ((float)(width()-_borderRight))/w;
  float x3   =                              1.0f;
  float y0   =                              1.0f;
  float y1   =   ((float)(height()-_borderUp))/h;
  float y2   =            ((float)_borderDown)/h;
  float y3   =                              0.0f;
  
  float hx0=0.0f,hx1=0.0f,hy0=0.0f,hy1=0.0f;
  if(_mouseZone==0) {
    hx0 = x0; hy0 = y0; hx1 = x1; hy1 = y1;
  } else if(_mouseZone==1) {
    hx0 = x1; hy0 = y0; hx1 = x2; hy1 = y1;
  } else if(_mouseZone==2) {
    hx0 = x2; hy0 = y0; hx1 = x3; hy1 = y1;
  } else if(_mouseZone==3) {
    hx0 = x0; hy0 = y1; hx1 = x1; hy1 = y2;
  } else if(_mouseZone==4) {
    // don't paint anything for the center region
    // hx0 = x1; hy0 = y1; hx1 = x2; hy1 = y2;
  } else if(_mouseZone==5) {
    hx0 = x2; hy0 = y1; hx1 = x3; hy1 = y2;
  } else if(_mouseZone==6) {
    hx0 = x0; hy0 = y2; hx1 = x1; hy1 = y3;
  } else if(_mouseZone==7) {
    hx0 = x1; hy0 = y2; hx1 = x2; hy1 = y3;
  } else if(_mouseZone==8) {
    hx0 = x2; hy0 = y2; hx1 = x3; hy1 = y3;
  }
  
  if(hx0!=hx1 && hy0!=hy1) {
    QColor colorHandles;
    colorHandles.setRgbF(0.6f,0.6f,0.9f);
    _handles->setColor(colorHandles);
    QMatrix4x4& handlesMatrix = _handles->getMatrix();
    handlesMatrix.setToIdentity();
    handlesMatrix.ortho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);
    _handles->setGeometry(hx0, hy0, hx1, hy1);
    _handles->paint(*this);
  }

  painter.endNativePainting();
  painter.end();
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::resizeGL(int /* w */, int /* h */) {
  _setProjectionMatrix();
  // TODO : create the mouse handles geometry and shaders here
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::enterEvent(QEvent* /*event*/) {
  _mouseInside = true;
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::leaveEvent(QEvent* /*event*/) {
  _mouseInside = false;
  _mouseZone = -1;
  _mainWindow->showStatusBarMessage("");
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::mousePressEvent(QMouseEvent * event) {
  _mousePressed = true;
  _mainWindow->timerStop();
  _buttons = event->buttons();
  _prevMouseX = event->x();
  _prevMouseY = event->y();
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::mouseReleaseEvent(QMouseEvent* /*event*/) {
  switch(_mouseZone) {
  case 0:
    _setHomeView(false);
    break;
  case 2:
    invertNormal();
    break;
  case 6:
    _mainWindow->showStatusBarMessage("Stoping Animation");
    _mainWindow->timerStop();
    _animationOn = false;
    break;
  case 8:
    _mainWindow->showStatusBarMessage("Restarting Animation");
    _mainWindow->timerStart();
    _animationOn = true;
    break;
  default:
    break;
  }
  _mainWindow->showStatusBarMessage("");
  _mousePressed = false;
  _mainWindow->timerStart();
  // _buttons = 0x0;
  update();
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::mouseMoveEvent(QMouseEvent * event) {

  if(_mouseInside==false) return;

  int x      = event->x();
  int y      = event->y();
  int codeX  = (x<_borderLeft)?0:(x>=width() -_borderRight)?2:1;
  int codeY  = (y<_borderUp  )?0:(y>=height()-_borderDown )?2:1;
  _mouseZone = codeX+3*codeY;

  if(_mousePressed) {

    int   dx     = (x-_prevMouseX); _prevMouseX = x;
    int   dy     = (_prevMouseY-y); _prevMouseY = y;

    float fdx    = ((float)dx/(float)width() );
    float thetaY =  180.0f*fdx;
    float fdy    = ((float)dy/(float)height());
    float thetaX = -180.0f*fdy;

    float ux = -fdy;
    float uy =  fdx;
    float uz = 0.0f;
    float uu = fdx*fdx+fdy*fdy;
    if(uu>0.0f) { uu = sqrt(uu); }
    ux /= uu; uy /= uu;
    float thetaXY = 180.0f*uu;

    float dxT = -0.5f * dx * _translateStep;
    float dyT = -0.5f * dy * _translateStep;
    float dzT = -8.0f * dy * _translateStep;

    Qt::MouseButtons buttons = event->buttons();

    QVector3D  translation;
    QMatrix4x4 sceneRotation;
    QMatrix4x4 lightRotation;

    if(buttons & Qt::LeftButton) {

      _mainWindow->showStatusBarMessage("");
      switch(_mouseZone) {
      case 1:
        if(_zone4enabled) {
          _mainWindow->showStatusBarMessage("Rotating Light Source");
          lightRotation.rotate(thetaXY,ux,uy,uz);
        }
        break;
      case 3:
        _mainWindow->showStatusBarMessage("Rotating Object");
        sceneRotation.rotate(thetaX,1.0f,0.0f,0.0f);
        break;
      case 4:
        if(_zone4enabled) {
          _mainWindow->showStatusBarMessage("Rotating Object");
          sceneRotation.rotate(thetaXY,ux,uy,uz);
        }
        break;
      case 5:
        _mainWindow->showStatusBarMessage("Zooming");
        _zoom(dzT);
        break;
      case 7:
        _mainWindow->showStatusBarMessage("Rotating Object");
        sceneRotation.rotate(thetaY,0.0f,1.0f,0.0f);
        break;
      default:
        _mainWindow->showStatusBarMessage("Translating Object");
        break;
      }

    } else if(buttons & Qt::MidButton) {

    } else if(buttons & Qt::RightButton) {

      _mainWindow->showStatusBarMessage("");
      switch(_mouseZone) {
      case 3:
        _mainWindow->showStatusBarMessage("Translating Object");
        translation += QVector3D(0,-dyT,0);
        break;
      case 4:
        if(_zone4enabled) {
          _mainWindow->showStatusBarMessage("Rotating Light Source");
          lightRotation.rotate(thetaXY,ux,uy,uz);
        }
        break;
      case 7:
        _mainWindow->showStatusBarMessage("Translating Object");
        translation += QVector3D(-dxT,0,0);
        break;
      default:
        break;
      }
    }

    _viewRotation = sceneRotation * _viewRotation;
    _lightSource = lightRotation.map(_lightSource);
    _cameraTranslation += translation;

    update();

  } else {

    switch(_mouseZone) {
    case 0:
      _mainWindow->showStatusBarMessage
        ("HOME");
      break;
    case 1:
      _mainWindow->showStatusBarMessage
        ("");
      break;
    case 2:
      _mainWindow->showStatusBarMessage
        ("Invert Normals");
      break;
    case 3:
      _mainWindow->showStatusBarMessage
        ("Rotate Around Horizontal Axis | Translate Left-Right");
      break;
    case 4:
      _mainWindow->showStatusBarMessage
        ("Rotate Object With Respect To Center");
      break;
    case 5:
      _mainWindow->showStatusBarMessage
        ("Translate Object Along Viewing Direction");
      break;
    case 6:
      _mainWindow->showStatusBarMessage
        ("Stop Animation");
      break;
    case 7:
      _mainWindow->showStatusBarMessage
        ("Rotate Around Vertical Axis | Translate Up-Down");
      break;
    case 8:
      _mainWindow->showStatusBarMessage
        ("Resume Animation");
      break;
    default:
      _mainWindow->showStatusBarMessage
        ("");
      break;
    }

  }
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::_zoom(const float value) {
  QVector3D v(0.0f,0.0f,value);
  _eye += v;
  float bboxDiameter = _bbox->getDiameter();
  float eyeToCenterDistance = _eye.z()-_center.z();
  if(eyeToCenterDistance<0.25*bboxDiameter) {
    eyeToCenterDistance = 0.25*bboxDiameter;
    _eye.setZ(_center.z()+eyeToCenterDistance);
  }
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::_setHomeView(const bool identity) {

  if(identity) {
    _viewRotation.setToIdentity();
  } else {
    QMatrix4x4 Rx;
    Rx.rotate(_angleHomeX, 1.0f, 0.0f, 0.0f);
    QMatrix4x4 Ry;
    Ry.rotate(_angleHomeY, 0.0f, 1.0f, 0.0f);
    QMatrix4x4 Rz;
    Rz.rotate(_angleHomeZ, 0.0f, 0.0f, 1.0f);
    _viewRotation = Rx*Ry*Rz;
  }

  _fAngle = 0.0f;
  float eyeToCenterDistance = 5.0f*_bbox->getDiameter();
  _eye = _center+QVector3D(0,0,eyeToCenterDistance);
}

//////////////////////////////////////////////////////////////////////
void IfsGLWidget::_setProjectionMatrix() {

  float w             = (float)width();
  float h             = (float)height();
  float verticalAngle = 10.0f;
  float aspectRatio   = w/h;
  float bboxDiameter  = (_bbox)?_bbox->getDiameter():2.0f;
  float near          =   1.0f*bboxDiameter;
  float far           = 100.0f*bboxDiameter;

  // float scale   = (h<w)?h:w; scale = 50.0f*(bboxDiameter/scale);
  // float left    = scale*((w>h)?-1.0f: -w/h);
  // float right   = scale*((w>h)? 1.0f:  w/h);
  // float bottom  = scale*((w>h)? -h/w:-1.0f);
  // float top     = scale*((w>h)?  h/w: 1.0f);

  // void QMatrix4x4::frustum
  //   (float left, float right, float bottom, float top,
  //    float nearPlane, float farPlane)
  //
  // Multiplies this matrix by another that applies a perspective
  // frustum projection for a window with lower-left corner (left,
  // bottom), upper-right corner (right, top), and the specified
  // nearPlane and farPlane clipping planes.
  //
  // QMatrix4x4 frustumMatrix;
  // frustumMatrix.setToIdentity();
  // frustumMatrix.frustum(left, right, bottom, top, near, far);
  // printQMatrix4x4("frustum",frustumMatrix);

  // void QMatrix4x4::ortho
  //   (float left, float right, float bottom, float top,
  //    float nearPlane, float farPlane);
  //
  // Multiplies this matrix by another that applies an orthographic
  // projection for a window with lower-left corner (left, bottom),
  // upper-right corner (right, top), and the specified nearPlane and
  // farPlane clipping planes.
  //
  // QMatrix4x4 orthoMatrix;
  // orthoMatrix.setToIdentity();
  // orthoMatrix.ortho(left, right, bottom, top, near, far);
  // printQMatrix4x4("ortho",orthoMatrix);

  // void QMatrix4x4::perspective
  //   (float verticalAngle, float aspectRatio,
  //     float nearPlane, float farPlane);
  //
  // Multiplies this matrix by another that applies a perspective
  // projection. The vertical field of view will be verticalAngle
  // degrees within a window with a given aspectRatio that determines
  // the horizontal field of view. The projection will have the
  // specified nearPlane and farPlane clipping planes which are the
  // distances from the viewer to the corresponding planes.

  QMatrix4x4 perspectiveMatrix;
  perspectiveMatrix.setToIdentity();
  perspectiveMatrix.perspective(verticalAngle, aspectRatio, near, far);
  // printQMatrix4x4("perspective",perspectiveMatrix);

  // _projectionMatrix = orthoMatrix;
  // _projectionMatrix = frustumMatrix;
  _projectionMatrix = perspectiveMatrix;
}
