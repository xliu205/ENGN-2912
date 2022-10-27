#include "MainWindow.hpp"
#include "AboutDialog.hpp"

// remember that if you add new variables to the class, you need to
// modify your constructors to initialize them

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags): 
  QMainWindow(parent, flags) {
  // this line has to be the first thing here.  It creates the GUI
  // from the MainWindow.ui file stored in the forms directory
      
  setupUi(this);
      firstExprButton->setDisabled(true);
      prevExprButton->setDisabled(true);
      nextExprButton->setDisabled(true);
      lastExprButton->setDisabled(true);
      delExprButton->setDisabled(true);
}

MainWindow::~MainWindow() {
}


// menu bar actions

void MainWindow::on_quit_action_triggered() {
    str_array.clear();
    root_array.clear();
  close();
  QApplication::quit();
}

void MainWindow::on_about_action_triggered() {
  AboutDialog dialog(this);
  dialog.exec();
}

// buttons
void MainWindow::on_number0Button_clicked() {
  displayTextEdit->insertPlainText(QString("0"));
}

void MainWindow::on_number1Button_clicked() {
  displayTextEdit->insertPlainText(QString("1"));
}

void MainWindow::on_number2Button_clicked() {
  displayTextEdit->insertPlainText(QString("2"));
}

void MainWindow::on_number3Button_clicked() {
  displayTextEdit->insertPlainText(QString("3"));
}

void MainWindow::on_number4Button_clicked() {
  displayTextEdit->insertPlainText(QString("4"));
}

void MainWindow::on_number5Button_clicked() {
  displayTextEdit->insertPlainText(QString("5"));
}

void MainWindow::on_number6Button_clicked() {
  displayTextEdit->insertPlainText(QString("6"));
}

void MainWindow::on_number7Button_clicked() {
  displayTextEdit->insertPlainText(QString("7"));
}

void MainWindow::on_number8Button_clicked() {
  displayTextEdit->insertPlainText(QString("8"));
}

void MainWindow::on_number9Button_clicked() {
  displayTextEdit->insertPlainText(QString("9"));
}

void MainWindow::on_addButton_clicked() {
  displayTextEdit->insertPlainText(QString("+"));
}

void MainWindow::on_subtractButton_clicked() {
  displayTextEdit->insertPlainText(QString("-"));
}

void MainWindow::on_multiplyButton_clicked() {
  displayTextEdit->insertPlainText(QString("*"));
}

void MainWindow::on_divideButton_clicked() {
  displayTextEdit->insertPlainText(QString("/"));
}

void MainWindow::on_leftParenButton_clicked() {
  displayTextEdit->insertPlainText(QString("("));
}

void MainWindow::on_rightParenButton_clicked() {
  displayTextEdit->insertPlainText(QString(")"));
}

void MainWindow::on_decimalPointButton_clicked() {
  displayTextEdit->insertPlainText(QString("."));
}

void MainWindow::on_changeSignButton_clicked() {

  // get from you QtCalc1 implementation
    // get current expression
    QString qStr = displayTextEdit->toPlainText();
    
    if (qStr.isEmpty()) {
        qStr.insert(0, '-');
    } else {
        if(qStr[qStr.length()-1] == '+')
            qStr.replace(qStr.length()-1, 1, '-');
        else if(qStr[qStr.length()-1] == '-')
            qStr.replace(qStr.length()-1, 1, '+');
        else
            qStr.append('-');
    }
    
    // clear the display and show the edited expression
    displayTextEdit->clear();
    displayTextEdit->insertPlainText(qStr);

}

void MainWindow::on_clearButton_clicked() {
  displayTextEdit->clear();
  QString qStr = "<font color=\"black\"></font>";
    displayTextEdit->setTextColor(QColor().black());
  displayTextEdit->insertHtml(qStr);
}

void MainWindow::on_evaluateButton_clicked() {

  // get from your QtCalc1 implementation, and modify so that:

  // - allocate the root node in the heap
  //   and save it in an appropriate container (vector, list, or set)
  //
  // - add buttons to retrieve first, previous, next, and last expression
  //
  // - add a button to remove the expressions shown in the display
  //   from the container
  //
  // - make sure that you don't save duplicate expressions in the container
  //
  // - buttons that operate on the stored expressions must be disabled
  //   the container is empty, and enabled when the container contains
  //   at least one expression
    QString qStr = displayTextEdit->toPlainText();
    
    // convert to C-style string
    // using QString::toLocal8Bit();
    // or
    // QString::toStdString();
    string input_str = qStr.toStdString();
    const char* expStr = input_str.c_str();
    string input = string(expStr);
    // create AlgebraicTreeExpression
    AlgebraicTreeExpression* root;
//    if(_expressionInHeap(input)){
//        std::vector<string>::iterator it = std::find(str_array.begin(), str_array.end(), input);
//        int index = distance(str_array.begin(), it);
//        root = root_array.at(index);
//    }else
        root = new AlgebraicTreeExpression(expStr);
    if(root->isEmpty()){
        int length = qStr.length();
        QString qStrParsed = qStr.left(root->getLength());
        QString qStrNotParsed = qStr.right(length-root->getLength());
        displayTextEdit->clear();
        QString qStr =
        "<font color=\"black\">" + qStrParsed + "</font>"+
        "<font color=\"red\">" + qStrNotParsed + "</font>";
        displayTextEdit->insertHtml(qStr);
    }else if(!root->isEmpty() && root->getrightParenthese()!=0){
        //        int length = qStr.length();
        QString qStrParsed = qStr.left(root->getLength());
        QString qStrNotParsed = QString::fromStdString("_");
        displayTextEdit->clear();
        qStr =
        "<font color=\"black\">" + qStrParsed + "</font>"+
        "<font color=\"red\">" + qStrNotParsed + "</font>";
        displayTextEdit->insertHtml(qStr);
    }
    else{
//        if(!_expressionInHeap(input)){
//            heap_length++;
//            str_array.push_back(input);
//            root_array.push_back(root);
//        }
        if(!_rootInHeap(root)){
            root_array.push_back(root);
            str_array.push_back(input);
        }
        if(!root_array.empty()){
            firstExprButton->setDisabled(false);
            prevExprButton->setDisabled(false);
            nextExprButton->setDisabled(false);
            lastExprButton->setDisabled(false);
            delExprButton->setDisabled(false);
        }
        double value = root->evaluate();
        QString valueAsString = QString::number(value);
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + valueAsString + "</font>";
        displayTextEdit->insertHtml(valueAsString);
    }
}

