pushd pug/src
make clean
# export CFLAGS="-static -DUSE_READLINE=1"
# CFLAGS="-DUSE_READLINE=1" LDFLAGS="-lm -ledit" make
#CC = "emcc" make
# make gofer_uni
make clean
CFLAGS="-g -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=worker -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=4 -s ALLOW_MEMORY_GROWTH=1 -s DYNAMIC_EXECUTION=0 -s SINGLE_FILE=0 -s WASM_ASYNC_COMPILATION=0 -s EXPORTED_FUNCTIONS='["_main","_writeInputStream"]' -s EXPORTED_RUNTIME_METHODS=['FS','cwrap'] --preload-file /home/saurabh/p/pugofer/pug/src/pusimple_uni.pre" CC="emcc" LDFLAGS="-lm -pthread" make gofer_uni
#CFLAGS="" CC="emcc" LDFLAGS="-lm" make gofer_uni

rm -rf pug
mkdir -p pug
cp gofer ./pug/pug
cp gofer_uni ./pug/pug_uni

cp ../pusimple.pre ../pustd.pre ../pucc28.pre ./pug
cd pug
# Zip all files in the pug directory
zip -r ../pug_linux.zip *
popd