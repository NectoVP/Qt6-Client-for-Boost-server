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

#include <QTranslator>
#include <QLocale>

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
    
    std::string path = "/home/nectovp/Code/cpp/qttest/";

    QTranslator translator;
    
    //QString current_locale = QLocale().name();
    //current_locale = "ru_RU";
    
    std::string current_locale(argv[2]);
    qDebug() << QString::fromStdString(current_locale);
    if(current_locale == "en") {
        auto load_res = translator.load(QLocale(), std::string(path + "build/en.qm").c_str());
        app.installTranslator(&translator);
    }

    MainWindow main_window(QUrl("http://0.0.0.0:8080"), nullptr, std::stoi(std::string(argv[1]).c_str()), current_locale, path);
    main_window.show();

    return app.exec();
}