#ifndef WADMIN_H
#define WADMIN_H

#include <QMainWindow>
#include <string>
using namespace std;

namespace Ui {
class WAdmin;
}

class WAdmin : public QMainWindow
{
    Q_OBJECT

public:
    explicit WAdmin(QWidget *parent = nullptr);
    ~WAdmin();

private:
    Ui::WAdmin *ui;
};

#endif // WADMIN_H
