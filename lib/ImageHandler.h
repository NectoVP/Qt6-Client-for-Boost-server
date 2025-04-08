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

#include <iostream>

#include "NetworkHandler.h"

#pragma once

class ImageHandler : public QObject
{
    Q_OBJECT
public:
    ImageHandler(QGridLayout* main_table): main_table(main_table) {}

public slots:
    void loadImage(NetworkHandler* network_handl);
    void pay();

signals:

private:
    QImage convertToHighQualityJPEG(const QByteArray& pngData);

    
private:
    QGridLayout* main_table;
    QVector<QPixmap> all_pixmaps;
    NetworkHandler* network_handl;
    std::vector<size_t> item_costs;
    int all_order = 0;
};