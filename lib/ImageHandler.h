#include <QObject>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QColorSpace>
#include <QBuffer>
#include <QPushButton>
#include <QApplication>
#include <QFontDatabase>
#include <QPropertyAnimation>

#include <iostream>

#include "NetworkHandler.h"
#include "OperationsHandler.h"

#pragma once

class ImageHandler : public QObject
{
    Q_OBJECT
public:
    ImageHandler(QGridLayout* main_table, OperationsHandler* operations_handl, size_t session_id, const std::string& path);

public slots:
    void loadImage(NetworkHandler* network_handl, int idx);
    void update_text_desc(NetworkHandler* network_handl);

signals:

private:
    QImage convertToHighQualityJPEG(const QByteArray& pngData);
    QPixmap roundCorners(const QPixmap& input, int radius);
    
private:
    QGridLayout* main_table;
    NetworkHandler* network_handl;
    OperationsHandler* operations_handl;
    size_t session_id;

    QIcon plus_icon;
    QIcon minus_icon;
    QIcon plus_dark_icon;
    QIcon minus_dark_icon;

    std::string path;
};