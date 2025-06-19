{
  description = "Daedalus game project using the Atlas engine";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    # atlas.url = "github:LarssonMartin1998/atlas";
    atlas.url = "path:/home/larssonmartin/dev/git/daedalus/atlas";
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
        pkgsCross = pkgs.pkgsCross.mingwW64;

        stdenv = pkgs.llvmPackages_20.stdenv;
        xstdenv = pkgsCross.stdenv;

        pname = "daedalus";
        version = "0.1.0";

        engine_src = atlas.packages.${system}.atlas.src;
      in
      {
        packages.daedalus = stdenv.mkDerivation {
          pname = pname;
          version = version;
          src = ./.;

          nativeBuildInputs =
            with pkgs;
            [
              llvmPackages_20.clang-tools
              ninja
              cmake
              pkg-config
            ]
            ++ lib.optional stdenv.isDarwin [ moltenvk ];

          buildInputs =
            with pkgs;
            [
              vulkan-headers
              vulkan-loader
              glfw
              glm
              stb
              taskflow
            ]
            ++ lib.optional stdenv.isDarwin [ moltenvk ];

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
            "-DBUILD_TESTS=ON"
            "-DENABLE_INSTALL=ON"
          ];

          # Needed on Linux: libvulkan is loaded with dlopen()
          # so it is *not* seen by patchelf → keep the RPATH.
          # dontPatchELF = stdenv.isLinux; # :contentReference[oaicite:0]{index=0}

          # Were depending on running `add_subdirectory(atlas)` in the CMakeLists.txt
          # This has a lot of benefits when it comes to the modularity of the project, but it
          # also hinders us from just adding the library as an flake- and buildInput.
          preConfigure = ''
            cp -r ${engine_src} atlas
            chmod -R u+w atlas
          '';

          doCheck = true;
          checkPhase = ''
            ctest --output-on-failure
          '';

          shellHook = ''
            export CXXFLAGS="$NIX_CFLAGS_COMPILE"
          '';
        };

        packages.daedalus-windows = xstdenv.mkDerivation {
          pname = pname;
          version = version;
          src = ./.;

          buildInputs = with pkgsCross; [
            vulkan-headers
            vulkan-loader
            glfw
            glm
            stb
            taskflow
          ];

          nativeBuildInputs = with pkgs; [
            llvmPackages_20.clang-tools
            ninja
            cmake
            pkg-config
          ];

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
            "-DBUILD_TESTS=OFF"
            "-DENABLE_INSTALL=ON"
          ];

          # Were depending on running `add_subdirectory(atlas)` in the CMakeLists.txt
          # This has a lot of benefits when it comes to the modularity of the project, but it
          # also hinders us from just adding the library as an flake- and buildInput.
          preConfigure = ''
            cp -r ${engine_src} atlas
            chmod -R u+w atlas
          '';
        };

        packages.default = self.packages.${system}.daedalus;
      }
    );
}
