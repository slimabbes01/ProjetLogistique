#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QTabWidget>
#include <QTableView>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QPdfWriter>
#include <QTextDocument>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Tabs
    QTabWidget *tabWidget;
    QWidget *tabClients;
    QWidget *tabCommandes;
    QWidget *tabStatistics; // <-- Statistics tab
    QWidget *tabStatisticsCommandes;
    QWidget *tabStatisticsClients;


    // Clients
    QTableView *tableViewClients;
    QLineEdit *lineEditNom;
    QLineEdit *lineEditPrenom;
    QLineEdit *lineEditAdresse;
    QLineEdit *lineEditTelephone;
    QLineEdit *lineEditEmail;
    QPushButton *pushButtonAddClient;
    QPushButton *pushButtonUpdateClient;
    QPushButton *pushButtonDeleteClient;
    QPushButton *pushButtonSearchClient;
    QPushButton *pushButtonExportClientsPdf;

    // Commandes
    QTableView *tableViewCommandes;
    QLineEdit *lineEditDescription;
    QLineEdit *lineEditMontant;
    QComboBox *comboBoxClient;
    QComboBox *comboBoxStatut;
    QDateEdit *dateEditCommande;
    QPushButton *pushButtonAddCommande;
    QPushButton *pushButtonUpdateCommande;
    QPushButton *pushButtonDeleteCommande;
    QPushButton *pushButtonSearchCommande;
    QPushButton *pushButtonExportCommandesPdf;

    // Models
    QSqlTableModel *modelClients;
    QSqlTableModel *modelCommandes;

    // Setup functions
    void setupUI();
    void applyStyling();
    void loadClients();
    void loadCommandes();
    void loadClientComboBox();
    void setupStatisticsTabs();




    // Validation
    bool validateClientInputs();
    bool validateCommandeInputs();

private slots:
    // Client slots
    void addClient();
    void updateClient();
    void deleteClient();
    void searchClient();
    void populateClientForm(const QModelIndex &current, const QModelIndex &previous);

    // Commande slots
    void addCommande();
    void updateCommande();
    void deleteCommande();
    void searchCommande();
    void populateCommandeForm(const QModelIndex &current, const QModelIndex &previous);

    // PDF slots
    void onExportClientsPdfClicked();
    void onExportCommandesPdfClicked();
    void exportClientsToPdf(const QString &filePath);
    void exportCommandesToPdf(const QString &filePath);

};

#endif // MAINWINDOW_H
