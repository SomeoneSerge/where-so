{
  description = "Inspect ld.so's search paths via dlopen and dlinf";

  inputs.nixpkgs.url = github:NixOS/nixpkgs/nixpkgs-unstable;
  inputs.flake-parts.url = github:hercules-ci/flake-parts;

  outputs = { self, flake-parts, nixpkgs }@inputs:
    flake-parts.lib.mkFlake { inherit inputs; } {

      systems = [ "x86_64-linux" "aarch64-linux" ];

      flake.overlays.default = final: prev: { where-so = final.callPackage ./. { }; };
      perSystem = { config, pkgs, ... }: {
        packages.default = config.packages.where-so;
        packages.where-so = pkgs.callPackage ./. { };
      };
    };
}
