{
  description = "PayPal SOAP Log Parser";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        
        # Build dependencies
        buildInputs = with pkgs; [
          cmake
        ];
        
        # Development dependencies
        nativeBuildInputs = with pkgs; [
          clang-tools
          bear
        ];
      in
      {
        packages = rec {
          soapdump = pkgs.stdenv.mkDerivation {
            pname = "soapdump";
            version = "0.1.0";
            
            src = ./.;
            
            nativeBuildInputs = [ pkgs.clang ];
            
            buildPhase = ''
              mkdir -p bin
              clang++ -std=c++17 -O3 -o bin/soapdump src/soapdump.cpp
            '';
            
            installPhase = ''
              mkdir -p $out/bin
              cp bin/soapdump $out/bin/
            '';
          };
          
          default = soapdump;
        };
        
        devShells.default = pkgs.mkShell {
          inherit buildInputs nativeBuildInputs;
          
          shellHook = ''
            echo "SoapDump development environment loaded"
          '';
        };
        
        apps = rec {
          soapdump = {
            type = "app";
            program = "${self.packages.${system}.default}/bin/soapdump";
            drv = self.packages.${system}.default;
          };
          
          default = soapdump;
        };
      }
    );
}