# OCR EMMA
### MADI'S TEAM PRESENT

> Our OCR is made by four students of EPITA, for the third semester.

This OCR resolves a Sudoku grid on a picture. Our Neural Network can detect any digits on any pictures.
Our OCR can be trained to detect more numbers than now.

### INSTALLATION

```sh
cd src/
make
```

### COMMAND AND COMPONENTS

#### boards
This folder contains every grids that can be tested on this OCR.

#### dataset
We took our pictures on an picture bank from internet : we used a generator to make more than 1000 different image file for every digits (1 to 9).

#### Neural Network
```sh
./xor
```
Our XOR can display at least 10 000 000 populations. It shows every 100 000 populations the results.

#### processing
##### filters
```sh
./processing [PICTURE/S NAME] [NAME PICTURE AFTER FILTERS]
```
The input folder contains initial pictures without being analysed yet.
The output folder contains pitures post-treatment.
When the "make clean" command is executed, all the files in the output's folder are being deleted.

##### rotation
```sh
./rot [PICTURE/S NAME] [ANGLE OF ROTATION]
```
The new picture is named as "picturerotate.bmp".
The new picture can be seen by executing 'display picturerotate.bmp' on the shell.

##### segmentation
```sh
./segmentation [PICTURE/S NAME]
```
All the pictures generated by the segmentation are saved in the segmentation folder.

##### solver
```sh
./solver [GRID/'S NAME]
```
The grid's result is printed on the shell by making "cat [GRID'S NAME].result" command.
