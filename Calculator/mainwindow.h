#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    QWidget* mainMenu;
    QVector<QPushButton*> allButtons;
    QLineEdit* mainText;
    QSpinBox* chooseNS;

    int operationId = 0;

    int firstNumNs = 16;
    int firstNumLhs = 0;
    double firstNumRhs = 0;

    int currentNS = 16;
    int currentNumLhs = 0;
    double currentNumRhs = 0;
    bool notConverted = true;
    QString curText;

    QString ConvertFromOneToOther(QString input, int target);
    void UpdateButtons();

private slots:
    void NumberInput();
    void InputNumS();
    void ClearInput();
    void ClearOneSimbol();

    void DoSomething();
    void ShowAnswer();
};
#endif // MAINWINDOW_H
