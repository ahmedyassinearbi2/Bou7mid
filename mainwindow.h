#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connection.h"
#include "etudiant.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>
#include <QProgressBar>
#include <QSqlQueryModel>
#include <QNetworkReply>  // Include this header for QNetworkReply
#include <QTimer>
#include <QtCharts>


// Include the generated UI header file
namespace Ui {
class MainWindow; // Forward declaration
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Product Management
    void on_pushButton_addProduct_clicked();
    void on_pushButton_updateProduct_clicked();
    void on_pushButton_deleteProduct_clicked();
    void on_pushButton_viewProducts_clicked();
    void on_pushButton_addProductInInvoice_clicked();
    //void handleEmailReply(QNetworkReply *reply);
    void on_pushButton_updateInvoiceProduct_clicked();
    void on_pushButton_deleteInvoiceProduct_clicked();


private:
    Ui::MainWindow *ui; // Pointer for the generated UI class

    // Product Management UI Elements

private:
    QStackedWidget *stackedWidget; // Declare stackedWidget
    QLineEdit *lineEdit_UpdateProductID;
    QLineEdit *lineEdit_DeleteProductID;
    QLineEdit *lineEdit_NewQuantity; // Add this line
    QLineEdit *lineEdit_ID;
    QLineEdit *lineEdit_ProductName;
    QLineEdit *lineEdit_PriceUnit;
    QLineEdit *lineEdit_Quantity;
    QLineEdit *lineEdit_ID_Delete;
    QLineEdit *lineEdit_Category;
    QPushButton *pushButton_addProduct;
    QPushButton *pushButton_updateProduct;
    QPushButton *pushButton_deleteProduct;
    QPushButton *pushButton_viewProducts;
    QTableView *viewTable;
    QTableView *viewTable_Sales;
    // Sales Management UI Elements
    QLineEdit *lineEdit_ClientName;
    QComboBox *comboBox_Products;
    QLineEdit *lineEdit_PriceUnitInvoice;
    QLineEdit *lineEdit_QuantitySold;
    QComboBox *comboBox_DeliveryOptions;
    QWidget *salesWidget;
     QVBoxLayout *invoiceLayout;
    QSqlQueryModel *invoiceModel;
     QProgressBar *progressBar;
     QLabel *statusLabel;
     QTimer *timer;
    // Helper Functions
    /*void populateProductDropdown();
    double getPriceForProduct(const QString &productName);
    int getAvailableQuantity(const QString &productName);
    //void loadProductsIntoCheckboxes(QVBoxLayout *checkboxLayout);*/
    void displayInvoice(const QString &clientName, QSqlQueryModel *model);
    //void sendEmailWithAttachment(const QString &recipient, const QString &filePath);
    // Statistics chart components
    QChart *chart; // Chart to display the statistics
    QBarSeries *series; // Bar series for the chart
};

#endif // MAINWINDOW_H
