#include "mainwindow.h"
#include <QGridLayout>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainMenu = new QWidget;
    QGridLayout* thisLayout = new QGridLayout;
    QVBoxLayout* forTextLayout = new QVBoxLayout;
    QSize commonSize(50,50);
    chooseNS = new QSpinBox;
    chooseNS->setMaximum(16);
    chooseNS->setMinimum(2);
    chooseNS->setValue(16);
    chooseNS->setFixedSize(commonSize);
    connect(chooseNS, SIGNAL(valueChanged(int)),this,SLOT(InputNumS()));

    mainText = new QLineEdit;
    mainText->setFixedHeight(50);
    mainText->setReadOnly(true);
    mainText->setAlignment(Qt::AlignRight);
    forTextLayout->addWidget(mainText,Qt::AlignCenter);
    int a = 1, b = 0, curNum = 1, row = 0;
    for(int i = 0; i < 28; i++){
        allButtons.push_back(new QPushButton);
        allButtons[i]->setFixedSize(commonSize);
    }
    for(int i = 0; i < 24; i++){
        if((i+1) % 4 != 0 && curNum < 16){
            QString btnText = QString::number(curNum);
            if(curNum >= 10){
                btnText = (char)(curNum+55);
            }
            allButtons[i]->setText(btnText);
            connect(allButtons[i], SIGNAL(clicked()),this, SLOT(NumberInput()));
            curNum++;
        } else{
            switch (row) {
            case 0:
                allButtons[i]->setText("+");
                connect(allButtons[i], SIGNAL(clicked()), this, SLOT(DoSomething()));
                break;
            case 1:
                allButtons[i]->setText("-");
                connect(allButtons[i], SIGNAL(clicked()), this, SLOT(DoSomething()));
                break;
            case 2:
                allButtons[i]->setText("/");
                connect(allButtons[i], SIGNAL(clicked()), this, SLOT(DoSomething()));
                break;
            case 3:
                allButtons[i]->setText("*");
                connect(allButtons[i], SIGNAL(clicked()), this, SLOT(DoSomething()));
                break;
            case 4:
                allButtons[i]->setText("^");
                connect(allButtons[i], SIGNAL(clicked()), this, SLOT(DoSomething()));
                break;
            case 5:
                allButtons[i]->setText("00");
                connect(allButtons[i], SIGNAL(clicked()),this, SLOT(NumberInput()));
                allButtons[i+1]->setText("0");
                connect(allButtons[i+1], SIGNAL(clicked()),this, SLOT(NumberInput()));
                allButtons[i+2]->setText(".");
                connect(allButtons[i+2], SIGNAL(clicked()),this, SLOT(NumberInput()));
                allButtons[i+3]->setText("=");
                connect(allButtons[i+3], SIGNAL(clicked()), this, SLOT(ShowAnswer()));
                break;
            default:
                break;
            }
            row++;
        }
        thisLayout->addWidget(allButtons[i],a,b, Qt::AlignCenter);
        b++;
        if((i+1) % 4 == 0){
            a++;
            b = 0;
        }
    }
    allButtons[24]->setText("C");
    connect(allButtons[24], SIGNAL(clicked()),this, SLOT(ClearInput()));
    allButtons[25]->setText("");
    allButtons[25]->setEnabled(false);

    allButtons[26]->setText("<-");
    connect(allButtons[26], SIGNAL(clicked()),this, SLOT(ClearOneSimbol()));
    thisLayout->addWidget(allButtons[24],0,0, Qt::AlignCenter);
    thisLayout->addWidget(chooseNS,0,1, Qt::AlignCenter);
    thisLayout->addWidget(allButtons[25],0,2, Qt::AlignCenter);
    thisLayout->addWidget(allButtons[26],0,3, Qt::AlignCenter);

    forTextLayout->addLayout(thisLayout);
    mainMenu->setLayout(forTextLayout);
    this->setCentralWidget(mainMenu);
}

