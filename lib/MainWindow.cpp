#include "MainWindow.h"

MainWindow::MainWindow(QUrl server_url, QWidget *parent, size_t session_id, const std::string& locale) 
    : QMainWindow(parent)
    , server_url(server_url)
    , network(nullptr)
    , main_table(new QGridLayout())
    , order_sum(new QLabel("0"))
    , operations_handler(new OperationsHandler(order_sum, network))
    , image_handler(new ImageHandler(main_table, operations_handler, session_id))
    , window(new QWidget(this))
    , main_layout(new QVBoxLayout(window))
    , network_thread(new QThread())
    , session_id(session_id)
    , locale(locale) {
        
    QUrl get_items_url = server_url;
    get_items_url.setPath("/get_description");
    network = new NetworkHandler(get_items_url, locale);
    operations_handler->network = network;

    network->moveToThread(network_thread);
    QObject::connect(this, &MainWindow::init_desc, network, &NetworkHandler::get_desc);
    QObject::connect(network, &NetworkHandler::update_ui_pic, image_handler, &ImageHandler::loadImage);
    QObject::connect(network, &NetworkHandler::update_text_desc, image_handler, &ImageHandler::update_text_desc);
    QObject::connect(network, &NetworkHandler::send_order_result, this, &MainWindow::show_order_res);
    QObject::connect(network, &NetworkHandler::send_buy_result, this, &MainWindow::handle_buy_fail);

    QObject::connect(this, &MainWindow::notify_operations_holder, operations_handler, &OperationsHandler::handle_buy_fail);

    QObject::connect(operations_handler, &OperationsHandler::buy_item, network, &NetworkHandler::buy_item);
    QObject::connect(operations_handler, &OperationsHandler::remove_item, network, &NetworkHandler::remove_item);
    QObject::connect(operations_handler, &OperationsHandler::make_order, network, &NetworkHandler::make_order);

    emit init_desc();
    network_thread->start();

    init_ui();
}

void MainWindow::init_ui() {
    int font_id = QFontDatabase::addApplicationFont("/home/nectovp/Code/cpp/qttest/resources/fonts/AmericanCaptainPatrius02Fre-PvPd.ttf");
    font_family = QFontDatabase::applicationFontFamilies(font_id).at(0);

    QPalette palette(QColor("#252b35"));
    window->setPalette(palette);

    init_main_label();
    init_order_layout();
    init_scroll_area();

    main_layout->setSpacing(20);
    window->setLayout(main_layout);
    
    setPalette(QPalette(QColor("#252b35")));
    setStyleSheet("background-color:rgb(56, 56, 66)");
    resize(2560, 1600);
    setWindowFlags(Qt::FramelessWindowHint);
    setCentralWidget(window);
}

void MainWindow::init_main_label() {
    QLabel* app_name_pic_label = new QLabel();
    QPixmap app_name_pixmap("/home/nectovp/Code/cpp/qttest/resources/icons/Vkusno_I_Tochka_symbol.png");
    app_name_pixmap = app_name_pixmap.scaled(100, 100, Qt::KeepAspectRatio);
    app_name_pic_label->setPixmap(app_name_pixmap);
    app_name_pic_label->setScaledContents(true);
    
    QLabel* app_name_text_label = new QLabel(QApplication::translate("mainlayout", "Вкусно и точка"));
    app_name_text_label->setFont(QFont(font_family, 45));
    app_name_text_label->setStyleSheet("color: rgb(57, 234, 228);");
    app_name_text_label->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout* logo_layout = new QHBoxLayout();
    logo_layout->addWidget(app_name_pic_label);
    logo_layout->addWidget(app_name_text_label);
    logo_layout->setAlignment(Qt::AlignCenter);
    logo_layout->setSpacing(20);
    
    main_layout->addLayout(logo_layout);
}

