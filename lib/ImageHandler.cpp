#include "ImageHandler.h"

ImageHandler::ImageHandler(QGridLayout* main_table, OperationsHandler* operations_handl, size_t session_id)
    : main_table(main_table)
    , plus_icon("/home/nectovp/Code/cpp/qttest/resources/icons/plus_icon.png")
    , minus_icon("/home/nectovp/Code/cpp/qttest/resources/icons/minus_icon.png")
    , plus_dark_icon("/home/nectovp/Code/cpp/qttest/resources/icons/plus_dark_icon.png")
    , minus_dark_icon("/home/nectovp/Code/cpp/qttest/resources/icons/minus_dark_icon.png")
    , operations_handl(operations_handl)
    , session_id(session_id) {
    }

void ImageHandler::update_text_desc(NetworkHandler* network_handl) {
    int font_id = QFontDatabase::addApplicationFont("/home/nectovp/Code/cpp/qttest/resources/fonts/AmericanCaptainPatrius02Fre-PvPd.ttf");
    QString font_family = QFontDatabase::applicationFontFamilies(font_id).at(0);

    const std::vector<std::string>& name_vec = network_handl->get_names();
    const std::vector<size_t>& cost_vec = network_handl->get_costs();
    const std::vector<int>& ids_vec = network_handl->get_ids();

    for(int i = 0; i < name_vec.size(); ++i) {
        
        QLabel *label_name = new QLabel(QString::fromStdString(name_vec[i]));
        label_name->setFont(QFont(font_family, 25));
        label_name->setStyleSheet("color: rgb(57, 234, 228);");
        label_name->setAlignment(Qt::AlignCenter);
        main_table->addWidget(label_name, i / 4 * 3, i % 4 * 3, 1, 3, Qt::AlignHCenter);
     
        QLabel *label_pic = new QLabel();
        QPixmap temp_pix("/home/nectovp/Code/cpp/qttest/resources/icons/loading2.png");
        temp_pix = temp_pix.scaled(286, 290, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label_pic->setPixmap(temp_pix);
        label_pic->setFixedSize(286, 290);
        label_pic->setAlignment(Qt::AlignCenter);
        main_table->addWidget(label_pic, i / 4 * 3 + 1, i % 4 * 3, 1, 3, Qt::AlignHCenter);

        QPushButton* plus_button = new QPushButton();
        plus_button->setIcon(plus_icon);
        plus_button->setIconSize(QSize(100, 100));
        plus_button->setMaximumSize(QSize(100, 100));
        plus_button->setStyleSheet(QString("QPushButton {border: 0px;}"));
        main_table->addWidget(plus_button, i / 4 * 3 + 2, (i % 4) * 3, Qt::AlignHCenter);

        QLabel* label_cost = new QLabel(QString::fromStdString(std::to_string(cost_vec[i])));
        label_cost->setFont(QFont(font_family, 20));
        label_cost->setFixedSize(QSize(100, 75));
        label_cost->setStyleSheet("color:rgb(57, 234, 228);");
        label_cost->setAlignment(Qt::AlignCenter);
        main_table->addWidget(label_cost, i / 4 * 3 + 2, (i % 4) * 3 + 1, Qt::AlignHCenter);   

        QPushButton* minus_button = new QPushButton();
        minus_button->setIcon(minus_icon);
        minus_button->setIconSize(QSize(100, 100));
        minus_button->setMaximumSize(QSize(100, 100));
        minus_button->setStyleSheet(QString("QPushButton {border: 0px;}"));
        main_table->addWidget(minus_button, i / 4 * 3 + 2, (i % 4) * 3 + 2, Qt::AlignHCenter);
        
        connect(plus_button, &QPushButton::pressed, [plus_button, this](){
            plus_button->setIcon(this->plus_dark_icon);
            plus_button->setIconSize(QSize(100, 100));
        });

        connect(minus_button, &QPushButton::pressed, [minus_button, this](){
            minus_button->setIcon(this->minus_dark_icon);
            minus_button->setIconSize(QSize(100, 100));
        });

        connect(plus_button, &QPushButton::released, [this, cost=cost_vec[i], id=ids_vec[i], plus_button](){
            this->operations_handl->buy(id, 1, this->session_id, cost);
            plus_button->setIcon(this->plus_icon);
            plus_button->setIconSize(QSize(100, 100));
        });

        connect(minus_button, &QPushButton::released, [this, cost=cost_vec[i], id=ids_vec[i], minus_button](){
            this->operations_handl->remove(id, 1, this->session_id, cost);
            minus_button->setIcon(this->minus_icon);
            minus_button->setIconSize(QSize(100, 100));
        });
    }

    main_table->setHorizontalSpacing(10);
    main_table->setVerticalSpacing(30);
}

void ImageHandler::loadImage(NetworkHandler* network_handl, int idx) {
    QLabel *label_pic = new QLabel();
    label_pic->setPixmap(roundCorners(QPixmap::fromImage(convertToHighQualityJPEG(network_handl->get_pics_data()[idx])), 20));
    label_pic->setFixedSize(286, 290);
    label_pic->setAlignment(Qt::AlignCenter);
    main_table->addWidget(label_pic, idx / 4 * 3 + 1, idx % 4 * 3, 1, 3, Qt::AlignHCenter);
}

QImage ImageHandler::convertToHighQualityJPEG(const QByteArray& pngData) {
    QImage image;
    if (!image.loadFromData(pngData, "PNG")) return QImage();

    if (image.hasAlphaChannel()) {
        QImage opaque(image.size(), QImage::Format_RGB32);
        opaque.fill(QColor("#5FC0CE"));
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

QPixmap ImageHandler::roundCorners(const QPixmap& input, int radius) {
    if (input.isNull()) return QPixmap();

    QPixmap output(input.size());
    output.fill(Qt::transparent);

    QPainter painter(&output);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addRoundedRect(output.rect(), radius, radius);

    painter.setClipPath(path);
    painter.drawPixmap(0, 0, input);

    return output;
}