OS := $(shell uname)

all: setup

setup: setup-torch setup-submodules setup-opencv setup-python

run-ai:
	@echo "Running AI"
	. .venv/bin/activate && python3 src/ai.py

setup-torch:
ifeq ($(OS),Darwin)
	@echo "Downloading libtorch for MacOS"
	rm -rf libs/libtorch
	curl -L https://github.com/mlverse/libtorch-mac-m1/releases/download/LibTorchOpenMP/libtorch-v2.1.0.zip -o libs/libtorch.zip
	unzip libs/libtorch.zip -d libs
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

setup-python:
	python -m venv .venv
	@echo "Installing Python dependencies"
	pip3 install -r requirements.txt