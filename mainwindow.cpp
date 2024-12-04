#include "connection.h"
#include "etudiant.h"
//#include "smtp.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFormLayout>
#include <QTableView>
#include <QHeaderView>
#include <QValidator>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QInputDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QCheckBox>
#include <QProgressBar>
#include <QTimer>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // Left navigation menu
    QVBoxLayout *menuLayout = new QVBoxLayout;
    QPushButton *buttonProducts = new QPushButton("Products");
    QPushButton *buttonSales = new QPushButton("Sales");
    QPushButton *buttonInvoice = new QPushButton("Invoice");
    QPushButton *buttonStatistic = new QPushButton("Statistic");
    // Inside MainWindow constructor
    QGroupBox *updateGroupBox = new QGroupBox("Update Product", this);
    QFormLayout *updateLayout = new QFormLayout;

    // Line edits for client name and new quantity
    QLineEdit *lineEdit_ClientName = new QLineEdit(this);
    lineEdit_ClientName->setPlaceholderText("Enter Client Name");

    QLineEdit *lineEdit_ProductName = new QLineEdit(this);
    lineEdit_ProductName->setPlaceholderText("Enter Product Name");

    QLineEdit *lineEdit_NewQuantity = new QLineEdit(this);
    lineEdit_NewQuantity->setPlaceholderText("Enter New Quantity");



    // Set button styles
    buttonProducts->setStyleSheet("QPushButton { background-color: #333300; color: white; font-weight: bold; border: none; padding: 10px; border-radius: 10px; }"
                                  "QPushButton:hover { background-color: #555500; }");
    buttonSales->setStyleSheet("QPushButton { background-color: #333300; color: white; font-weight: bold; border: none; padding: 10px; border-radius: 10px; }"
                               "QPushButton:hover { background-color: #555500; }");
    buttonInvoice->setStyleSheet("QPushButton { background-color: #333300; color: white; font-weight: bold; border: none; padding: 10px; border-radius: 10px; }"
                                 "QPushButton:hover { background-color: #555500; }");
    buttonStatistic->setStyleSheet("QPushButton { background-color: #333300; color: white; font-weight: bold; border: none; padding: 10px; border-radius: 10px; }"
                                 "QPushButton:hover { background-color: #555500; }");

    menuLayout->addWidget(buttonProducts);
    menuLayout->addWidget(buttonSales);
    menuLayout->addWidget(buttonInvoice);
    menuLayout->addWidget(buttonStatistic);
    menuLayout->addStretch();
    // Adding line edits to the form layout
    updateLayout->addRow("Product Name:", lineEdit_ProductName);
    updateLayout->addRow("New Quantity:", lineEdit_NewQuantity);
    updateLayout->addRow("Client Name:", lineEdit_ClientName);
    QPushButton *buttonUpdateProduct = new QPushButton("Update Product", this);
    QPushButton *buttonDeleteProduct = new QPushButton("Delete Product", this);
    QPushButton *buttonStatistics = new QPushButton("Statistics");
    menuLayout->addWidget(buttonStatistics);



    QWidget *menuWidget = new QWidget(this);
    menuWidget->setLayout(menuLayout);
    menuWidget->setFixedWidth(150);
    menuWidget->setStyleSheet("background-color: #222222; border-radius: 10px;");
    buttonDeleteProduct->setStyleSheet("QPushButton { background-color: #F44336; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                       "QPushButton:hover { background-color: #d32f2f; }");
    // Stacked widget for dynamic content
    QStackedWidget *stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget { background-color: #f0f0f0; border-radius: 10px; }");

    // Product Management Section
    QWidget *productWidget = new QWidget;
    QVBoxLayout *productLayout = new QVBoxLayout;

    // Tab widget
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet("QTabWidget::pane { border: none; }"
                             "QTabBar::tab { background: #555555; color: white; padding: 10px; border-radius: 10px; }"
                             "QTabBar::tab:selected { background: #333300; }");

    // Tab: Add/Update Product
    QWidget *addUpdateTab = new QWidget;
    QFormLayout *formLayout = new QFormLayout;

    // Input fields
    lineEdit_ID = new QLineEdit(this);
    lineEdit_ProductName = new QLineEdit(this);
    lineEdit_PriceUnit = new QLineEdit(this);
    lineEdit_Quantity = new QLineEdit(this);

    // Set input styles
    lineEdit_ID->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                               "QLineEdit:focus { border: 2px solid #555500; }");
    lineEdit_ProductName->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                                        "QLineEdit:focus { border: 2px solid #555500; }");
    lineEdit_PriceUnit->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                                      "QLineEdit:focus { border: 2px solid #555500; }");
    lineEdit_Quantity->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                                     "QLineEdit:focus { border: 2px solid #555500; }");

    // Set validators
    lineEdit_ID->setValidator(new QIntValidator(1, 99999, this));
    lineEdit_PriceUnit->setValidator(new QDoubleValidator(0.01, 99999.99, 2, this));
    lineEdit_Quantity->setValidator(new QIntValidator(1, 99999, this));

    // Add fields to form layout
    formLayout->addRow("ID (Optional):", lineEdit_ID);
    formLayout->addRow("Product Name:", lineEdit_ProductName);
    formLayout->addRow("Price per Unit:", lineEdit_PriceUnit);
    formLayout->addRow("Quantity:", lineEdit_Quantity);

    // Add/Update Buttons
    pushButton_addProduct = new QPushButton("Add Product", this);
    pushButton_updateProduct = new QPushButton("Update Product", this);
    pushButton_addProduct->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                         "QPushButton:hover { background-color: #45a049; }");
    pushButton_updateProduct->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                            "QPushButton:hover { background-color: #0b7dda; }");
    formLayout->addRow(pushButton_addProduct, pushButton_updateProduct);

    addUpdateTab->setLayout(formLayout);
    tabWidget->addTab(addUpdateTab, "Add/Update Product");

    // Tab: Delete Product
    QWidget *deleteTab = new QWidget;
    QVBoxLayout *deleteLayout = new QVBoxLayout;
    lineEdit_ID_Delete = new QLineEdit(this);
    lineEdit_ID_Delete->setPlaceholderText("Enter Product ID to delete");
    lineEdit_ID_Delete->setValidator(new QIntValidator(1, 99999, this));
    lineEdit_ID_Delete->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                                      "QLineEdit:focus { border: 2px solid #555500; }");
    pushButton_deleteProduct = new QPushButton("Delete Product", this);
    pushButton_deleteProduct->setStyleSheet("QPushButton { background-color: #F44336; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                            "QPushButton:hover { background-color: #d32f2f; }");
    deleteLayout->addWidget(new QLabel("Product ID:"));
    deleteLayout->addWidget(lineEdit_ID_Delete);
    deleteLayout->addWidget(pushButton_deleteProduct);
    deleteTab->setLayout(deleteLayout);
    tabWidget->addTab(deleteTab, "Delete Product");

    // Add tab widget to product layout
    productLayout->addWidget(tabWidget);
    productWidget->setLayout(productLayout);

    // Add productWidget to stackedWidget
    stackedWidget->addWidget(productWidget);

    // Sales Section
    QWidget *salesWidget = new QWidget;
    QVBoxLayout *salesLayout = new QVBoxLayout;
    QLabel *salesLabel = new QLabel("Sales Page");
    salesLabel->setAlignment(Qt::AlignCenter);
    salesLabel->setStyleSheet("font-size: 20px; color: #333300;");
    salesLayout->addWidget(salesLabel);

    // New View Products Section in Sales
    QWidget *viewProductsWidget = new QWidget;
    QVBoxLayout *viewLayout = new QVBoxLayout;

    pushButton_viewProducts = new QPushButton("View All Products", this);
    pushButton_viewProducts->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                           "QPushButton:hover { background-color: #0b7dda; }");
    viewTable = new QTableView(this);
    viewTable->setSortingEnabled(true);
    viewTable->horizontalHeader()->setStretchLastSection(true);
    viewTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    viewLayout->addWidget(pushButton_viewProducts);

    QPushButton *pushButton_addProductInInvoice = new QPushButton("Add Product in Invoice", this);
    pushButton_addProductInInvoice->setStyleSheet("QPushButton { background-color: #FF9800; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                                  "QPushButton:hover { background-color: #FB8C00; }");
    viewLayout->addWidget(pushButton_addProductInInvoice);

    viewLayout->addWidget(viewTable);
    viewProductsWidget->setLayout(viewLayout);

    salesLayout->addWidget(viewProductsWidget);
    salesWidget->setLayout(salesLayout);

    // Add salesWidget to stackedWidget
    stackedWidget->addWidget(salesWidget);
    // Add the button to the layout
    updateLayout->addWidget(buttonUpdateProduct);

    // Set the layout to the group box
    updateGroupBox->setLayout(updateLayout);

    // **Invoice Section**
    QWidget *invoiceWidget = new QWidget;
    QVBoxLayout *invoiceLayout = new QVBoxLayout;

    // Client Name Input

    lineEdit_ClientName->setPlaceholderText("Enter Client Name");
    QPushButton *buttonEnter = new QPushButton("Generate Invoice", this);
    QPushButton *buttonExportPDF = new QPushButton("Export to PDF", this);
    buttonExportPDF->setEnabled(false); // Disable until invoice is generated

    // Layout for client input
    QHBoxLayout *clientLayout = new QHBoxLayout;
    clientLayout->addWidget(lineEdit_ClientName);
    clientLayout->addWidget(buttonEnter);
    clientLayout->addWidget(buttonExportPDF);
    // Add to invoice layout
    invoiceLayout->addWidget(buttonUpdateProduct);
    invoiceLayout->addLayout(clientLayout);
    invoiceLayout->addWidget(updateGroupBox);
    // Delivery Option
    QCheckBox *deliveryCheckBox = new QCheckBox("Delivery Option", this);
    invoiceLayout->addWidget(deliveryCheckBox); // Add the checkbox to the layout

    // Invoice Display Section
    QLabel *invoiceTitle = new QLabel("Invoice", this);
    invoiceTitle->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center; color: #333300;");
    invoiceLayout->addWidget(invoiceTitle);

    // Placeholder for invoice details
    QLabel *invoiceDetails = new QLabel("Invoice details will be displayed here.", this);
    invoiceDetails->setWordWrap(true);
    invoiceDetails->setStyleSheet("font-size: 16px; padding: 10px; background-color: #e8f0fe; border: 1px solid #c1d3fe; border-radius: 5px; color: #333333;");
    invoiceLayout->addWidget(invoiceDetails);

    // Progress Bar and Status Label
    QProgressBar *progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    invoiceLayout->addWidget(progressBar);

    QLabel *statusLabel = new QLabel("Status: Not started", this);
    invoiceLayout->addWidget(statusLabel);
    invoiceLayout->addWidget(buttonDeleteProduct);
    // Timer for updating progress
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        int currentValue = progressBar->value();

        if (currentValue < 100) {
            currentValue += 25; // Increment by 25 for each stage
            progressBar->setValue(currentValue);

            // Update the status label based on progress
            switch (currentValue) {
            case 25:
                statusLabel->setText("Status: In Company");
                break;
            case 50:
                statusLabel->setText("Status: Out from Company");
                break;
            case 75:
                statusLabel->setText("Status: Close to Arrival");
                break;
            case 100:
                statusLabel->setText("Status: Delivered");
                timer->stop(); // Stop the timer when complete
                break;
            }
        }
    });

    // Connect the button to generate the invoice
    connect(buttonEnter, &QPushButton::clicked, [this, lineEdit_ClientName, invoiceDetails, buttonExportPDF, deliveryCheckBox, progressBar, statusLabel, timer]() {
        QString clientName = lineEdit_ClientName->text().trimmed();
        if (clientName.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Client name cannot be empty.");
            return;
        }

        // Check delivery option
        bool deliveryOption = deliveryCheckBox->isChecked();

        // Update the delivery option for the existing client
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE INVOICE SET delivery = :delivery WHERE client_name = :client_name");
        updateQuery.bindValue(":client_name", clientName);
        updateQuery.bindValue(":delivery", deliveryOption ? 1 : 0); // 1 for checked, 0 for unchecked

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Database Error", updateQuery.lastError().text());
            return;
        }

        // Reset progress bar and status label
        progressBar->setValue(0);
        statusLabel->setText("Status: Not started");

        // Start timer if delivery is selected
        if (deliveryOption) {
            timer->start(1000); // Update every second
        }

        // Now fetch the invoice details for the client
        QSqlQuery selectQuery;
        selectQuery.prepare("SELECT product_name, quantity_sold, price_total FROM INVOICE WHERE client_name = :client_name");
        selectQuery.bindValue(":client_name", clientName);
        if (!selectQuery.exec()) {
            QMessageBox::critical(this, "Database Error", selectQuery.lastError().text());
            return;
        }

        // Check if any invoice was found
        if (!selectQuery.next()) {
            invoiceDetails->setText("No invoice found for this client.");
            buttonExportPDF->setEnabled(false);
            return;
        }

        // Prepare invoice content
        QString invoiceText = "<b>Client:</b> " + clientName + "<br><br>";
        invoiceText += "<b>Products:</b><br>";

        double totalAmount = 0.0;
        do {
            QString productName = selectQuery.value(0).toString();
            int quantity = selectQuery.value(1).toInt();
            double priceTotal = selectQuery.value(2).toDouble();
            totalAmount += priceTotal;

            invoiceText += QString(" - %1 (Quantity: %2, Total: $%3)<br>")
                               .arg(productName)
                               .arg(quantity)
                               .arg(priceTotal, 0, 'f', 2);
        } while (selectQuery.next());

        invoiceText += QString("<br><b>Total Amount:</b> $%1").arg(totalAmount, 0, 'f', 2);
        invoiceDetails->setText(invoiceText);
        buttonExportPDF->setEnabled(true); // Enable PDF export
    });

    // Connect the button to export PDF
    connect(buttonExportPDF, &QPushButton::clicked, [this, invoiceDetails]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
        if (fileName.isEmpty()) {
            return; // User canceled the dialog
        }

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        QPainter painter(&printer);
        painter.setFont(QFont("Arial", 12));
        painter.drawText(100, 100, invoiceDetails->text()); // Position the text in the PDF
        painter.end();

        QMessageBox::information(this, "Success", "Invoice exported to PDF successfully!");
        // Send the email with the PDF attachment
        //sendEmailWithAttachment("arbiahmedyassine@gmail.com", fileName);
    });

    // Finalize layout
    invoiceWidget->setLayout(invoiceLayout);

    // Add invoiceWidget to stackedWidget
    stackedWidget->addWidget(invoiceWidget);

    // Add menuWidget and stackedWidget to mainLayout
    mainLayout->addWidget(menuWidget);
    mainLayout->addWidget(stackedWidget);
    mainLayout->setStretch(1, 5);

    // Set central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Connect navigation buttons to stackedWidget
    connect(buttonProducts, &QPushButton::clicked, [=]() {
        stackedWidget->setCurrentWidget(productWidget);
    });
    connect(buttonSales, &QPushButton::clicked, [=]() {
        stackedWidget->setCurrentWidget(salesWidget);
    });
    connect(buttonInvoice, &QPushButton::clicked, [=]() {
        stackedWidget->setCurrentWidget(invoiceWidget); // Switch to Invoice widget
    });

    // Keep existing button functionality
    connect(pushButton_addProduct, &QPushButton::clicked, this, &MainWindow::on_pushButton_addProduct_clicked);
    connect(pushButton_updateProduct, &QPushButton::clicked, this, &MainWindow::on_pushButton_updateProduct_clicked);
    connect(pushButton_deleteProduct, &QPushButton::clicked, this, &MainWindow::on_pushButton_deleteProduct_clicked);
    connect(pushButton_viewProducts, &QPushButton::clicked, this, &MainWindow::on_pushButton_viewProducts_clicked);
    connect(pushButton_addProductInInvoice, &QPushButton::clicked, this, &MainWindow::on_pushButton_addProductInInvoice_clicked);
    connect(buttonUpdateProduct, &QPushButton::clicked, this, &MainWindow::on_pushButton_updateInvoiceProduct_clicked);
    connect(buttonDeleteProduct, &QPushButton::clicked, this, &MainWindow::on_pushButton_deleteInvoiceProduct_clicked);
}

