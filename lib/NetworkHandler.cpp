#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(QUrl server_url, QObject *parent) : QObject(parent) {
    QNetworkReply* reply = manager.get(QNetworkRequest(server_url));
    connect(reply, &QNetworkReply::finished, this, &NetworkHandler::parse_desc);
}

NetworkHandler::~NetworkHandler() {}

void NetworkHandler::parse_desc() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    m_DownloadedData = reply->readAll();
    std::string s = QString(m_DownloadedData).toStdString();
    nlohmann::json temp_js = nlohmann::json::parse(s);
    for(auto &i : temp_js) {
        all_item_pic_urls.push_back(i["pic_url"].template get<std::string>());
        all_item_names.push_back(i["name"].template get<std::string>());
        all_item_cost.push_back(i["cost"].template get<size_t>());
        all_item_ids.push_back(i["id"].template get<size_t>());
        url_to_id.insert(std::make_pair(all_item_pic_urls.back(), all_item_pic_urls.size() - 1));
    }
    all_item_pics.resize(all_item_pic_urls.size());
    reply->deleteLater();
    request_pics();
}

void NetworkHandler::request_pics() {
    for(int i = 0; i < all_item_pic_urls.size(); ++i) {
        QNetworkRequest request(QUrl(all_item_pic_urls[i].c_str()));
        QNetworkReply* reply = manager.get(request);
        connect(reply, &QNetworkReply::finished, this, &NetworkHandler::load_pic);
        std::chrono::milliseconds time(100);
        std::this_thread::sleep_for(time);
    }
}

void NetworkHandler::load_pic() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if(reply->error()) {
        //qDebug() << reply->url() <<  reply->errorString();
        QNetworkRequest request(reply->url());
        QNetworkReply* reply = manager.get(request);
        connect(reply, &QNetworkReply::finished, this, &NetworkHandler::load_pic);
        std::chrono::milliseconds time(200);
        std::this_thread::sleep_for(time);
        reply->deleteLater();
        return;
    }
    all_item_pics[url_to_id[reply->url().toString().toStdString()]] = reply->readAll();// .push_back(reply->readAll());
    reply->deleteLater();
    url_parsed++;
    if(url_parsed == all_item_pic_urls.size());
        emit update_ui_pic(this);
}

void NetworkHandler::buy_item(size_t product_id, size_t count, size_t session_id) {
    QNetworkRequest request(QUrl("http://0.0.0.0:8080/buy"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj;
    obj["id"] = (int)product_id;
    obj["count"] = (int)count;
    obj["sessionId"] = (int)session_id;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager.post(request, data);
}


void NetworkHandler::remove_item(size_t product_id, size_t count, size_t session_id) {
    QNetworkRequest request(QUrl("http://0.0.0.0:8080/remove"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-HTTP-Method-Override", "DELETE");
    
    QJsonObject obj;
    obj["id"] = (int)product_id;
    obj["count"] = (int)count;
    obj["sessionId"] = (int)session_id;
    QJsonDocument doc(obj);
    
    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager.post(request, data);
}


void NetworkHandler::make_order(size_t order_sum, size_t session_id) {
    QNetworkRequest request(QUrl("http://0.0.0.0:8080/make_order"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj;
    obj["order_sum"] = (int)order_sum;
    obj["sessionId"] = (int)session_id;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager.post(request, data);
}

const QVector<QByteArray>& NetworkHandler::get_pics_data() {
    return all_item_pics;
}

const std::vector<std::string>& NetworkHandler::get_names() {
    return all_item_names;
}

const std::vector<size_t>& NetworkHandler::get_costs() {
    return all_item_cost;
}

const std::vector<int>& NetworkHandler::get_ids() {
    return all_item_ids;
}