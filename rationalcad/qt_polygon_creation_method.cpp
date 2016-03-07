#include "qt_polygon_creation_method.h"
#include "ui_polygon_creation_method.h"

PolygonCreationMethod::PolygonCreationMethod(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PolygonCreationMethod)
{
    ui->setupUi(this);
}

PolygonCreationMethod::~PolygonCreationMethod()
{
    delete ui;
}