MainWindow::~MainWindow() {}

void MainWindow::on_pushButton_addProduct_clicked()
{
    int id = lineEdit_ID->text().isEmpty() ? 0 : lineEdit_ID->text().toInt();
    QString productName = lineEdit_ProductName->text();
    double priceUnit = lineEdit_PriceUnit->text().toDouble();
    int quantity = lineEdit_Quantity->text().toInt();

    if (productName.isEmpty() || priceUnit <= 0 || quantity <= 0) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled with valid data.");
        return;
    }

    Product product(id, productName, priceUnit, quantity);
    if (product.addProduct()) {
        QMessageBox::information(this, "Success", "Product added successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the product.");
    }
}

void MainWindow::on_pushButton_updateProduct_clicked()
{
    int id = lineEdit_ID->text().toInt();
    QString productName = lineEdit_ProductName->text();
    double priceUnit = lineEdit_PriceUnit->text().toDouble();
    int quantity = lineEdit_Quantity->text().toInt();

    if (id <= 0 || productName.isEmpty() || priceUnit <= 0 || quantity < 0) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled with valid data.");
        return;
    }

    Product product(id, productName, priceUnit, quantity);

    if (product.updateProduct()) {
        QMessageBox::information(this, "Success", "Product updated successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to update the product.");
    }
}

