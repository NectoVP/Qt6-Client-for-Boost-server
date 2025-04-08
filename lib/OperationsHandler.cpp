#include "OperationsHandler.h"

OperationsHandler::OperationsHandler(QLabel* order_sum, NetworkHandler* network) : order_sum(order_sum), network(network) {

}

void OperationsHandler::buy(size_t product_id, size_t count, size_t session_id, size_t cost) {
    this->session_id = session_id;
    costs[product_id] = cost;
    order_sum->setText(QString::fromStdString(std::to_string(order_sum->text().toInt() + cost)));
    bought_items[product_id] += 1;
    emit buy_item(product_id, count, session_id + current_order_id);
}
void OperationsHandler::remove(size_t product_id, size_t count, size_t session_id, size_t cost) {
    int old_val = order_sum->text().toInt();
    if(old_val == 0 || bought_items[product_id] == 0)
        return;
    bought_items[product_id] -= 1;
    order_sum->setText(QString::fromStdString(std::to_string(old_val - cost)));
    emit remove_item(product_id, count, session_id + current_order_id);
}

void OperationsHandler::pay(size_t session_id) {
    bought_items.clear();
    emit make_order(order_sum->text().toInt(), session_id + current_order_id);
    if(++current_order_id == 100)
        current_order_id = 0;
    order_sum->setText(QString("0"));
}

void OperationsHandler::handle_buy_fail(int product_id) {
    int old_val = order_sum->text().toInt();
    order_sum->setText(QString::fromStdString(std::to_string(old_val - costs[product_id])));
    bought_items[product_id] -= 1;
}
