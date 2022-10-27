#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QMainWindow>

// this file is created by QT from the file MainWindow.ui
#include "ui_MainWindow.h"
#include "AlgebraicTreeExpression.hpp"

#define MAX 512
// add other header files that you may need here

// #include "AlgebraicTreeExpression.hpp"

// Since this is a subclass of Ui::MainWindow (in addition to QMainWindow)
// all the widgets dfined in Ui::MainWindow become members of my class.

// The class Ui::MainWindow is defined in "ui_MainWindow.h".
// The "MainWindow" name in Ui:MainWindow corresponds to the name of the window
// defined in the .ui file, not from the name of the .ui file itself. 

class MainWindow : public QMainWindow, public Ui::MainWindow {

  // this macro adds Signal/Slot support to this class,
  // only add this macro to classes that require signal/slot processing.

  Q_OBJECT

public:

  MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
  ~MainWindow();

public slots:

  // by naming the slots
  //    on_ + widget name + _ + signal name 
  // Qt connects them automatically
  //
  // if you put any other name to the slot, you have
  // to connect them explicitely on the code by calling Qt::connect(....)

  // menu actions
  void on_quit_action_triggered();
  void on_about_action_triggered();

  // push-button actions
  void on_number0Button_clicked();
  void on_number1Button_clicked();
  void on_number2Button_clicked();
  void on_number3Button_clicked();
  void on_number4Button_clicked();
  void on_number5Button_clicked();
  void on_number6Button_clicked();
  void on_number7Button_clicked();
  void on_number8Button_clicked();
  void on_number9Button_clicked();
  void on_addButton_clicked();
  void on_subtractButton_clicked();
  void on_multiplyButton_clicked();
  void on_divideButton_clicked();
  void on_leftParenButton_clicked();
  void on_rightParenButton_clicked();
  void on_decimalPointButton_clicked();
  void on_changeSignButton_clicked();
  void on_clearButton_clicked();
  void on_evaluateButton_clicked();

  void on_firstExprButton_clicked();
  void on_prevExprButton_clicked();
  void on_nextExprButton_clicked();
  void on_lastExprButton_clicked();
  void on_delExprButton_clicked();

    bool _expressionInHeap(string expr);
    bool _rootInHeap(AlgebraicTreeExpression* cur_root);
    
private:

    vector<AlgebraicTreeExpression*> root_array;
    vector<string> str_array;
    int heap_length = 0;
    
  // add private variables and functions that you need here

};

#endif  /* __MAINWINDOW_HPP__ */