void MainWindow::on_firstExprButton_clicked() {
  // implement
    if(str_array.size()!=0){
        string first = str_array.front();
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(first)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }else{
        string err= "Heap Empty";
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(err)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }
}

void MainWindow::on_prevExprButton_clicked() {
  // implement
    QString qStr = displayTextEdit->toPlainText();
    string input = qStr.toStdString();
    AlgebraicTreeExpression* cur_root = new AlgebraicTreeExpression(input.c_str());
//    std::vector<AlgebraicTreeExpression*>::iterator it = std::find(root_array.begin(), root_array.end(), cur_root);
//    std::vector<string>::iterator it = std::find(str_array.begin(), str_array.end(), input);
    int index;
    for(AlgebraicTreeExpression* root: root_array){
        if(root->operator==(*cur_root)){
            std::vector<AlgebraicTreeExpression*>::iterator it = std::find(root_array.begin(), root_array.end(), root);
            index = distance(root_array.begin(), it);
        }
    }
    if(!root_array.empty() && index != 0){
        string prev = str_array.at(index-1);
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(prev)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }else if(!root_array.empty() && index == 0){
        string prev = str_array.back();
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(prev)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }
    else{
        string err= "Heap Empty";
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(err)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }
}

void MainWindow::on_nextExprButton_clicked() {
  // implement
    QString qStr = displayTextEdit->toPlainText();
    string input = qStr.toStdString();
    AlgebraicTreeExpression* cur_root = new AlgebraicTreeExpression(input.c_str());
    int index;
    for(AlgebraicTreeExpression* root: root_array){
        if(root->operator==(*cur_root)){
            std::vector<AlgebraicTreeExpression*>::iterator it = std::find(root_array.begin(), root_array.end(), root);
        index = distance(root_array.begin(), it);
        }
    }
    if(_rootInHeap(cur_root) && index != root_array.size()-1){
        string next = str_array.at(index+1);
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(next)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }else if(_rootInHeap(cur_root) && index == root_array.size()-1){
        string next = str_array.at(0);
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(next)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }
    else{
        string err= "Error";
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(err)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }
}

void MainWindow::on_lastExprButton_clicked() {
  // implement
    if(!str_array.empty()){
        string last = str_array.back();
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(last)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }else{
        string err= "Heap Empty";
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(err)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }
}

void MainWindow::on_delExprButton_clicked() {
  // implement
    QString qStr = displayTextEdit->toPlainText();
    string input = qStr.toStdString();
    AlgebraicTreeExpression* cur_root = new AlgebraicTreeExpression(input.c_str());
    if(_rootInHeap(cur_root)){
        std::vector<string>::iterator it = std::find(str_array.begin(), str_array.end(), input);
        int index = distance(str_array.begin(), it);
        AlgebraicTreeExpression* cur_root = root_array.at(index);
        str_array.erase(it);
        root_array.erase(root_array.begin()+index);
        delete cur_root;
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">""</font>";
        displayTextEdit->insertHtml(qStr);
        if(root_array.empty()){
            firstExprButton->setDisabled(true);
            prevExprButton->setDisabled(true);
            nextExprButton->setDisabled(true);
            lastExprButton->setDisabled(true);
            delExprButton->setDisabled(true);
        }
    }else{
        string err= "No such expression in our storage";
        displayTextEdit->clear();
        QString qStr = "<font color=\"black\">" + QString::fromStdString(err)+ "</font>";
        displayTextEdit->insertHtml(qStr);
    }
}

bool MainWindow::_expressionInHeap(string expr){
    if(str_array.empty()) {
        return false;
    }
    for(string str:str_array){
        if(expr.compare(str)==0) return true;
    }
    return false;
}

bool MainWindow::_rootInHeap(AlgebraicTreeExpression* cur_root){
    if(root_array.empty()){
        return false;
    }
    for(AlgebraicTreeExpression* root:root_array){
        if(root->operator==(*cur_root)) return true;
    }
    return false;
}

