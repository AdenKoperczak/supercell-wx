from conan import ConanFile
import os
import sys

class StopBuildConan(ConanFile):
    name = "stopbuild"
    version = "1.0"

    def build(self):
        if os.getenv("CONAN_NO_BUILD", "0") == "1":
            self.output.info("Build aborted: CONAN_NO_BUILD set")
            sys.exit(1)
        else:
            self.output.info("Build continueing: CONAN_NO_BUILD not set")


    def package(self):
        pass
