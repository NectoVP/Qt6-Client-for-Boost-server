#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(QUrl server_url, const std::string& locale, QObject *parent) : server_url(server_url), QObject(parent), locale(locale) {

}

void NetworkHandler::get_desc() {
    manager = new QNetworkAccessManager();
    QNetworkReply* reply = manager->get(QNetworkRequest(server_url));
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
        all_item_names.push_back(i[locale + "_name"].template get<std::string>());
        all_item_cost.push_back(i["cost"].template get<size_t>());
        all_item_ids.push_back(i["id"].template get<size_t>());
        url_to_id.insert(std::make_pair(all_item_pic_urls.back(), all_item_pic_urls.size() - 1));
    }
    all_item_pics.resize(all_item_pic_urls.size());
    reply->deleteLater();
    emit update_text_desc(this);
    request_pics();
}

void NetworkHandler::request_pics() {
    for(int i = 0; i < all_item_pic_urls.size(); ++i) {
        QNetworkRequest request(QUrl(all_item_pic_urls[i].c_str()));
        QNetworkReply* reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, &NetworkHandler::load_pic);
        QThread::msleep(100);
    }
}

void NetworkHandler::load_pic() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if(reply->error()) {
        QNetworkRequest request(reply->url());
        QNetworkReply* reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, &NetworkHandler::load_pic);
        QThread::msleep(500);
        reply->deleteLater();
        return;
    }
    all_item_pics[url_to_id[reply->url().toString().toStdString()]] = reply->readAll();
    emit update_ui_pic(this, url_to_id[reply->url().toString().toStdString()]);
    reply->deleteLater();
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
    QNetworkReply *reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, &NetworkHandler::buy_result);
}


void NetworkHandler::remove_item(size_t product_id, size_t count, size_t session_id) {
    QNetworkRequest request(QUrl("http://0.0.0.0:8080/remove"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonObject obj;
    obj["id"] = (int)product_id;
    obj["count"] = (int)count;
    obj["sessionId"] = (int)session_id;
    QJsonDocument doc(obj);
    
    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager->sendCustomRequest(request, "DELETE", data);
}


void NetworkHandler::make_order(size_t order_sum, size_t session_id) {
    QNetworkRequest request(QUrl("http://0.0.0.0:8080/make_order"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj;
    obj["order_sum"] = (int)order_sum;
    obj["sessionId"] = (int)session_id;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, &NetworkHandler::order_result);
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

void NetworkHandler::order_result() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    emit send_order_result(QString::fromStdString(reply->readAll().toStdString()), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
}

void NetworkHandler::buy_result() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    std::string temp = reply->readAll().toStdString();
    if(temp == "item was bought correctly")
        return;
    emit send_buy_result(std::stoi(temp.substr(temp.find(':') + 1)));
}