{
  description = "Daedalus game project using the Atlas engine";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    atlas.url = "github:LarssonMartin1998/atlas";
  };

  outputs =
    {
      self,
      nixpkgs,
      atlas,
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      packages.${system}.default = pkgs.stdenv.mkDerivation {
        pname = "atlas";
        version = "0.1.0";
        src = ./.;

        nativeBuildInputs = with pkgs; [
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
    };
}
