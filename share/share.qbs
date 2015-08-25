import qbs 1.0

Product {
    name: "Custom SharedContent"

    Group {
        name: "Unconditional"
        qbs.install: true
        qbs.installDir: project.ide_data_path
        prefix: "qtcreator/"
        files: [
            "generic-highlighter",
        ]
    }
}
