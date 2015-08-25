import qbs.base 1.0
import QtcPlugin

QtcPlugin {
    name: "GoEditor"

    Depends {
        name: "Qt"
        submodules: [
            "core", "gui", "widgets"
        ]
    }
    Depends { name: "ExtensionSystem" }
    Depends { name: "Core" }
    Depends { name: "TextEditor" }


    cpp.includePaths: [
        "../../3rdparty/qtcreator/src/plugins",
    ]

    files: [
        "goautocompleter.cpp", "goautocompleter.h",
        "gocodeformatter.cpp", "gocodeformatter.h",
        "gocompletionassistprovider.cpp", "gocompletionassistprovider.h",
        "godocument.cpp", "godocument.h",
        "goeditor.cpp", "goeditor.h",
        "goeditor.qrc",
        //"goeditorfactory.cpp", "goeditorfactory.h",
        "goeditorplugin.cpp", "goeditorplugin.h",
        //"goeditorwidget.cpp", "goeditorwidget.h",
        "goeditor_global.h",
        "goeditorconstants.h",
        "gohighlighter.cpp", "gohighlighter.h",
        "goindenter.cpp", "goindenter.h",
        "golexer.cpp", "golexer.h",
        "gotoken.cpp", "gotoken.h",
    ]
}
