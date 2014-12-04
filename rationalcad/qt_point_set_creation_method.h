#ifndef POINT_SET_CREATION_METHOD_H
#define POINT_SET_CREATION_METHOD_H

#include <QGroupBox>

namespace Ui {
class PointSetCreationMethod;
}

class PointSetCreationMethod : public QGroupBox
{
    Q_OBJECT

public:
    explicit PointSetCreationMethod(QWidget *parent = 0);
    ~PointSetCreationMethod();

private slots:
    void on_file_button_clicked();

    void on_generate_clicked();

private:
    Ui::PointSetCreationMethod *ui;
};

#endif // POINT_SET_CREATION_METHOD_H