void MainWindow::init_order_layout() {
    QPushButton* pay_button = new QPushButton(QApplication::translate("mainlayout", "Оплатить"));
    pay_button->setFixedSize(300, 75);
    pay_button->setFont(QFont(font_family, 30));
    pay_button->setStyleSheet("color: rgb(57, 234, 228);;");

    QObject::connect(pay_button, &QPushButton::released, [this](){
        this->operations_handler->pay(session_id);
    });
    
    QLabel* order_sum_text = new QLabel(QApplication::translate("mainlayout", "Сумма заказа:"));
    order_sum_text->setFont(QFont(font_family, 30));
    order_sum_text->setStyleSheet("color: rgb(57, 234, 228);");
    order_sum_text->setAlignment(Qt::AlignCenter);
    order_sum_text->setFixedSize(320, 75);

    order_sum->setFont(QFont(font_family, 30));
    order_sum->setStyleSheet("color: rgb(57, 234, 228);");
    order_sum->setAlignment(Qt::AlignCenter);
    order_sum->setFixedSize(150, 75);

    QHBoxLayout* order_layout = new QHBoxLayout();
    order_layout->addWidget(order_sum_text);
    order_layout->addWidget(order_sum);
    order_layout->addWidget(pay_button);
    order_layout->setAlignment(Qt::AlignCenter);
    order_layout->setSpacing(40);

    main_layout->addLayout(order_layout);
}

void MainWindow::init_scroll_area() {
    QScrollArea* scroll_area = new QScrollArea();
    scroll_area->setWidgetResizable(true);
    scroll_area->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
    scroll_area->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scroll_area->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical {"
    "    border: none;"
    "    background: transparent;"
    "    width: 35px;"
    "    margin: 0px;"
    "}"
    "QScrollBar::handle:vertical {"
    "    background: rgb(57, 234, 228);"
    "    min-height: 20px;"
    "    border-radius: 4px;"
    "}"
    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
    "    height: 0px;"
    "    background: none;"
    "}"
    "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
    "    border: none;"
    "    background: none;"
    "}");
    
    QWidget* scroll_widget = new QWidget();
    scroll_widget->setLayout(main_table);
    scroll_area->setWidget(scroll_widget);

    main_layout->addWidget(scroll_area);
}

MainWindow::~MainWindow() {
    delete operations_handler;
    delete image_handler;  
    delete window;

    network_thread->quit();
    network_thread->wait();
    delete network;
}

void MainWindow::show_order_res(QString message, int result) {
    QPushButton* order_result = new QPushButton();
    order_result->setFont(QFont(font_family, 40));
    order_result->setStyleSheet("color: rgb(57, 234, 228); background-color:rgb(56, 56, 66); border-radius: 4px;");
    order_result->setFixedSize(700, 300);

    auto find_order_id = [](const std::string& str) {
        int digit_start = -1;
        int digit_end = -1;
        for(int i = 0; i < str.size(); ++i) {
            if(std::isdigit(str[i])) {
                if(digit_start == -1)
                    digit_start = i;
            } else if(digit_start != -1) {
                digit_end = i - 1;
                break;
            }
        }
        return QString::fromStdString(str.substr(digit_start, digit_end - digit_start + 1));
    };
    
    order_result->setText(QApplication::translate("mainlayout", "заказ ") + find_order_id(message.toStdString()) + QApplication::translate("mainlayout", " готов"));
    order_result->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(order_result, &QPushButton::pressed, order_result, &QPushButton::deleteLater);
    
    order_result->move(400, 500);
    order_result->resize(700, 300);
    order_result->show();
}

void MainWindow::handle_buy_fail(int id) {
    QPushButton* order_result = new QPushButton();
    order_result->setFont(QFont(font_family, 40));
    order_result->setStyleSheet("color: rgb(57, 234, 228); background-color:rgb(56, 56, 66); border-radius: 4px;");
    order_result->setFixedSize(700, 300);

    order_result->setText(QApplication::translate("mainlayout", "извините, нет в наличии"));
    order_result->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(order_result, &QPushButton::pressed, order_result, &QPushButton::deleteLater);
    
    order_result->move(400, 500);
    order_result->resize(700, 300);
    order_result->show();

    emit notify_operations_holder(id);
}

QGridLayout* MainWindow::get_grid_layout() {
    return main_table;
}