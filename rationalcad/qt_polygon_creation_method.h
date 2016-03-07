#ifndef QT_POLYGON_CREATION_METHOD_H
#define QT_POLYGON_CREATION_METHOD_H

#include <QWidget>

namespace Ui {
class PolygonCreationMethod;
}

class PolygonCreationMethod : public QWidget
{
    Q_OBJECT

public:
    explicit PolygonCreationMethod(QWidget *parent = 0);
    ~PolygonCreationMethod();

private:
    Ui::PolygonCreationMethod *ui;
};

#endif // QT_POLYGON_CREATION_METHOD_H
