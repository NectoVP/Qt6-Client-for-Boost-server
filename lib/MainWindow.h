#pragma once

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QTableView>
#include <QList>
#include <QListWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMainWindow>
#include <QScrollBar>
#include <QScreen>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include <QDebug>

#include <QNetworkRequest>
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QThread>

#include <iostream>

#include "ImageHandler.h"
#include "NetworkHandler.h"
#include "OperationsHandler.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QUrl server_url, QWidget *parent, size_t session_id);

    virtual ~MainWindow();

private:
    void init_ui();
    void init_main_label();
    void init_order_layout();
    void init_scroll_area();

signals:
    void init_desc();
    void notify_operations_holder(int id);

public slots:
    void show_order_res(QString message, int result);
    void handle_buy_fail(int id);

private:
    QUrl server_url;
    NetworkHandler* network;
    QGridLayout* main_table;
    QLabel* order_sum;
    OperationsHandler* operations_handler;
    ImageHandler* image_handler;
    
    QWidget* window;
    QVBoxLayout* main_layout;
    QString font_family;

    QThread* network_thread;
    size_t session_id;
};