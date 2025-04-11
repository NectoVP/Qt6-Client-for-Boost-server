#include "gui_tests.h"

void TestGui::initTestCase() {
    std::string path = "/home/nectovp/Code/cpp/qttest/";
    main_window = new MainWindow(QUrl("http://0.0.0.0:8080"), nullptr, 100, "ru", path);
    main_window->show();

    QTest::qWait(10000);
    QApplication::processEvents();

    QObjectList main_children = main_window->children()[1]->children();
    pay_button = qobject_cast<QPushButton*>(main_children.at(5));
    current_order_sum = qobject_cast<QLabel*>(main_children.at(4));
    
    QGridLayout* grid = main_window->get_grid_layout();
    
    first_burger_plus = qobject_cast<QPushButton*>(grid->itemAtPosition(2, 0)->widget());
    first_burger_minus = qobject_cast<QPushButton*>(grid->itemAtPosition(2, 2)->widget());

    second_burger_plus = qobject_cast<QPushButton*>(grid->itemAtPosition(2, 6)->widget());
    second_burger_minus = qobject_cast<QPushButton*>(grid->itemAtPosition(2, 8)->widget());
}

void TestGui::cleanupTestCase() {
    QTest::qWait(5000);
    QApplication::processEvents();

    main_window->close();
    QApplication::processEvents();
}

void TestGui::testBuyOneItem() {
    QTest::mouseClick(first_burger_plus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QCOMPARE(current_order_sum->text(), QString("730"));
}

void TestGui::testButManyItems() {
    QTest::mouseClick(second_burger_plus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QTest::mouseClick(second_burger_plus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QCOMPARE(current_order_sum->text(), QString("1530"));
};

void TestGui::testRemoveOneItem() {
    QTest::mouseClick(first_burger_minus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QCOMPARE(current_order_sum->text(), QString("800"));
}
void TestGui::testRemoveManyItems() {
    QTest::mouseClick(second_burger_minus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QTest::mouseClick(second_burger_minus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QCOMPARE(current_order_sum->text(), QString("0"));
}

void TestGui::testMakeOrder() {
    QTest::mouseClick(first_burger_plus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QTest::mouseClick(pay_button, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QPushButton* orderResult = nullptr;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        if (widget->inherits("QPushButton") && widget->windowFlags() & Qt::Window) {
            orderResult = qobject_cast<QPushButton*>(widget);
            break;
        }
    }
    
    QVERIFY(orderResult != nullptr);
    QVERIFY(orderResult->isVisible());
    QCOMPARE(orderResult->text(), QString("заказ 100 готов"));
    QCOMPARE(current_order_sum->text(), QString("0"));
    
    if (orderResult)
        orderResult->deleteLater();
}

void TestGui::testTwoMakeOrders() {

    QTest::mouseClick(first_burger_plus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QTest::mouseClick(pay_button, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QPushButton* orderResult = nullptr;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        if (widget->inherits("QPushButton") && widget->windowFlags() & Qt::Window) {
            orderResult = qobject_cast<QPushButton*>(widget);
            break;
        }
    }
    
    QVERIFY(orderResult != nullptr);
    QVERIFY(orderResult->isVisible());
    QCOMPARE(orderResult->text(), QString("заказ 101 готов"));
    
    if (orderResult)
        orderResult->deleteLater();
    QCOMPARE(current_order_sum->text(), QString("0"));

    QTest::mouseClick(first_burger_plus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QTest::mouseClick(second_burger_plus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QTest::mouseClick(pay_button, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QPushButton* second_orderResult = nullptr;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        if (widget->inherits("QPushButton") && widget->windowFlags() & Qt::Window) {
            second_orderResult = qobject_cast<QPushButton*>(widget);
            break;
        }
    }
    
    QVERIFY(second_orderResult != nullptr);
    QVERIFY(second_orderResult->isVisible());
    QCOMPARE(second_orderResult->text(), QString("заказ 102 готов"));
    
    if (second_orderResult)
        second_orderResult->deleteLater();
    QCOMPARE(current_order_sum->text(), QString("0"));
}

void TestGui::testBuyingTooMany() {
    for(int i = 0; i < 13; ++i) {
        QTest::mouseClick(second_burger_plus, Qt::LeftButton);
        QTest::qWait(2000);
        QApplication::processEvents();
    }

    QPushButton* orderResult = nullptr;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        if (widget->inherits("QPushButton") && widget->windowFlags() & Qt::Window) {
            orderResult = qobject_cast<QPushButton*>(widget);
            break;
        }
    }

    QVERIFY(orderResult != nullptr);
    QVERIFY(orderResult->isVisible());
    QCOMPARE(orderResult->text(), QString("извините, нет в наличии"));

    if (orderResult)
        orderResult->deleteLater();
    
    QCOMPARE(current_order_sum->text(), QString("4800"));
}
void TestGui::testRemovingWrong() {
    QTest::mouseClick(first_burger_minus, Qt::LeftButton);
    QTest::qWait(2000);
    QApplication::processEvents();

    QCOMPARE(current_order_sum->text(), QString("4800"));

    for(int i = 0; i < 13; ++i) {
        QTest::mouseClick(second_burger_minus, Qt::LeftButton);
        QTest::qWait(2000);
        QApplication::processEvents();
    }

    QCOMPARE(current_order_sum->text(), QString("0"));
}

QTEST_MAIN(TestGui)