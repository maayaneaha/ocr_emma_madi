make && for f in *.jpeg; do ./processing $f && mv output.bmp output/$f.bmp; done
make clean
