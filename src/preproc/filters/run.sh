make && for f in output/*.jpeg; do ./processing $f && mv output.bmp $f.bmp; done
make clean
