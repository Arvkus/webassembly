# webassembly
Demo: https://arvkus.github.io/webassembly/index.html

```sh
# compile
g++ cpp/src/* \
    -o \"cpp/bin/main.out\" \
    -I cpp/include \
    -l GLESv2 -l SDL2 -l assimp \
    -std=c++17 -Wall 

em++ -std=c++1z cpp/src/* cpp/lib/* -I cpp/include -o module.js \
    -s WASM=1 -s USE_SDL=2 -s FULL_ES3=1 -s USE_WEBGL2=1 \
    --preload-file filesystem -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1
```
