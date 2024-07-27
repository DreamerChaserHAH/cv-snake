OS := $(shell uname)

all: setup

setup: setup-torch setup-submodules setup-opencv

setup-torch:
ifeq ($(OS),Darwin)
	@echo "Downloading libtorch for MacOS"
	rm -rf libtorch
	curl -L https://github.com/mlverse/libtorch-mac-m1/releases/download/LibTorchOpenMP/libtorch-v2.1.0.zip -o libs/libtorch.zip
	unzip libs/libtorch.zip -d libs/libtorch
	rm libs/libtorch.zip
else
	@echo "Running on a different operating system"
endif

setup-opencv:
ifeq ($(OS),Darwin)
	@echo "Downloading OpenCV for MacOS"
	brew install opencv
else
	@echo "Running on a different operating system"
endif

setup-submodules:
	@echo "Initializing and updating git submodules"
	git submodule update --init --recursive