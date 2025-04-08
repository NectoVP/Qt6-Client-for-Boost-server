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

#include <QDebug>

#include <QNetworkRequest>
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <iostream>

#include "ImageHandler.h"
#include "NetworkHandler.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QUrl server_url, QWidget *parent);


private:
    void init_ui();
    void init_main_label();
    void init_order_layout();
    void init_scroll_area();

private:
    QUrl server_url;
    NetworkHandler* network;
    QGridLayout* main_table;
    ImageHandler* image_handler;
    
    QWidget* window;
    QVBoxLayout* main_layout;

    QLabel* app_name_label;

    QPushButton* pay_button;
    QLabel* order_sum;
    QHBoxLayout* order_layout;
    
    QScrollArea* scroll_area;
    QWidget* scroll_widget;
};