#pragma once

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGraphicsDropShadowEffect>

#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "json.hpp"

class NetworkHandler : public QObject
{
    Q_OBJECT
public:
    explicit NetworkHandler(QUrl server_url, QObject *parent = 0);
    virtual ~NetworkHandler();

signals:
    void update_ui_pic(NetworkHandler* network_handl);

private slots:
    void parse_desc();
    void load_pic();

public:
    void request_pics();
    const QVector<QByteArray>& get_pics_data();
    const std::vector<std::string>& get_names();
    const std::vector<size_t>& get_costs();
    const std::vector<int>& get_ids();

    void buy_item(size_t product_id, size_t count, size_t session_id);
    void remove_item(size_t product_id, size_t count, size_t session_id);
    void make_order(size_t order_sum, size_t session_id);

private:
    QNetworkAccessManager manager;
    QByteArray m_DownloadedData;
    std::vector<std::string> all_item_pic_urls;
    std::vector<std::string> all_item_names;
    std::vector<size_t> all_item_cost;
    std::vector<int> all_item_ids;
    std::unordered_map<std::string, int> url_to_id;
    QVector<QByteArray> all_item_pics;
    std::string last_request;
    int url_parsed = 0;
};