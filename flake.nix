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
              clang++ -std=c++17 -O3 -o bin/transaction-parser src/transaction-parser.cpp
            '';
            
            installPhase = ''
              mkdir -p $out/bin
              cp bin/transaction-parser $out/bin/
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
          transaction-parser = {
            type = "app";
            program = "${self.packages.${system}.default}/bin/transaction-parser";
            drv = self.packages.${system}.default;
          };
          
          default = transaction-parser;
        };
      }
    );
}