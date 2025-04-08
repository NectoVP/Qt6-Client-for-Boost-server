#include "ImageHandler.h"

void ImageHandler::loadImage(NetworkHandler* network_handl) {
    this->network_handl = network_handl;

    const QVector<QByteArray>& pic_vec = network_handl->get_pics_data();
    const std::vector<std::string>& name_vec = network_handl->get_names();
    const std::vector<size_t>& cost_vec = network_handl->get_costs();
    const std::vector<int>& ids_vec = network_handl->get_ids();

    int font_id = QFontDatabase::addApplicationFont("/home/nectovp/Code/cpp/qttest/resources/Freedom-10eM.ttf");
    QString font_family = QFontDatabase::applicationFontFamilies(font_id).at(0);

    for(int i = 0; i < pic_vec.size(); ++i) {
        
        QLabel *label_name = new QLabel(QString::fromStdString(name_vec[i]));
        label_name->setFont(QFont(font_family, 25));
        label_name->setAlignment(Qt::AlignCenter);
        main_table->addWidget(label_name, i / 4 * 3, i % 4 * 3, 1, 3, Qt::AlignHCenter);
     
        QLabel *label_pic = new QLabel();
        label_pic->setPixmap(QPixmap::fromImage(convertToHighQualityJPEG(pic_vec[i])));
        label_pic->setAlignment(Qt::AlignCenter);
        main_table->addWidget(label_pic, i / 4 * 3 + 1, i % 4 * 3, 1, 3, Qt::AlignHCenter);

        QPushButton* plus_button = new QPushButton();
        QIcon plus(QPixmap("/home/nectovp/Code/cpp/qttest/resources/plus_icon.png"));
        plus_button->setIcon(plus);
        plus_button->setIconSize(QSize(100, 100));
        plus_button->setMaximumSize(QSize(100, 100));
        plus_button->setStyleSheet(QString("QPushButton {border: 0px;}"));
        main_table->addWidget(plus_button, i / 4 * 3 + 2, (i % 4) * 3, Qt::AlignHCenter);

        QLabel* label_cost = new QLabel(QString::fromStdString(std::to_string(cost_vec[i])));
        label_cost->setFont(QFont(font_family, 20));
        label_cost->setFixedSize(QSize(100, 75));
        label_cost->setAlignment(Qt::AlignCenter);
        main_table->addWidget(label_cost, i / 4 * 3 + 2, (i % 4) * 3 + 1, Qt::AlignHCenter);   

        QPushButton* minus_button = new QPushButton();
        QIcon minus(QPixmap("/home/nectovp/Code/cpp/qttest/resources/minus_icon.png"));
        minus_button->setIcon(minus);
        minus_button->setIconSize(QSize(100, 100));
        minus_button->setMaximumSize(QSize(100, 100));
        minus_button->setStyleSheet(QString("QPushButton {border: 0px;}"));
        main_table->addWidget(minus_button, i / 4 * 3 + 2, (i % 4) * 3 + 2, Qt::AlignHCenter);
        
        connect(plus_button, &QPushButton::released, [this, cost=cost_vec[i], id=ids_vec[i]](){
            this->network_handl->buy_item(id, 1, 1);
            this->all_order += cost;
        });

        connect(minus_button, &QPushButton::released, [this, cost=cost_vec[i], id=ids_vec[i]](){
            this->network_handl->remove_item(id, 1, 1);
            this->all_order -= cost;
        });
        
    }
    main_table->setHorizontalSpacing(10); 
    main_table->setVerticalSpacing(30);
}

void ImageHandler::pay() {
    network_handl->make_order(all_order, 1);
}

QImage ImageHandler::convertToHighQualityJPEG(const QByteArray& pngData) {
    QImage image;
    if (!image.loadFromData(pngData, "PNG")) return QImage();

    if (image.hasAlphaChannel()) {
        QImage opaque(image.size(), QImage::Format_RGB32);
        opaque.fill(QColor("white"));
        QPainter painter(&opaque);
        painter.drawImage(0, 0, image);
        image = opaque;
    }

    image.setColorSpace(QColorSpace::SRgb);
    
    QByteArray jpegData;
    QBuffer buffer(&jpegData);
    buffer.open(QIODevice::WriteOnly);
    
    image.save(&buffer, "JPEG", 100);
    
    return QImage::fromData(jpegData, "JPEG");
}