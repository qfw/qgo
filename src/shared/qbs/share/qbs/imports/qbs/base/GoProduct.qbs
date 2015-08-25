import qbs 1.0

Product {
    property path buildDir

    Depends { name: "go" }
    Depends { name: "jade" }
    Depends { name: "less" }
}
