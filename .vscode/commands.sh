g++ -Wall -std=c++17 cpp/src/*.cpp lib/* -o "cpp/bin/main.out" -I cpp/include -lGLESv2 -lSDL2

em++ -std=c++11 cpp/src/*.cpp -I cpp/include -o module.js -s WASM=1 -s USE_SDL=2 -s FULL_ES3=1 -O3 -s USE_WEBGL2=1 --preload-file filesystem

--disable-gpu-vsync

cd ${workspaceFolder}
python -m SimpleHTTPServer 8080


# -s NO_EXIT_RUNTIME=1  do not exit after `main()` function is executed
# -std=c++1z use C++17 standart
# -s SIDE_MODULE=1 create as dynamic lib, to use it `loadWebAssemblyModule`
# --preload-file files package files to .data file (accessed same as fstream in web)
# --embed-file files are inside .js fil