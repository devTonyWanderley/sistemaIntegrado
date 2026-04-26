//	ConfigSerialWidget.cpp
#include "ConfigSerialWidget.h"
#include "ui_ConfigSerialWidget.h"
#include <windows.h>

ConfigSerialWidget::ConfigSerialWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ConfigSerialWidget) {
    ui->setupUi(this);
    atualizarPortas();
}

ConfigSerialWidget::~ConfigSerialWidget() {
    delete ui;
}

void ConfigSerialWidget::atualizarPortas() {
    ui->comboPorta->clear();
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char nome[255], dado[255];
        DWORD i = 0, nLen, dLen, tipo;
        while (RegEnumValueA(hKey, i++, nome, &(nLen=255), NULL, &tipo, (LPBYTE)dado, &(dLen=255)) == ERROR_SUCCESS) {
            ui->comboPorta->addItem(dado);
        }
        RegCloseKey(hKey);
    }
}

int ConfigSerialWidget::obterPortaSelecionada() const {
    QString porta = ui->comboPorta->currentText();
    if (porta.isEmpty()) return -1;
    return porta.mid(3).toInt(); // Remove "COM" e pega o número
}

ConfigSerial ConfigSerialWidget::obterConfiguracao() const {
    ConfigSerial cfg;
    cfg.baudRate = ui->comboBaud->currentText().toUInt();
    cfg.byteSize = 8;
    
    // Mapeamento dos enums que criamos na LIB/COM
    cfg.paridade = static_cast<Paridade>(ui->comboParidade->currentIndex());
    cfg.stopBits = static_cast<StopBits>(ui->comboStopBits->currentIndex());
    
    return cfg;
}
