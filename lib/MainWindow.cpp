#include "MainWindow.h"

MainWindow::MainWindow(QUrl server_url, QWidget *parent) 
    : QMainWindow(parent)
    , server_url(server_url)
    , network(nullptr)
    , main_table(new QGridLayout())
    , image_handler(new ImageHandler(main_table))
    , window(new QWidget(this))
    , main_layout(new QVBoxLayout(window)) {
    
    QUrl get_items_url = server_url;
    get_items_url.setPath("/get_description");
    network = new NetworkHandler(get_items_url);
    
    QObject::connect(network, &NetworkHandler::update_ui_pic, image_handler, &ImageHandler::loadImage);

    init_ui();
}

void MainWindow::init_ui() {
    QPalette palette(QColor("white"));
    window->setPalette(palette);

    init_main_label();
    init_order_layout();
    init_scroll_area();

    window->setLayout(main_layout);
    //window->setWindowFlags(Qt::FramelessWindowHint);
    //window->resize(2560, 1600);
    
    setPalette(QPalette(QColor("white")));
    resize(2560, 1600);
    setWindowFlags(Qt::FramelessWindowHint);
    setCentralWidget(window);
}

void MainWindow::init_main_label() {
    //QLabel* app_name_label = new QLabel();
    app_name_label = new QLabel();
    QPixmap app_name_pixmap("/home/nectovp/Code/cpp/qttest/resources/app_name.png");
    app_name_label->setPixmap(app_name_pixmap);
    app_name_label->setAlignment(Qt::AlignCenter);
    
    main_layout->addWidget(app_name_label);    
}

void MainWindow::init_order_layout() {
    //QPushButton* pay_button = new QPushButton(QApplication::translate("mainlayout", "Оплатить"));
    pay_button = new QPushButton(QApplication::translate("mainlayout", "Оплатить"));
    pay_button->setFixedSize(300, 75);
    pay_button->setFont(QFont("Arial", 30));
    pay_button->setStyleSheet("background-color: green; color: white;");
    
    //QLabel* order_sum = new QLabel(QApplication::translate("mainlayout", "Сумма заказа:"));
    order_sum = new QLabel(QApplication::translate("mainlayout", "Сумма заказа:"));
    order_sum->setFont(QFont("Arial", 30));
    order_sum->setStyleSheet("background-color: blue; color: white;");
    order_sum->setAlignment(Qt::AlignCenter);
    order_sum->setFixedSize(300, 75);

    QObject::connect(pay_button, &QPushButton::released, image_handler, &ImageHandler::pay);

    //QHBoxLayout* order_layout = new QHBoxLayout();
    order_layout = new QHBoxLayout();
    order_layout->addWidget(order_sum);
    order_layout->addWidget(pay_button);
    order_layout->setAlignment(Qt::AlignCenter);

    main_layout->addLayout(order_layout);
}

void MainWindow::init_scroll_area() {
    //QScrollArea* scroll_area = new QScrollArea();
    scroll_area = new QScrollArea();
    scroll_area->setWidgetResizable(true);
    scroll_area->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
    scroll_area->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scroll_area->verticalScrollBar()->setStyleSheet("width: 35px;");

    //QWidget* scroll_widget = new QWidget();
    scroll_widget = new QWidget();
    scroll_widget->setLayout(main_table);
    scroll_area->setWidget(scroll_widget);

    main_layout->addWidget(scroll_area);
}