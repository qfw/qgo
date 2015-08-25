import qbs

Project {  
    name: "QGo Sources"

    references: [
        "examples/examples.qbs",
        "plugins/plugins.qbs",
    ]

    Project {
            name: "qbs"
            id: qbsProject
            property string resourcesInstallDir: project.ide_data_path + "/qbs"

            references: [
                path + "/shared/qbs/share/share.qbs",
            ]
        }
}
