import qbs 1.0

Product {
    name: "Examples"

    Group {
        qbs.install: true
        qbs.installDir: "examples/go"
        files: [
            "helloworld",
            "mypackage",
        ]
    }
}
