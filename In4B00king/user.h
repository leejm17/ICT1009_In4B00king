#ifndef USER_H
#define USER_H
#include <QMainWindow>
#include <QMessageBox>

class user
{
public:
    user();
    void GetUserVariables(QString username);
    QString getName() const;

    int getAge() const;

    QString getGender() const;

    QString getAbout() const;

    QString getType() const;

    QString getFname() const;

    QString getLname() const;

    QString getEmail() const;

private:
    QString email;
    QString name;
    int age;
    QString gender;
    QString about;
    QString type;
    QString fname;
    QString lname;

};

class customer : public user{
public:
    customer();
    customer(QString);
};

class administrator : public user{
private:
    int privileges;

public:
    administrator();
    administrator(QString);
    int getpriv();
};



#endif // USER_H
