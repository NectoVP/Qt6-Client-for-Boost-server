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

#include "lib/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow main_window(QUrl("http://0.0.0.0:8080"), nullptr, std::stoi(std::string(argv[1]).c_str()));
    main_window.show();

    return app.exec();
}