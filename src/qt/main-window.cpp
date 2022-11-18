#include "main-window.h"

MainWindow::MainWindow() {
  this->setFixedSize(QSize(WIDTH, HEIGHT));

  QPushButton* button = new QPushButton("New Window", this);
  button->setGeometry(QRect(QPoint((WIDTH / 2) - 45, (HEIGHT / 2) - 20), QSize(90, 40)));

  MainWindow::connect(button, &QPushButton::clicked, this, &MainWindow::login);
}

void MainWindow::login() {
  QDialog dialog(this);
  dialog.setFixedSize(QSize(280, 170));

  QFormLayout form(&dialog);

  form.addRow(new QLabel(QObject::tr("<div align='center'>Login</div>")));

  GM::Vector<QLineEdit*> fields;

  QLineEdit* lineEdit = new QLineEdit(&dialog);
  form.addRow(QString("Username: "), lineEdit);
  fields.push_back(lineEdit);

  lineEdit = new QLineEdit(&dialog);
  form.addRow(QString("Password: "), lineEdit);
  fields.push_back(lineEdit);

  QPushButton* button = new QPushButton("Login", &dialog);
  form.addRow(button);
  QObject::connect(button, &QPushButton::clicked, &dialog, [&dialog, &fields]() {
    std::size_t hash =
        std::hash<std::string>{}((fields[0]->text() + fields[1]->text()).toStdString());
    if (Utility::Login::isValid(hash)) {
      dialog.accept();
    }

    bool shakeSwitch = true;
    QPoint offset(10, 0);
    for (int i = 0; i < 9; i++) {
      QTimer::singleShot(100, &dialog, [&]() {
        dialog.move(((shakeSwitch) ? dialog.pos() + offset : dialog.pos() - offset));
        shakeSwitch = !shakeSwitch;
      });
    }
  });

  if (dialog.exec() == QDialog::Rejected) {
    this->close();
  }
}

void MainWindow::closeEvent(QCloseEvent* event) {}

void MainWindow::commitData(QSessionManager &) {}