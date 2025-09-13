{
  description = "SoapDump - A high-performance PayPal SOAP log parser";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, ... }:
    let
      allSystems = [
        "x86_64-linux" # 64-bit Intel/AMD Linux
        "aarch64-linux" # 64-bit ARM Linux
        "x86_64-darwin" # 64-bit Intel macOS
        "aarch64-darwin" # 64-bit ARM macOS
      ];
      forAllSystems = f:
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
              cmake
              clang
              installShellFiles 
            ];
            
            buildPhase = ''
              cmake -B build -S .
              cmake --build build --config Release
            '';
            
            installPhase = ''
              mkdir -p $out/bin
              cp build/soapdump $out/bin/
              
              # Generate shell completions
              mkdir -p completions
              $out/bin/soapdump --help > /dev/null 2>&1 || true
              
              # Install shell completions if they exist
              if [ -f completions/soapdump.bash ]; then
                installShellCompletion --bash completions/soapdump.bash
              fi
              if [ -f completions/soapdump.fish ]; then
                installShellCompletion --fish completions/soapdump.fish
              fi
              if [ -f completions/soapdump.zsh ]; then
                installShellCompletion --zsh completions/soapdump.zsh
              fi
              
              # Generate man page
              mkdir -p $out/share/man/man1
              $out/bin/soapdump --help | sed 's/^/    /' > $out/share/man/man1/soapdump.1
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