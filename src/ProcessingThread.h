/************************************************************************/
/* qt-opencv-multithreaded:                                             */
/* A multithreaded OpenCV application using the Qt framework.           */
/*                                                                      */
/* ProcessingThead.h                                                    */
/*                                                                      */
/* Nick D'Ademo <nickdademo@gmail.com>                                  */
/*                                                                      */
/* Copyright (c) 2012-2015 Nick D'Ademo                                 */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* The above copyright notice and this permission notice shall be       */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  */
/* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   */
/* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     */
/* SOFTWARE.                                                            */
/*                                                                      */
/************************************************************************/

#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QTime>
#include <QtCore/QQueue>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Structures.h"
#include "Config.h"
#include "Buffer.h"
#include "MatToQImage.h"
#include "SharedImageBuffer.h"

using namespace cv;

class ProcessingThread : public QThread
{
    Q_OBJECT

    public:
        ProcessingThread(SharedImageBuffer *sharedImageBuffer, int deviceNumber);
        QRect getCurrentROI();
        void stop();

    private:
        void updateFPS(int);
        void setROI();
        void resetROI();
        SharedImageBuffer *m_sharedImageBuffer;
        Mat m_currentFrame;
        Mat m_currentFrameGrayscale;
        Rect m_currentROI;
        QImage m_frame;
        QTime m_t;
        QQueue<int> m_fps;
        QMutex m_doStopMutex;
        QMutex m_processingMutex;
        Size m_frameSize;
        Point m_framePoint;
        struct ImageProcessingFlags m_imgProcFlags;
        struct ImageProcessingSettings m_imgProcSettings;
        struct ThreadStatisticsData m_statsData;
        volatile bool m_doStop;
        int m_processingTime;
        int m_fpsSum;
        int m_sampleNumber;
        int m_deviceNumber;
        bool m_enableFrameProcessing;

    protected:
        void run();

    private slots:
        void updateImageProcessingFlags(struct ImageProcessingFlags);
        void updateImageProcessingSettings(struct ImageProcessingSettings);
        void setROI(QRect roi);

    signals:
        void newFrame(const QImage &frame);
        void updateStatisticsInGUI(struct ThreadStatisticsData);
};

#endif // PROCESSINGTHREAD_H
