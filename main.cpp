#include <QApplication>
#include "mainwindow.h"
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize Oracle connection
    if (!Connection::getInstance().open()) {
        qDebug() << "Database connection failed!";
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
