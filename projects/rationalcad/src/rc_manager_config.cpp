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

/*!
 * @author Clinton Freeman <admin@freemancw.com>
 * @date 04/22/2013
 */

// Qt
#include <QSysInfo>

// RationalCAD
#include "rc_common.h"
#include "rc_manager_config.h"

BEGIN_NAMESPACE(RCAD)

RCConfig g_config;

void InitializeGlobalConfig() {

    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setRedBufferSize(8);
    glf.setGreenBufferSize(8);
    glf.setBlueBufferSize(8);
    glf.setAlphaBufferSize(8);
    glf.setSampleBuffers(true);
    glf.setSamples(8);
    glf.setDepth(true);
    glf.setDepthBufferSize(24);
    glf.setVersion(3, 3);
    glf.setProfile(QGLFormat::CompatibilityProfile);
    glf.setSwapInterval(1);
    QGLFormat::setDefaultFormat(glf);

    g_config.tag_colors.insert("normal", QColor(0, 0, 0));
    g_config.tag_colors.insert("active", QColor(230, 180, 0));
    g_config.tag_colors.insert("inactive", QColor(200, 200, 200));
    g_config.tag_colors.insert("error", QColor(200, 0, 0));
    g_config.tag_colors.insert("success", QColor(0, 200, 0));

    g_config.tag_colors.insert("vert_default", QColor(100, 100, 100));
    g_config.tag_colors.insert("vert_selected", QColor(50, 75, 150));
    g_config.tag_colors.insert("edge_default", QColor(0, 0, 0));
    g_config.tag_colors.insert("edge_selected", QColor(50, 160, 255));
    g_config.tag_colors.insert("face_default", QColor(200, 200, 200));
    g_config.tag_colors.insert("face_selected", QColor(255, 100, 100));

    g_config.grid_major_color_ = QColor(100, 100, 100);
    g_config.grid_minor_color_ = QColor(200, 200, 200);

    g_config.input_state_ = SELECT;

    g_config.snap_to_grid_ = false;
}

END__NAMESPACE(RCAD)

