#ifndef QT_TRIANGLE_SOUP_CREATION_METHOD_H
#define QT_TRIANGLE_SOUP_CREATION_METHOD_H

#include <QGroupBox>

namespace Ui {
class TriangleSoupCreationMethod;
}

class TriangleSoupCreationMethod : public QGroupBox
{
    Q_OBJECT

public:
    explicit TriangleSoupCreationMethod(QWidget *parent = 0);
    ~TriangleSoupCreationMethod();

signals:
    void CreateTriangleSoup();

private slots:
    void on_method_dropdown_currentTextChanged(const QString &arg1);

    void on_file_button_choose_clicked();

    void on_file_button_generate_clicked();

private:
    Ui::TriangleSoupCreationMethod *ui;
};

#endif // QT_TRIANGLE_SOUP_CREATION_METHOD_H
