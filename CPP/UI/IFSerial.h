//  IFSerial.h
#pragma once
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QStringList>

class IFSerial: public QWidget
{
    Q_OBJECT
public:
    IFSerial(QWidget* parent = nullptr);
    enum class Status{OCIOSO, OCUPADO};
    enum class Fase{CONECTANDO, CAPTURANDO, SALVANDO};
private slots:
private:
    //  Componentes:
    QLabel *mLbStatus;
    QComboBox *mCbEquipo, *mCbPorta, *mCbBaud, *mCbPar, *mCbStop;
    QPushButton *mPbAgir, *mPbDeclinar, *mPbAtualizar;

    //  layout:
    QGroupBox *mGrupoHard;
    QFormLayout *mLayHard;
    QHBoxLayout *mLayBotoes;
    QVBoxLayout *mLayPrincipal;

    //  Controle:
    Status mEstado;
    Fase mFase;

    QStringList buscaPortas();
    void Povoar();
    void MontarLayout();
};