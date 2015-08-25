import qbs.base 1.0
import QtcPlugin

QtcPlugin {
    name: "Template"

    Depends {
        name: "Qt"
        submodules: [
            "core", "gui", "widgets"
        ]
    }
    Depends { name: "ExtensionSystem" }
    Depends { name: "Core" }


    cpp.includePaths: [
        "../../3rdparty/qtcreator/src/plugins",
    ]

    files: [
        "templateplugin.cpp", "templateplugin.h",
        "template_global.h",
        "templateconstants.h"
    ]
}
