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

#include "lib/ImageHandler.h"
#include "lib/NetworkHandler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QUrl imageUrl("http://0.0.0.0:8080/get_description");
    
    NetworkHandler* network = new NetworkHandler(imageUrl);

    QGridLayout* main_table = new QGridLayout(); 
    
    ImageHandler* imageHandler = new ImageHandler(main_table);
    QObject::connect(network, &NetworkHandler::update_ui_pic, imageHandler, &ImageHandler::loadImage);
 
    QWidget* window = new QWidget();
    QPalette palette(QColor("white"));
    window->setPalette(palette);

    QVBoxLayout* main_layout = new QVBoxLayout(window);
    
    QScrollArea* scroll_area = new QScrollArea();
    
    scroll_area->setWidgetResizable(true);
    scroll_area->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
    scroll_area->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scroll_area->verticalScrollBar()->setStyleSheet("width: 35px;");

    QWidget* scroll_widget = new QWidget();
    scroll_widget->setLayout(main_table);
    scroll_area->setWidget(scroll_widget);
    
    QPushButton* pay_button = new QPushButton(QApplication::translate("mainlayout", "Оплатить"));
    pay_button->setFixedSize(300, 75);
    pay_button->setFont(QFont("Arial", 30));
    pay_button->setStyleSheet("background-color: green; color: white;");
    
    QLabel* order_sum = new QLabel(QApplication::translate("mainlayout", "Сумма заказа:"));
    order_sum->setFont(QFont("Arial", 30));
    order_sum->setStyleSheet("background-color: blue; color: white;");
    order_sum->setAlignment(Qt::AlignCenter);
    order_sum->setFixedSize(300, 75);

    QObject::connect(pay_button, &QPushButton::released, imageHandler, &ImageHandler::pay);

    QHBoxLayout* order_layout = new QHBoxLayout();
    order_layout->addWidget(order_sum);
    order_layout->addWidget(pay_button);
    order_layout->setAlignment(Qt::AlignCenter);

    QLabel* app_name_label = new QLabel();
    QPixmap app_name_pixmap("/home/nectovp/Code/cpp/qttest/resources/app_name.png");
    app_name_label->setPixmap(app_name_pixmap);
    app_name_label->setAlignment(Qt::AlignCenter);

    main_layout->addWidget(app_name_label);
    main_layout->addLayout(order_layout);
    main_layout->addWidget(scroll_area);

    window->setWindowFlags(Qt::FramelessWindowHint);
    window->setLayout(main_layout);
    window->resize(2560, 1600);
    
    window->show();
    return app.exec();
}