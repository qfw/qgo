import qbs
import qbs.File
import qbs.FileInfo

Module {
    property string compilerPath
    property string outputSuffix

    FileTagger {
        patterns: ["*.jade"]
        fileTags: ["jade"]
    }

    additionalProductTypes: ["jade_html", "jade_output"]

    Rule {
        id: jadeCompiler
        inputs: ["jade"]

        Artifact {
            filePath: {
                var path = product.destinationDirectory + "/";
                if (input.baseDir !== "")
                    path += input.baseDir + "/";
                path += input.completeBaseName + ".html";
                return path;
            }
            fileTags: ["jade_html"]
            qbs.install: {
                var jadeOutputSuffix = product.moduleProperty("jade", "outputSuffix");
                return (jadeOutputSuffix === "") || (jadeOutputSuffix === "html");
            }
            qbs.installDir: input.baseDir
        }

        prepare: {
            var args = [input.filePath, "--out", FileInfo.path(output.filePath)];

            var cmd = new Command(product.moduleProperty("jade", "compilerPath"), args);
            cmd.description = 'compiling ' + input.fileName;
            cmd.highlight = 'compiler'

            return cmd;
        }
    }

    Rule {
        id: jadeRenamer
        inputs: ["jade_html"]
        condition: {
            var jadeOutputSuffix = product.moduleProperty("jade", "outputSuffix");
                return (jadeOutputSuffix !== "") && (jadeOutputSuffix !== "html");
        }

        Artifact {
            filePath: {
                var path = product.destinationDirectory + "/";
                if (input.baseDir !== "")
                    path += input.baseDir + "/";
                path += input.completeBaseName + "." + product.moduleProperty("jade", "outputSuffix");
                return path;
            }
            fileTags: ["jade_output"]
            qbs.install: true
            qbs.installDir: input.baseDir
        }

        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "renaming " + input.fileName;
            cmd.sourceCode = function() {
                File.copy(input.filePath, output.filePath);
                File.remove(input.filePath);
            };
            return cmd;
        }
    }
}
