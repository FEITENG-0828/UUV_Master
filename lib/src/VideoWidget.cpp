#include "VideoWidget.h"

#include <QPainter>

#include "VideoCoreThread.h"

namespace FEITENG
{
    VideoWidget::VideoWidget(QWidget* parent_ptr) : QWidget(parent_ptr),
        m_core_ptr(nullptr),
        m_image()
    {
        m_core_ptr = new VideoCoreThread(this);
        connect(m_core_ptr, &VideoCoreThread::sendLatestFrame, this, &VideoWidget::updateImage);
        connect(m_core_ptr, &VideoCoreThread::finished, m_core_ptr, &QObject::deleteLater);
        m_core_ptr->start();
    }

    VideoWidget::~VideoWidget()
    {
        m_core_ptr->stopRunning();
        m_core_ptr->quit();
        m_core_ptr->wait();
    }

    void VideoWidget::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(rect(), Qt::black);
        if(m_image.isNull())
        {
            return;
        }
        QImage scaled_image = m_image.scaled(size(), Qt::KeepAspectRatio);
        int pos_x = (width() - scaled_image.width()) / 2;
        int pos_y = (height() - scaled_image.height()) / 2;
        painter.drawImage(QPoint(pos_x, pos_y), scaled_image);
    }

    void VideoWidget::updateImage(const QImage& image)
    {
        if(image.isNull())
        {
            return;
        }
        m_image = image;
        update();
    }

    void VideoWidget::play()
    {
        m_core_ptr->proceed();
    }

    void VideoWidget::pause()
    {
        m_core_ptr->suspend();
    }

    void VideoWidget::refresh()
    {
        m_core_ptr->restart();
    }

    void VideoWidget::setUrl(const QString& url)
    {
        m_core_ptr->updateStreamAddr(url);
    }
} // namespace FEITENG
