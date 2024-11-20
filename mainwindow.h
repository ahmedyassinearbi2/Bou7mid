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
#include <QComboBox>
#include <QProgressBar>

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
    void on_pushButton_generatePDF_clicked();
    void onCalendarDateClicked(const QDate &date);



private:
    Ui::MainWindow *ui; // Pointer for the generated UI class

    // Product Management UI Elements
     QProgressBar *progressBar;
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

    // Sales Management UI Elements
    QLineEdit *lineEdit_ClientName;
    QComboBox *comboBox_Products;
    QLineEdit *lineEdit_PriceUnitInvoice;
    QLineEdit *lineEdit_QuantitySold;
    QComboBox *comboBox_DeliveryOptions;
    QWidget *salesWidget;
    // Helper Functions
    void populateProductDropdown();
    double getPriceForProduct(const QString &productName);
    int getAvailableQuantity(const QString &productName);
};

#endif // MAINWINDOW_H
