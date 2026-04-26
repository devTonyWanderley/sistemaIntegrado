//	ConfigSerialWidget.h
#pragma once

#include <QWidget>
#include "MotorComunicacao.h" // Para acessar a struct ConfigSerial

namespace Ui { class ConfigSerialWidget; }

class ConfigSerialWidget : public QWidget {
    Q_OBJECT

public:
    explicit ConfigSerialWidget(QWidget *parent = nullptr);
    ~ConfigSerialWidget();

    // Retorna a struct preenchida com o que o usuário selecionou na tela
    ConfigSerial obterConfiguracao() const;
    
    // Retorna o número da porta (ex: 3 se selecionado "COM3")
    int obterPortaSelecionada() const;

public slots:
    void atualizarPortas(); // Varre o registro do Windows

private:
    Ui::ConfigSerialWidget *ui;
};
