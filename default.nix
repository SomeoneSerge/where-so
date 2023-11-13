{ pkgs ? import <nixpkgs> { } }:

pkgs.callPackage
  ({ cmake, stdenv, lib }:

  stdenv.mkDerivation {
    pname = "where-so";
    version = "0.0.0";
    src = lib.cleanSource ./.;

    nativeBuildInputs = [ cmake ];

    doCheck = stdenv.buildPlatform == stdenv.hostPlatform;
    checkPhase = ''
      runHook preCheck

      echo Sanity checking where-so
      if LD_LIBRARY_PATH=$PWD ./where-so libfindme.so | grep -q "$PWD" ; then
        echo Passed 1 test
      else
        echo Failed 1 test
        exit 1
      fi

      runHook postCheck
    '';

    meta.mainProgram = "where-so";
  })
{ }
