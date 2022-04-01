# OCR EMMA
> Our OCR is made by four students of EPITA, for the third semester.

This OCR resolves a Sudoku grid on a picture. Our Neural Network can detect digits.
Our OCR is powered by a simple neuronal network who is able to recognize simple digits on a given picture.

## Requirements

Use the SDL library [SDL](https://www.libsdl.org/) to compile this OCR.

For Ubuntu operating systems :

```bash
sudo apt install libsdl1.2-dev
```

## Usage

First compile this project by :

```sh
cd src && make

To clean this OCR :
```sh
cd src
make clean
```

### 1) Neural Network
There is a working neuronal network learning from XOR logical function. 

```sh
./xor [POPULATION/S NUMBER]
```

Our XOR can learn from 10.000.000 randoms inputs in ~20 seconds. It displays probability as a number between [0,1]. With 300 neurons on hidden layer, probability of success can be more than 99.9%. 

### 2) Digital image processing
#### a) Filters
```sh
./processing [INPUT PICTURE/S PATH] [OUTPUT PICTURE/S NAME WITH FILTERS]
```
The input folder contains initial pictures without being analyzed yet.

The output folder contains pictures with post-treatment.

"make clean" will purge every files inside output folders. 

#### b) Rotate images
```sh
./rot [INPUT PICTURE/S PATH] [ANGLE OF ROTATION]
```
The new picture is named as "picturrotate.bmp".

#### c) Segmentation
```sh
./segmentation [INPUT PICTURE/S PATH]
```
Pictures will be saved inside executable folder. 

#### d) Sudoku solver
```sh
./solver [GRID/S PATH]
```
This solver only works with 9x9 sudoku grid. 

Input files needs to be same as grid exemple files given inside the Solver folders.


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