void MainWindow::on_pushButton_deleteProduct_clicked()
{
    int id = lineEdit_ID_Delete->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid ID.");
        return;
    }

    Product product;
    product.setId(id);
    if (product.deleteProduct()) {
        QMessageBox::information(this, "Success", "Product deleted successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete the product.");
    }
}

void MainWindow::on_pushButton_viewProducts_clicked()
{
    Product product;
    QSqlQueryModel *model = product.getAllProducts();

    if (model) {
        // Add a new column for the checkbox
        model->insertColumn(0);

        viewTable->setModel(model);

        // Set the header data
        model->setHeaderData(0, Qt::Horizontal, "Select");
        model->setHeaderData(1, Qt::Horizontal, "Product Name");
        model->setHeaderData(2, Qt::Horizontal, "Price per Unit");
        model->setHeaderData(3, Qt::Horizontal, "Quantity");

        // Set the checkbox column width
        viewTable->setColumnWidth(0, 50);

        // Enable checkbox selection
        viewTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        viewTable->setSelectionMode(QAbstractItemView::MultiSelection);
    } else {
        QMessageBox::critical(this, "Error", "Failed to retrieve product list.");
    }
}

void MainWindow::on_pushButton_addProductInInvoice_clicked()
{
    // Prompt for CLIENT_NAME
    bool ok;
    QString clientName = QInputDialog::getText(this, tr("Client Name"),
                                               tr("Enter Client Name:"), QLineEdit::Normal,
                                               "", &ok);
    if (!ok || clientName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Client name cannot be empty.");
        return;
    }

    // Get the selected rows
    QItemSelectionModel *selectionModel = viewTable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select at least one product to add to the invoice.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO INVOICE (client_name, product_name, quantity_sold, price_total) VALUES (:client_name, :product_name, :quantity_sold, :price_total)");

    for (const QModelIndex &index : selectedRows) {
        // Get the product name and available quantity from the selected row
        QString productName = viewTable->model()->data(viewTable->model()->index(index.row(), 1)).toString();
        int availableQuantity = viewTable->model()->data(viewTable->model()->index(index.row(), 3)).toInt();
        double pricePerUnit = viewTable->model()->data(viewTable->model()->index(index.row(), 2)).toDouble();

        // Prompt for quantity sold
        bool quantityOk;
        int quantitySold = QInputDialog::getInt(this, tr("Quantity Sold"),
                                                tr("Enter Quantity Sold for ") + productName,
                                                1, 1, 10000, 1, &quantityOk);
        if (!quantityOk) {
            QMessageBox::warning(this, "Input Error", "Quantity sold input was cancelled.");
            return;
        }

        // Check if the quantity sold exceeds the available quantity
        if (quantitySold > availableQuantity) {
            QMessageBox::warning(this, "Insufficient Quantity",
                                 "Product quantity not enough for " + productName + ". Available: " + QString::number(availableQuantity));
            continue;
        }

        // Calculate total price
        double priceTotal = quantitySold * pricePerUnit;

        // Bind values for the query
        query.bindValue(":client_name", clientName);
        query.bindValue(":product_name", productName);
        query.bindValue(":quantity_sold", quantitySold);
        query.bindValue(":price_total", priceTotal);

        if (!query.exec()) {
            QMessageBox::critical(this, "Error", "Failed to add the selected products to the invoice: " + query.lastError().text());
            return;
        }
    }

    QMessageBox::information(this, "Success", "The selected products have been added to the invoice.");
}
/*void MainWindow::sendEmailWithAttachment(const QString &recipient, const QString &filePath) {
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);
    smtp.setUser("your_email@gmail.com"); // Your email
    smtp.setPassword("your_password"); // Your email password or app password

    MimeMessage message;
    message.setSender(new EmailAddress("your_email@gmail.com"));
    message.addRecipient(new EmailAddress(recipient));
    message.setSubject("Invoice");

    MimeText text;
    text.setText("Please find the attached invoice.");
    message.addPart(&text);

    // Attach the PDF file (optional)
    MimeAttachment attachment(new QFile(filePath));
    message.addPart(&attachment);

    if (!smtp.connectToHost()) {
        QMessageBox::warning(this, "Error", "Could not connect to SMTP server.");
        return;
    }

    if (!smtp.login()) {
        QMessageBox::warning(this, "Error", "Could not log in to SMTP server.");
        return;
    }

    if (smtp.sendMail(message)) {
        QMessageBox::information(this, "Success", "Invoice sent successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to send email: " + smtp.getError());
    }

    smtp.quit();
}
void MainWindow::handleEmailReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "Success", "Invoice sent successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to send email: " + reply->errorString());
    }
    reply->deleteLater();
}*/
void MainWindow::on_pushButton_updateInvoiceProduct_clicked() {
    QString productName = lineEdit_ProductName->text().trimmed();
    QString clientName = lineEdit_ClientName->text().trimmed();
    QString newQuantityStr = lineEdit_NewQuantity->text().trimmed();

    // Validate the inputs
    if (productName.isEmpty() || clientName.isEmpty() || newQuantityStr.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled out.");
        return;
    }

    bool ok;
    int newQuantity = newQuantityStr.toInt(&ok);
    if (!ok || newQuantity < 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid quantity.");
        return;
    }

    // Debug output to check values
    qDebug() << "Updating invoice:";
    qDebug() << "Product Name:" << productName;
    qDebug() << "Client Name:" << clientName;
    qDebug() << "New Quantity:" << newQuantity;

    // Prepare and execute the SQL update query for the invoice table
    QSqlQuery query;
    query.prepare("UPDATE invoice SET quantity_sold = :quantity_sold WHERE product_name = :product_name AND client_name = :client_name");
    query.bindValue(":product_name", productName);
    query.bindValue(":quantity_sold", newQuantity);
    query.bindValue(":client_name", clientName);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "Invoice updated successfully.");
        // Clear the input fields after successful update
        lineEdit_ProductName->clear();
        lineEdit_ClientName->clear();
        lineEdit_NewQuantity->clear();
    }
}


void MainWindow::on_pushButton_deleteInvoiceProduct_clicked() {
    QString productName = lineEdit_ProductName->text().trimmed();
    QString clientName = lineEdit_ClientName->text().trimmed();

    // Validate the inputs
    if (productName.isEmpty() || clientName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Product name and client name must be provided.");
        return;
    }

    // Ensure the database is open
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not established.");
        return;
    }

    // Prepare and execute the SQL delete query for the invoice table
    QSqlQuery query;
    query.prepare("DELETE FROM invoice WHERE product_name = :product_name AND client_name = :client_name");
    query.bindValue(":product_name", productName);
    query.bindValue(":client_name", clientName);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "Product removed from invoice successfully.");
        // Clear the input fields after successful deletion
        lineEdit_ProductName->clear();
        lineEdit_ClientName->clear();
    }
}
