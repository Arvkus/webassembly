
# sukompiliuoti Linux versija
g++ cpp/src/* \
    -o \"cpp/bin/main.out\" \
    -I cpp/include \
    -l GLESv2 -l SDL2 -l assimp \
    -std=c++17 -Wall 

# sukompiliuoti WASM versija
em++ -std=c++1z cpp/src/* cpp/lib/* -I cpp/include -o module.js \
    -s WASM=1 -s USE_SDL=2 -s FULL_ES3=1 -s USE_WEBGL2=1 \
    --preload-file filesystem -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1

# startinti HTTP serveri
python -m SimpleHTTPServer 8080

# Linux bibliotekos
sudo apt-get install libassimp-dev
sudo apt-get install libsdl2-dev

# emscripten vieta: ~/repos/emsdk
# aplinkos kintamuku aktyvavimas
source ./emsdk_env.sh --build=Release
cat emsdk_set_env.sh >> ~/.bashrc

# Assimp bibliotekos kompiliavimo komanda
cmake CMakeLists.txt \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_TOOLCHAIN_FILE=~/repos/emsdk/emscripten/1.38.31/cmake/Modules/Platform/Emscripten.cmake \
    -DEMSCRIPTEN=1 

# prideti MIME tipa: "application/wasm    wasm"
/etc/mime.types # failo vieta

# GPU drivers:
https://github.com/lutris/lutris/wiki/Installing-drivers

# kiti paketai
sudo apt-get install git
sudo apt-get install g++

# github repos
git clone https://github.com/emscripten-core/emsdk.git # emscripten
git clone https://github.com/assimp/assimp.git # assimp biblioteka
git clone https://github.com/Arvkus/webassembly.git # mano projektas

Github workflow test

