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
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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
        tr("FBX files (*.fbx)")
    );

    ui->file_name->setText(fileName);
}

void TriangleSoupCreationMethod::on_file_button_generate_clicked() {
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(ui->file_name->text().toStdString(),
            aiProcess_CalcTangentSpace       |
            aiProcess_Triangulate            |
            aiProcess_JoinIdenticalVertices  |
            aiProcess_SortByPType);

    if (!scene) {
        LOG(ERROR) << "Unable to open file.";
        return;
    }

    LOG(INFO) << "Successfully opened OBJ file. Continuing.";

    QVector<QVector3D> vertices;
    QVector<qint32> indices;

    vertices.push_back(QVector3D(0, 0, 0));
    vertices.push_back(QVector3D(8, 0, 0));
    vertices.push_back(QVector3D(0, 8, 0));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    emit CreateTriangleSoup(vertices, indices);
}

