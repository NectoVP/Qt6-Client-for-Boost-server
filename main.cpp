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

    QTranslator translator;
    
    QString current_locale = QLocale().name();
    //current_locale = "ru_RU";
    if(current_locale == "en_US") {
        auto load_res = translator.load(QLocale(), "/home/nectovp/Code/cpp/qttest/build/en.qm");
        app.installTranslator(&translator);
    }

    MainWindow main_window(QUrl("http://0.0.0.0:8080"), nullptr, std::stoi(std::string(argv[1]).c_str()), current_locale.toStdString().substr(0, 2));
    main_window.show();

    return app.exec();
}