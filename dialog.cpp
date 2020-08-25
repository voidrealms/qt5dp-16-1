#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->lblOS->setText("OS Detected: " + m_ping.operatingSystem());
    m_ping.setAddress(ui->txtIP->text());
    connect(ui->btnStart,&QPushButton::clicked,&m_ping,&Ping::start);
    connect(ui->btnStop,&QPushButton::clicked,&m_ping,&Ping::stop);
    connect(&m_ping,&Ping::output,this,&Dialog::output);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::output(QString data)
{
    ui->plainTextEdit->appendPlainText(data);
}

void Dialog::on_txtIP_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    m_ping.setAddress(ui->txtIP->text());
}
