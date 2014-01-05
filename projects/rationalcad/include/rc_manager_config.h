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
 * @date 08/16/2013
 * @brief Manager type responsible for global configuration settings.
 */

#ifndef RC_MANAGER_CONFIG_H
#define RC_MANAGER_CONFIG_H

// RationalCAD
#include "rc_common.h"

BEGIN_NAMESPACE(RCAD)

enum InputState {
    SELECT,
    CREATE_POLYGON,
    CREATE_POLYTOPE,
    CREATE_2CONE_VERTEX,
    CREATE_2CONE_RAY_A,
    CREATE_2CONE_RAY_B,
    CREATE_MEDIANTSEQUENCE_ORIGIN,
    CREATE_MEDIANTSEQUENCE_TARGET
};

class ConfigManager {
private:
    ConfigManager();
    ConfigManager(const ConfigManager&);
    ConfigManager& operator=(const ConfigManager&);

public:
    static ConfigManager& get() {
        static ConfigManager instance;
        return instance;
    }

    void Initialize();

    const QColor& GetDebugColor(const quint32 key) const;
    void SetDebugColor(const quint32 key, const QColor& color);

    const InputState& input_state() const;
    void set_input_state(const InputState& input_state);

private:
    QMap<quint32, QColor> dbg_colors_;
    InputState input_state_;
};

struct RCConfig {
    QMap<QString, QColor> tag_colors;
    QColor grid_minor_color_;
    QColor grid_major_color_;
    QColor color_vert_default;
    QColor color_vert_rounded;

    InputState input_state_;
    bool snap_to_grid_;
};

extern RCConfig g_config;

void InitializeGlobalConfig();

END__NAMESPACE(RCAD)

#endif // RC_MANAGER_CONFIG_H
