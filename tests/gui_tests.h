#include <QtWidgets>
#include <QTest>

#include "../lib/MainWindow.h"

class TestGui: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testBuyOneItem();
    void testButManyItems();

    void testRemoveOneItem();
    void testRemoveManyItems();

    void testMakeOrder();
    void testTwoMakeOrders();

    void testBuyingTooMany();
    void testRemovingWrong();

private:
    MainWindow* main_window;

    QPushButton* pay_button;
    QLabel* current_order_sum;

    QPushButton* first_burger_plus;
    QPushButton* first_burger_minus;

    QPushButton* second_burger_plus;
    QPushButton* second_burger_minus;
};