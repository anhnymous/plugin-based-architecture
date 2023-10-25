import os
import re
import shutil
from conans import ConanFile, CMake, tools

class FCore(ConanFile):
  name = "feel_core"
  version = "v0.1.0"
  license = "MIT License"
  author = "Anh Tran <anhthd2017@gmail.com>"
  url = ""
  description = "Feel Core (gRPC Server)"
  topics = ("open source", "conan package", "Plugin Loader/Handler")
  settings = "cppstd", "os", "compiler", "build_type", "arch"
  options = {"shared": [True, False], "fPIC": [True, False]}
  default_options = {"shared": False, "fPIC": True}
  generators = "cmake"
  exports_sources = [ "src/*", "CMakeLists.txt" ]
  requires = [ "nlohmann-json/v3.11.2", "grpc/v1.50.0" ]
  no_copy_source = True

  def requirements(self):
    self.settings.compiler.libcxx = "libstdc++11"
    if self.settings.os == "Windows":
      del self.options.fPIC

  def imports(self):
    self.copy("*.a",   keep_path=True)
    self.copy("*.h",   keep_path=True)
    self.copy("*.hh",  keep_path=True)
    self.copy("*.hpp", keep_path=True)
    self.copy("*.so*", keep_path=True)

  def build(self):
    cmake = CMake(self)
    cmake.definitions["CODE_COVERAGE"] = "OFF"

    grpc_pkg_folder = self.deps_cpp_info["grpc"].rootpath
    cmake.definitions["CMAKE_PREFIX_PATH"] = grpc_pkg_folder
    ld_library_path = os.path.join(grpc_pkg_folder, "lib")
    os.environ['LD_LIBRARY_PATH'] = ld_library_path

    if (tools.get_env("DEBUG") == "ON"):
      cmake.definitions["DEBUG"] = "ON"

    if (tools.get_env("target") == "LINUX_PC"):
      cmake.definitions["TARGET"] = "LINUX_PC"
      install_destination = "../bin/linux_pc"
    if (tools.get_env("target") == "ARM64"):
      cmake.definitions["TARGET"] = "ARM64"
      install_destination = "../bin/arm64"
    else:
      cmake.definitions["TARGET"] = "LINUX_PC"
      install_destination = "../bin/linux_pc"

    cmake.configure(source_folder=".")
    cmake.build()
    cmake.install()

    data_dir = install_destination + "/data"
    lib_dir  = install_destination + "/base/lib"
    conf_dir = install_destination + "/conf"
    os.system("mkdir -p " + data_dir)
    os.system("mkdir -p " + lib_dir)
    os.system("mkdir -p " + conf_dir)

    for shared_lib in os.listdir("lib"):
      if re.match(".*.so.*", shared_lib):
        cmd = "cp lib/" + shared_lib + " " + lib_dir
        os.system(cmd)

  def package(self):
    self.copy("feel_core", src="bin", dst="bin")

  def packae_info(self):
    pass  # we don't publish this package as conan lib
