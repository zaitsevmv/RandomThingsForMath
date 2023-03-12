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
    if(notConverted){
        int a = 0;
        QString lhs;
        QVector<int> rhs;
        while(input[a] != '.'){
            lhs += input[a];
            a++;
        }
        bool ok;
        currentNumLhs = lhs.toInt(&ok, currentNS);
        for(int i = a+1; i < input.size(); i++){
            int num = input[i].unicode();
            num -= 48;
            if(num > 9){
                num -= 7;
            }
            rhs.push_back(num);
        }
        double sumRhs;
        for(int i = 1; i <= rhs.size(); i++){
            sumRhs += rhs[i-1]/(std::pow(currentNS,i));
            if(i == 5){
                break;
            }
        }
        currentNumRhs = sumRhs;
        notConverted = false;
    }
    QString ans;
    if(target != 10){
        ans = QString::number(currentNumLhs,target);
        if(currentNumRhs != 0){
            ans += '.';
            double rhs = currentNumRhs;
            for(int i = 0; i < 3; i++){
                double c = rhs * target;
                int lhs = std::floor(c);
                rhs = c - lhs;
                ans += QString::number(lhs,target);
            }
        }
        ans = ans.toUpper();
        return ans;
    }
    double decNum = currentNumLhs + currentNumRhs;
    ans = QString::number(decNum);
    mainText->setText(ans);
    return ans;
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
    if(mainText->text() == "0" && inputBtn != '.'){
        ClearInput();
    }
    QString topText = mainText->text() + inputBtn;
    mainText->setText(topText);
    curText += inputBtn;
    notConverted = true;
}

void MainWindow::InputNumS()
{
    QString cur = ConvertFromOneToOther(curText,chooseNS->value());
    mainText->setText(cur);
    currentNS = chooseNS->value();
    UpdateButtons();
}

void MainWindow::ClearInput()
{
    notConverted = true;
    curText.clear();
    mainText->clear();
}

void MainWindow::ClearOneSimbol()
{
    QString num = mainText->text();
    if(num.size() != 0){
        num.resize(num.size()-1);
        mainText->setText(num);
        curText.resize(curText.size()-1);
        notConverted = true;
    }
}

QString HaveAnswer(double lhs, double rhs, int opId)
{
    qDebug() << lhs << " " << rhs;
    double ans;
    switch (opId) {
    case 1:
        ans = lhs + rhs;
        break;
    case 2:
        ans = lhs - rhs;
        break;
    case 3:
        ans = lhs * rhs;
        break;
    case 4:
        ans = lhs / rhs;
        break;
    case 5:
        ans = std::pow(lhs,rhs);
        break;
    default:
        break;
    }
    return QString::number(ans);
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
    ConvertFromOneToOther(curText, currentNS);

    firstNumLhs = currentNumLhs;
    firstNumRhs = currentNumRhs;
    firstNumNs = currentNS;

    currentNumLhs = 0;
    currentNumRhs = 0;
    notConverted = true;
    mainText->clear();
    curText.clear();
}

void MainWindow::ShowAnswer()
{
    ConvertFromOneToOther(curText, currentNS);
    curText = HaveAnswer(firstNumRhs + firstNumLhs, currentNumRhs + currentNumLhs, operationId);
    int tempNS = currentNS;
    currentNS = 10;
    firstNumLhs = 0;
    firstNumRhs = 0;
    firstNumNs = 0;

    notConverted = true;
    qDebug() << curText << currentNS;
    curText = ConvertFromOneToOther(curText, tempNS);
    mainText->setText(curText);
}

