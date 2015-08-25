import qbs.base 1.0
import QtcPlugin

QtcPlugin {
    name: "GoCompiler"

    Depends {
        name: "Qt"
        submodules: [
            "core", "gui", "widgets"
        ]
    }
    Depends { name: "ExtensionSystem" }
    Depends { name: "Core" }
    Depends { name: "ProjectExplorer" }
    Depends { name: "qbscore" }
    Depends { name: "QbsProjectManager" }


    cpp.includePaths: [
        "../../3rdparty/qtcreator/src/plugins",
    ]

    files: [
        "gocompiler.qrc",
        "gocompilerplugin.cpp", "gocompilerplugin.h",
        "gocompiler_global.h",
        "gocompilerconstants.h",
        "gooutputparser.cpp", "gooutputparser.h",
        "gopropertyprovider.cpp", "gopropertyprovider.h",
        "gotoolchain.cpp", "gotoolchain.h",
        "gotoolchainconfigwidget.cpp", "gotoolchainconfigwidget.h",
        "gotoolchainfactory.cpp", "gotoolchainfactory.h",
        "nodemodulessettingspage.cpp", "nodemodulessettingspage.h", "nodemodulessettingspage.ui",
    ]
}
