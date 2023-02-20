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
    int currentNS = 16;
    QSpinBox* chooseNS;

    int operationId = 0;
    QString firstNum;
    QString currentNum;
    int firstNumNs = 16;
    int firstNumDotPoint = 0;
    int currentNumDotPoint = 0;

    bool hasDot = false;

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
