#ifndef QT_POLYGON_ALGORITHMS_H
#define QT_POLYGON_ALGORITHMS_H

#include <QWidget>

namespace Ui {
class PolygonAlgorithms;
}

class PolygonAlgorithms : public QWidget
{
    Q_OBJECT

public:
    explicit PolygonAlgorithms(QWidget *parent = 0);
    ~PolygonAlgorithms();

private:
    Ui::PolygonAlgorithms *ui;
};

#endif // QT_POLYGON_ALGORITHMS_H
