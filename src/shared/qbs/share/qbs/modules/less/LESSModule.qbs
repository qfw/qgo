import qbs
import qbs.File
import qbs.FileInfo
import qbs.ModUtils

Module {
    property string compilerPath
    property bool compressFlag

    FileTagger {
        patterns: ["*.less"]
        fileTags: ["less"]
    }

    additionalProductTypes: ["less_css"]

    Rule {
        id: lessCompiler
        inputs: ["less"]

        Artifact {
            filePath: {
                var path = product.destinationDirectory + "/";
                if (input.baseDir !== "")
                    path += input.baseDir + "/";
                path += input.completeBaseName + ".css";
                return path;
            }
            fileTags: ["less_css"]
            qbs.install: true
            qbs.installDir: input.baseDir
        }

        prepare: {
            var args = [];
            if (product.moduleProperty("less", "compressFlag"))
                args.push("-x");

            args.push(input.filePath);
            args.push(output.filePath);

            var cmd = new Command(product.moduleProperty("less", "compilerPath"), args);
            cmd.description = 'compiling ' + input.fileName;
            cmd.highlight = 'compiler'

            return cmd;
        }
    }
}
