import qbs

Project {
    name: "Plugins"

    references: [
        "gocompiler/gocompiler.qbs",
        "goeditor/goeditor.qbs",
        "template/template.qbs",
    ]
}
