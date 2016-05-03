/*
 * #%L
 * OME-QTWIDGETS C++ library for display of OME-Files pixel data and metadata.
 * %%
 * Copyright © 2014 - 2015 Open Microscopy Environment:
 *   - Massachusetts Institute of Technology
 *   - National Institutes of Health
 *   - University of Dundee
 *   - Board of Regents of the University of Wisconsin-Madison
 *   - Glencoe Software, Inc.
 * %%
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of any organization.
 * #L%
 */

#include <cstdlib>
#include <iostream>

#include <ome/qtwidgets/GLWindow.h>

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLDebugLogger>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

namespace ome
{
  namespace qtwidgets
  {

    GLWindow::GLWindow(QWindow *parent):
      QWindow(parent),
      update_pending(false),
      animating(false),
      glcontext(0),
      device(0),
      logger(0)
    {
      setSurfaceType(QWindow::OpenGLSurface);
    }

    GLWindow::~GLWindow()
    {
      if (logger)
        logger->stopLogging();
      delete device;
    }

    void
    GLWindow::render(QPainter *painter)
    {
      Q_UNUSED(painter);
    }

    void
    GLWindow::initialize()
    {
    }

    void
    GLWindow::resize()
    {
    }

    void
    GLWindow::render()
    {
      if (!device)
        device = new QOpenGLPaintDevice;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      device->setSize(size());

      QPainter painter(device);
      render(&painter);
    }

    void
    GLWindow::renderLater()
    {
      if (!update_pending) {
        update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
      }
    }

    bool
    GLWindow::event(QEvent *event)
    {
      switch (event->type()) {
      case QEvent::UpdateRequest:
        update_pending = false;
        renderNow();
        return true;
      default:
        return QWindow::event(event);
      }
    }

    void
    GLWindow::exposeEvent(QExposeEvent *event)
    {
      Q_UNUSED(event);

      if (isExposed())
        renderNow();
    }

    void GLWindow::resizeEvent(QResizeEvent * /* event */)
    {
      if (glcontext)
        resize();
    }

    QOpenGLContext *
    GLWindow::context() const
    {
      return glcontext;
    }

    void
    GLWindow::makeCurrent()
    {
      if (glcontext)
        glcontext->makeCurrent(this);
    }

    void GLWindow::renderNow()
    {
      if (!isExposed())
        return;

      bool needsInitialize = false;
      bool enableDebug = false;

      if (std::getenv("OME_QTWIDGETS_OPENGL_DEBUG"))
        enableDebug = true;

      if (!glcontext) {
        QSurfaceFormat format = requestedFormat();
        // OpenGL 3.3 core profile with debugging.
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
        if (enableDebug)
          {
            format.setOption(QSurfaceFormat::DebugContext);
          }
        format.setSamples(8);
        format.setDepthBufferSize(24);

        glcontext = new QOpenGLContext(this);
        glcontext->setFormat(format);
        bool valid = glcontext->create();
        std::cerr << "Valid OpenGL context: " << valid << std::endl;
        makeCurrent();

        if (enableDebug)
          {
            logger = new QOpenGLDebugLogger(this);
            connect(logger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
                    this, SLOT(logMessage(QOpenGLDebugMessage)),
                    Qt::DirectConnection);
            if (logger->initialize())
              {
                logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
                logger->enableMessages();
              }
          }

        needsInitialize = true;
      }

      makeCurrent();

      if (needsInitialize)
        {
          initializeOpenGLFunctions();
          initialize();
        }

      render();

      glcontext->swapBuffers(this);

      if (animating)
        renderLater();
    }

    void
    GLWindow::setAnimating(bool animating)
    {
      this->animating = animating;

      if (this->animating)
        renderLater();
    }

    void
    GLWindow::logMessage(QOpenGLDebugMessage message)
    {
      std::cerr << message.message().toStdString();
    }

  }
}
