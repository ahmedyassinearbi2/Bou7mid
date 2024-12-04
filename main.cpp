#include "connection.h"
#include "etudiant.h"
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "etudiant.h"
#include <QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QCoreApplication>
#include "arduinoo.h"

QString fetchDataFromDatabase() {
    // Implement your database query here and return the data as QString
    QString data = "Hello, World!"; // Placeholder for actual database data
    return data;
}

// Method to send data to Arduino
void sendDataToArduino(Arduino &arduino) {
    QString data = fetchDataFromDatabase(); // Fetch data from database
    QByteArray byteArray = data.toUtf8(); // Convert QString to QByteArray
    arduino.write_to_arduino(byteArray); // Send data to Arduino
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;
    bool test=c.createconnect();
    if(test)
    {w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

    Arduino arduino;
    if (arduino.connect_arduino() == 0) { // Connect to Arduino
        sendDataToArduino(arduino); // Send data to Arduino
        arduino.close_arduino(); // Close the connection after sending
    } else {
        qDebug() << "Failed to connect to Arduino.";
    }

    return a.exec();
}
