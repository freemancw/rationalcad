/*
 * This file is part of RationalCAD.
 *
 * RationalCAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * RationalCAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with RationalCAD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "qt_triangle_soup_creation_method.h"
#include "ui_triangle_soup_creation_method.h"

#include "common.h"

TriangleSoupCreationMethod::TriangleSoupCreationMethod(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::TriangleSoupCreationMethod) {
    ui->setupUi(this);
}

TriangleSoupCreationMethod::~TriangleSoupCreationMethod() {
    delete ui;
}

void TriangleSoupCreationMethod::on_method_dropdown_currentTextChanged(const QString &arg1) {

}

void TriangleSoupCreationMethod::on_file_button_choose_clicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open triangle soup data"),
        "./",
        tr("OBJ files (*.obj)")
    );

    ui->file_name->setText(fileName);
}

void TriangleSoupCreationMethod::on_file_button_generate_clicked() {
    LOG(INFO) << "on_generate_clicked with file "
              << ui->file_name->text().toStdString();

    QFile file(ui->file_name->text());

    // error check
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG(ERROR) << "Unable to open OBJ file.";
        return;
    }

    LOG(INFO) << "Successfully opened OBJ file. Continuing.";

    QVector<QVector3D> points;
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
/*
        auto tokens = line.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        points.push_back(QVector3D(tokens.at(0).toFloat(),
                                   tokens.at(1).toFloat(),
                                   tokens.at(2).toFloat()));
                                   */
    }

    emit CreateTriangleSoup();
}

