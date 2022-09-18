#ifndef WLOGIN_H
#define WLOGIN_H

#include <QMainWindow>
#include <string>
#include <QMessageBox>
#include <QImage>
#include <QCryptographicHash>

using namespace std;

namespace Ui {
class WLogin;
}

class WLogin : public QMainWindow
{
    Q_OBJECT

public:
    explicit WLogin(QMainWindow *p, QWidget *parent = nullptr);
    ~WLogin();
    Ui::WLogin * uii;

private slots:
    void on_pushButton_clicked();

signals:
    void Send_uid_to_mwd(QString,QString);

private:
    QMainWindow* p_mwd;
    QString id,pwd;
    Ui::WLogin *ui;
    void Send_log_id();

};

#endif // WLOGIN_H
