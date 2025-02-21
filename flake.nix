{
  description = "Daedalus game project using the Atlas engine";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    atlas.url = "github:LarssonMartin1998/atlas";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      atlas,
    }:
    flake-utils.lib.eachSystem [ "aarch64-darwin" "aarch64-linux" "x86_64-darwin" "x86_64-linux" ] (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        stdenv = pkgs.llvmPackages.stdenv;
      in
      {
        packages.default = stdenv.mkDerivation {
          pname = "atlas";
          version = "0.1.0";
          src = ./.;

          nativeBuildInputs = with pkgs; [
            clang-tools
            clang
            ninja
            cmake
          ];

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
            "-DBUILD_TESTS=ON"
            "-DENABLE_INSTALL=ON"
          ];

          # Were depending on running `add_subdirectory(atlas)` in the CMakeLists.txt
          # This has a lot of benefits when it comes to the modularity of the project, but it
          # also hinders us from just adding the library as an flake- and buildInput.
          preConfigure = ''
            cp -r ${atlas.src} $(pwd)/atlas
            chmod -R u+w atlas
          '';

          doCheck = true;
          checkPhase = ''
            ctest --output-on-failure
          '';
        };
      }
    );
}
