//	GUISerial.hpp
#pragma once
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

class GuiSerial: public QWidget
{
    Q_OBJECT
public:
    GuiSerial(QWidget* parent = nullptr);
private slots:
private:
    QComboBox *mCbEqui, *mCbPorta, *mCbBaud, *mCbPar, *mCbStop, *mCbSize;
    QPushButton *mPbAgir, *mPbDeclinar;
    QLabel *mLbStatus;
    QGroupBox *mGbHard, mGbControle;
    QFormLayout *mFlHard;
    QHBoxLayout *mHblBotoes;
    QVBoxLayout *mVblMain;

    void Preencher();
    void MontarLay();
};