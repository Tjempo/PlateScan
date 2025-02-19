CFLAGS = -std=c++17 -Wall -Wextra 
LIBS = $(pkg-config --cflags --libs opencv4) -L/home/tjempo/Documents/GIT/darknet -ldarknet -lcrypto -ltesseract

# Default argument if none is provided
ARGS ?= "ImageConfig.json"

all:

clean:
	rm -f ./src/plateScan

run:
	cd ./src && ./plateScan $(ARGS)

build:
	g++ -o ./src/plateScan $(find ./src -name '*.cpp' -or -name '*.hpp') $(CFLAGS) $(LIBS)
