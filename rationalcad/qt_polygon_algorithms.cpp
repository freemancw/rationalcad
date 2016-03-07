#include "qt_polygon_algorithms.h"
#include "ui_polygon_algorithms.h"

PolygonAlgorithms::PolygonAlgorithms(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PolygonAlgorithms)
{
    ui->setupUi(this);
}

PolygonAlgorithms::~PolygonAlgorithms()
{
    delete ui;
}