QString MainWindow::ConvertFromOneToOther(QString input, int target)
{
    bool ok;
    int normalNum = input.toInt(&ok, currentNS);
    return QString::number(normalNum,target);
}

void MainWindow::UpdateButtons()
{
    int k = currentNS;
    int cur = 1;
    for(int i = 0; i < 20; i++){
        if((i+1) % 4 != 0){
            if(cur < k){
                allButtons[i]->setEnabled(true);
                cur++;
            } else{
                allButtons[i]->setEnabled(false);
                cur++;
            }
        }
    }
}

void MainWindow::NumberInput()
{
    QString inputBtn = qobject_cast<QPushButton*>(sender())->text();
    if(mainText->text() == "0.0"){
        mainText->clear();
    }
    if(mainText->text() == "0"){
        mainText->clear();
    }
    if(inputBtn != "." && !hasDot){
        QString topText = mainText->text() + inputBtn;
        mainText->setText(topText);
        currentNum += inputBtn;
    } else if(inputBtn == "." && !hasDot){
        QString topText = mainText->text() + inputBtn;
        mainText->setText(topText);
        hasDot = true;
    } else if(inputBtn != "." && hasDot){
        QString topText = mainText->text() + inputBtn;
        mainText->setText(topText);
        currentNum += inputBtn;
    }
    if(hasDot){
        currentNumDotPoint++;
    }
}

void MainWindow::InputNumS()
{
    QString output = ConvertFromOneToOther(currentNum, chooseNS->value());
    currentNS = chooseNS->value();
    UpdateButtons();
    currentNum = output;
    mainText->setText(output);
}

void MainWindow::ClearInput()
{
    UpdateButtons();
    currentNum.clear();
    hasDot = false;
    currentNumDotPoint = 0;
    mainText->clear();
}

void MainWindow::ClearOneSimbol()
{
    QString num = mainText->text();
    if(num.size() != 0){
        num.resize(num.size()-1);
        mainText->setText(num);
        if(hasDot){
            if(currentNumDotPoint > 0){
                currentNumDotPoint--;
            } else{
                hasDot = false;
            }
            currentNum.resize(currentNum.size()-1);
            UpdateButtons();
        } else{
            currentNum.resize(currentNum.size()-1);
        }
    }
}

QString HaveAnswer(QString lhs, QString rhs, int opId)
{
    qDebug() << lhs << " " << rhs;
    QString ans;
    bool ok;
    switch (opId) {
    case 1:
        ans = QString::number((lhs.toInt() + rhs.toInt()),10);
        break;
    case 2:
        ans = QString::number((lhs.toInt() - rhs.toInt()),10);
        break;
    case 3:
        ans = QString::number(lhs.toInt() * rhs.toInt(),10);
        break;
    case 4:
        ans = QString::number(lhs.toInt() / rhs.toInt(),10);
        break;
    case 5:
        ans = QString::number(std::pow(lhs.toInt(), rhs.toInt()));
        break;
    default:
        break;
    }
    return ans;
}

void MainWindow::DoSomething()
{
    QString inputBtn = qobject_cast<QPushButton*>(sender())->text();
    if(inputBtn == "/"){
        operationId = 4;
    } else if(inputBtn == "*"){
        operationId = 3;
    } else if(inputBtn == "+"){
        operationId = 1;
    } else if(inputBtn == "-"){
        operationId = 2;
    } else if(inputBtn == "^"){
        operationId = 5;
    }
    firstNum = currentNum;
    firstNumDotPoint = currentNumDotPoint;
    firstNumNs = currentNS;

    mainText->clear();
    currentNum.clear();
    hasDot = false;
    currentNumDotPoint = 0;
}

void MainWindow::ShowAnswer()
{
    QString lhs = ConvertFromOneToOther(firstNum, 10);
    QString rhs = ConvertFromOneToOther(currentNum, 10);
    QString ans = HaveAnswer(lhs, rhs, operationId);
    operationId = 0;
    currentNum = ans;
    currentNS = 10;
    chooseNS->setValue(10);
    UpdateButtons();
    mainText->setText(ans);
}

