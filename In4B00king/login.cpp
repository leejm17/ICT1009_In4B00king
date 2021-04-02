#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QPixmap>
#include <QCryptographicHash>
Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    QPixmap pix(":/resources/img/Logo.png");
    ui->label_pic->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{

    QString username = ui->username->text();
    QString password = ui->password->text();
    QString salted_password = username + password;
    // SHA1 Hash
    QByteArray hashed_password = QCryptographicHash::hash(salted_password.toUtf8(),QCryptographicHash::Md5);
    QString inputHash = QLatin1String(hashed_password.toHex());

    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("Select * FROM User WHERE email_ID='" + username + "' AND password='" + inputHash + "';");
    if(!query.exec()){
        qDebug() << query.lastError().text() << query.lastQuery();
    }else{
        if(query.next()){
            qDebug() << "read was successful "<< query.lastQuery();
            QMessageBox::information(this, "Login", "You have logined successfully");
            close();
            mainpage = new MovieList(this);
            mainpage->show();
        }else{
            qDebug() << "read was successful "<< query.lastQuery();
            QMessageBox::warning(this, "Login", "Wrong email or password");
        }
    }
}

void Login::on_Register_clicked()
{
    this->close();
    Register registerpage;
    registerpage.setModal(true);
    registerpage.exec();
}