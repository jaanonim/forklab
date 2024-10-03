{ lib, stdenv, cmake, openssl }:
stdenv.mkDerivation
{
  name = "forklab";
  src = ./.;

  buildInputs = [
    cmake
    openssl
  ];

  configurePhase = ''
    mkdir -p build
    cd build
    cmake ..
  '';

  buildPhase = ''
    cmake --build . --config Release
  '';

  installPhase = ''
    mkdir -p $out/bin
    mv forklab $out/bin
  '';
}
