{
  description = "SoapDump - A high-performance PayPal SOAP log parser";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      allSystems = [
        "x86_64-linux" # 64-bit Intel/AMD Linux
        "aarch64-linux" # 64-bit ARM Linux
        "x86_64-darwin" # 64-bit Intel macOS
        "aarch64-darwin" # 64-bit ARM macOS
      ];
      forAllSystems =
        f:
        nixpkgs.lib.genAttrs allSystems (
          system:
          f {
            pkgs = import nixpkgs { inherit system; };
          }
        );
    in
    {
      packages = forAllSystems (
        { pkgs }:
        let
          version = if self ? rev then self.rev else "0.1.0";
          soapdump = pkgs.stdenv.mkDerivation {
            pname = "soapdump";
            inherit version;
            src = self;

            nativeBuildInputs = with pkgs; [
              clang
              installShellFiles
            ];

            dontUseCmakeConfigure = true;

            buildPhase = ''
              # Direct compilation instead of using CMake
              mkdir -p build
              $CXX -std=c++17 -O3 -o build/soapdump $src/src/soapdump.cpp
            '';

            installPhase = ''
              mkdir -p $out/bin
              cp build/soapdump $out/bin/

              # Generate and install shell completions
              mkdir -p completions
              $out/bin/soapdump --generate-bash-completion > completions/soapdump.bash
              $out/bin/soapdump --generate-zsh-completion > completions/soapdump.zsh
              $out/bin/soapdump --generate-fish-completion > completions/soapdump.fish

              installShellCompletion --cmd soapdump \
                --bash completions/soapdump.bash \
                --fish completions/soapdump.fish \
                --zsh completions/soapdump.zsh

              # Generate and install man page
              mkdir -p $out/share/man/man1
              $out/bin/soapdump --man > $out/share/man/man1/soapdump.1
            '';

            meta = with pkgs.lib; {
              description = "A high-performance PayPal SOAP log parser";
              homepage = "https://github.com/taciturnaxolotl/soapdump";
              license = licenses.mit;
              maintainers = with maintainers; [ taciturnaxolotl ];
              platforms = platforms.linux ++ platforms.darwin;
            };
          };
        in
        {
          default = soapdump;
        }
      );

      apps = forAllSystems (
        { pkgs }:
        {
          default = {
            type = "app";
            program = "${self.packages.${pkgs.system}.default}/bin/soapdump";
          };
        }
      );

      devShells = forAllSystems (
        { pkgs }:
        {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              cmake
              clang
              self.packages.${pkgs.system}.default
            ];

            shellHook = ''
              echo "SoapDump development environment loaded"
            '';
          };
        }
      );

      formatter = forAllSystems ({ pkgs }: pkgs.nixfmt-tree);
    };
}
