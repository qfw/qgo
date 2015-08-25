import qbs
import qbs.File
import qbs.FileInfo

Module {
    property string compilerPath

    FileTagger {
        patterns: ["*.go"]
        fileTags: ["go"]
    }

    Rule {
        id: applicationCompiler
        multiplex: true
        inputs: ["go"]

        Artifact {
            filePath: product.destinationDirectory + "/" + product.targetName
            fileTags: ["application"]
            qbs.install: true
        }

        prepare: {
            var args = ["build", "-o", output.filePath];
            var cmd = new Command(product.moduleProperty("go", "compilerPath"), args);
            cmd.description = 'compiling ' + output.fileName;
            cmd.highlight = 'compiler'
            cmd.workingDirectory = product.buildDir || project.path;
            cmd.responseFileUsagePrefix = '@';

            return cmd;
        }
    }
}
