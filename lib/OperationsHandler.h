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

#pragma once

class OperationsHandler : public QObject
{
    Q_OBJECT
public:
    OperationsHandler(QLabel* order_sum, NetworkHandler* network);
    void buy(size_t product_id, size_t count, size_t session_id, size_t cost);
    void remove(size_t product_id, size_t count, size_t session_id, size_t cost);
    void pay(size_t session_id);

signals:
    void buy_item(size_t product_id, size_t count, size_t session_id);
    void remove_item(size_t product_id, size_t count, size_t session_id);
    void make_order(size_t order_sum, size_t session_id);

public slots:
    void handle_buy_fail(int id);
public:
    QLabel* order_sum;
    NetworkHandler* network;
    std::unordered_map<size_t, size_t> bought_items;
    std::unordered_map<size_t, size_t> costs;
    size_t current_order_id = 0;
    size_t session_id;
};