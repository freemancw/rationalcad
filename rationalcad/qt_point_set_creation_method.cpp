#include "qt_point_set_creation_method.h"
#include "ui_point_set_creation_method.h"

#include "common.h"

PointSetCreationMethod::PointSetCreationMethod(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PointSetCreationMethod)
{
    ui->setupUi(this);
}

PointSetCreationMethod::~PointSetCreationMethod()
{
    delete ui;
}

void PointSetCreationMethod::on_file_button_clicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open point set data"),
        "./",
        tr("Text files (*.txt)")
    );

    ui->file_name->setText(fileName);
}

void PointSetCreationMethod::on_generate_clicked() {
    QFile file(ui->file_name->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //rInfo("Could not open file %s.", fileName.toUtf8().data());
        return;
    } else {
        //rInfo("Successfully opened file %s.", fileName.toUtf8().data());
    }

    /*
    QVector<QVector3D> points;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        auto tokens = line.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        //qDebug() << tokens << " size: " << tokens.size();
        points.push_back(QVector3D(tokens.at(1).toFloat(),
                                   tokens.at(2).toFloat(),
                                   tokens.at(3).toFloat()));
        //qDebug() << points.back();
    }
    */
}
