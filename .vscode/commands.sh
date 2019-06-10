g++ cpp/src/* \
    -o \"cpp/bin/main.out\" \
    -I cpp/include \
    -l GLESv2 -l SDL2 -l assimp \
    -std=c++17 -Wall 

em++ -std=c++1z cpp/src/* cpp/lib/* -I cpp/include -o module.js \
    -s WASM=1 -s USE_SDL=2 -s FULL_ES3=1 -s USE_WEBGL2=1 \
    --preload-file filesystem -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1


# model class rework
# memory fix
# light fix
# WASM filesystem 
# index html loading / icons
    
# assimp lib size reduction

cd ${workspaceFolder}
python -m SimpleHTTPServer 8080


# -s NO_EXIT_RUNTIME=1  do not exit after `main()` function is executed
# -std=c++1z use C++17 standart
# -s SIDE_MODULE=1 create as dynamic lib, to use it `loadWebAssemblyModule`
# --preload-file files package files to .data file (accessed same as fstream in web)
# --embed-file files are inside .js generated script
# -s ALLOW_MEMORY_GROWTH=1  -static amount of memory is reserved, increase that number
# -s ASSERTIONS=1  - display more errors/warnings

sudo apt-get install libassimp-dev
# win32 API isn't available on the web



/home/arvydas/repos/emsdk/emscripten/1.38.31/cmake/Modules/Platform/Emscripten.cmake

# download assimp / follow compilation insturction / add extra flags
cmake CMakeLists.txt \
    -DASSIMP_BUILD_NO_EXPORT=1 \
    -DASSIMP_BUILD_NO_X_IMPORTER=1 \
    -DASSIMP_BUILD_NO_VTA_IMPORTER=1 \
    -DASSIMP_BUILD_NO_UC_IMPORTER=1 \
    -DASSIMP_BUILD_NO_TER_IMPORTER=1 \
    -DASSIMP_BUILD_NO_NDO_IMPORTER=1 \
    -DASSIMP_BUILD_NO_MDC_IMPORTER=1 \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_TOOLCHAIN_FILE=~/repos/emsdk/emscripten/1.38.31/cmake/Modules/Platform/Emscripten.cmake \
    -DEMSCRIPTEN=1 

cmake CMakeLists.txt \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_TOOLCHAIN_FILE=~/repos/emsdk/emscripten/1.38.31/cmake/Modules/Platform/Emscripten.cmake \
    -DEMSCRIPTEN=1 


set(ASSIMP_BUILD_FBX_IMPORTER OFF)
set(ASSIMP_BUILD_X_IMPORTER OFF)
set(ASSIMP_BUILD_VTA_IMPORTER OFF)
set(ASSIMP_BUILD_UC_IMPORTER OFF)
set(ASSIMP_BUILD_TER_IMPORTER OFF)
set(ASSIMP_BUILD_NDO_IMPORTER OFF)
set(ASSIMP_BUILD_MDC_IMPORTER OFF)

# -DEMSCRIPTEN=1 - ????


