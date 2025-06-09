pushd pug/src
make clean
# export CFLAGS="-static -DUSE_READLINE=1"
CFLAGS="-DUSE_READLINE=1" LDFLAGS="-lm -ledit" make

# make gofer_uni
make clean
CFLAGS="-DUSE_READLINE=1" LDFLAGS="-lm -ledit" make gofer_uni

rm -rf pug
mkdir -p pug
cp gofer ./pug/pug
cp gofer_uni ./pug/pug_uni

cp ../pusimple.pre ../pustd.pre ../pucc28.pre ./pug
cd pug
# Zip all files in the pug directory
zip -r ../pug_linux.zip *
popd