#ifndef ___FEITENG_VIDEOWIDGET_H
#define ___FEITENG_VIDEOWIDGET_H

#include <QWidget>
#include <QImage>

#include "VideoCoreThread.h"

namespace FEITENG
{
    class VideoWidget : public QWidget
    {
            Q_OBJECT

        private:
            VideoCoreThread* m_core_ptr;
            QImage m_image;

        public:
            explicit VideoWidget(QWidget* = nullptr);
            ~VideoWidget();

        protected:
            void paintEvent(QPaintEvent*) override;

        private slots:
            void updateImage(const QImage&);

        public slots:
            void play();
            void pause();
            void refresh();

            void setUrl(const QString&);
    };
};

#endif // ___FEITENG_VIDEOWIDGET_H
