from conan import ConanFile
import os
import sys

class StopBuildConan(ConanFile):
    name = "stopbuild"
    version = "1.0"

    def build(self):
        self.output.info("Build aborted by stopbuild")
        sys.exit(1)

    def package(self):
        pass
