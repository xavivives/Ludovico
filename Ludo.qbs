import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '../../..'

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            "src/appController.cpp",
            "src/appController.h",
            "src/audioController.cpp",
            "src/audioController.h",
            "src/constants.h",
            "src/eventX.cpp",
            "src/eventX.h",
            "src/main.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
            "src/parameterTransformer.cpp",
            "src/parameterTransformer.h",
            "src/socketController.cpp",
            "src/socketController.h",
            "src/uiConnector.cpp",
            "src/uiConnector.h",
            "src/utils.h",
            "src/utils.mm",
            "src/video/videoController.cpp",
            "src/video/videoController.h",
            "src/viz/fxController.cpp",
            "src/viz/fxController.h",
            "src/viz/fxFactory.cpp",
            "src/viz/fxFactory.h",
        ]

        of.addons: [
            'ofxJSON',
            'ofxMacSystemProfiler',
            'ofxLibWebSockets',
            'ofxFft',
            'ofxExprtk',
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include

        // include search paths
        of.includePaths: [
            'of/addons/ofxFft/src',
            'of/addons/ofxFft/libs/kiss'
        ]

        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
        // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
