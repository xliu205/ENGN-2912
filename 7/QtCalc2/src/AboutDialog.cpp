#include "AboutDialog.hpp"

AboutDialog::AboutDialog(QWidget * parent, Qt::WindowFlags flags): 
  QDialog(parent, flags) {

  setupUi(this);
  
  qt_version_label->setText(qVersion());
  author_label->setText("(c) 2021 Liu Xingyu");

  built_date_label->setText(QString("%1 %2").arg(__DATE__, __TIME__));
}

AboutDialog::~AboutDialog() {
}
