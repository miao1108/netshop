#ifndef WIRESHARK_H
#define WIRESHARK_H

#include <QMainWindow>

namespace Ui {
class Wireshark;
}

class Wireshark : public QMainWindow
{
    Q_OBJECT

public:
    explicit Wireshark(QWidget *parent = nullptr);
    ~Wireshark();

private:
    Ui::Wireshark *ui;
};

#endif // WIRESHARK_H
