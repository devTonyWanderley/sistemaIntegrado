//  mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../LIB/COM/portaSerialWin.h"
#include "../LIB/QTW/ConfigSerialWidget.h" // Ajuste o caminho se necessário
#include "../LIB/TIP/tipConfig.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void on_btnConfigManual_clicked();
    void on_btnIniciarDownload_clicked();
    void on_btnSalvar_clicked();

private:
    Ui::MainWindow *ui;
    PortaSerialWin m_motor;
    ConfigSerialWidget *m_configWidget;
    QTimer *m_timerLeitura;
    QByteArray m_dadosBrutos;
};
#endif // MAINWINDOW_H
