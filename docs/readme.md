# PyMurTree 

[![GitHub](https://img.shields.io/github/license/koenderinklab/openddm?)](https://github.com/MurTree/pymurtree/blob/master/LICENSE)
![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/MurTree/pymurtree/wheels.yml)
[![codecov](https://codecov.io/gh/koenderinklab/openddm/branch/master/graph/badge.svg?token=V4VZcNYyMG)](https://codecov.io/gh/MurTree/pymurtree)
![GitHub repo size](https://img.shields.io/github/repo-size/MurTree/pymurtree)

Warning:
> This repository is under active development and currently in a pre-alpha state. 
> The current version only supports Linux.


PyMurTree is a Python wrapper for the [MurTree project](https://github.com/DCC/murtree). The MurTree algorithm constructs optimal classification trees that minimize the misclassification score of a given dataset while respecting constraints on depth and number of feature nodes. The sparse objective, which penalizes each node added in the tree, is also supported.

---
## **Citation**

This package is based on the methods and algorithms described in:
> **"MurTree: Optimal Decision Trees via Dynamic Programming and Search"**  
> by Emir Demirović, Anna Lukina, Emmanuel Hebrard, Jeffrey Chan, James Bailey, Christopher Leckie, Kotagiri Ramamohanarao, and Peter J. Stuckey  
> Journal of Machine Learning Research (JMLR), 2022.  
> [Available online](https://jmlr.org/papers/v23/20-520.html)

---


## Installation

Before attempting to install pymurtree, make sure your system has the following software available: Python version 3.7 or higher and pip.

### Install from source using `pip`

```bash
git clone https://github.com/MurTree/pymurtree.git
cd pymurtree

# Optional: build pymurtree within a virtual environment
# python3 --version # check your python version
# sudo apt install python<your_python_version>-venv
# python3 -m venv env
# . env/bin/activate
 
# Install
pip install . 

# To install the dev version:
# pip install .[dev] 
```

### Building and running the tests

For building and running the tests, you will need the following software: pytest, a C++ compiler, and CMake   version 3.14 or higher.

```bash
# Run the Python tests
pytest

# Run the C++ tests
cd tests/cpptests
mkdir build
cd build
cmake ..
make
ctest
```

## Usage

### API

The full [API specification](https://github.com/MurTree/pymurtree/wiki/API-documentation) is available in the repo's Wiki. 

pymurtree is implemented as a thin Python wrapper around the main C++ MurTree application. The main functionality of MurTree is exposed in pymurtree via the OptimalDecisionTreeClassifier class. Utility functions to load training datasets and export the tree in text and dot formats are also included in the python package.

**OptimalDecisionTreeClassifier class**
- `constructor`: initialize the parameters of the model  
- `fit`: fit a decision tree classifier to the given training dataset
- `predict`: predict the labels for a set of features
- `score`: return the accuracy on the given test data and labels
- `depth`: return the depth of the tree
- `num_nodes`: return the number of nodes of the tree
- `export_text`: export decision tree in text format
- `export_dot`: export decision tree in DOT format

**Utility functions**
- `read_from_file`: read features and labels from file into a pandas dataframe
- `load_data`: read features and labels from file into a numpy array


### Example

After installing pymurtree you can use it in your Python code by importing the package. Here's an example of how to build a decision tree classifier from a training dataset, make predictions and export the tree for visualization with [graphviz](https://graphviz.org/):

```python
import pymurtree
import numpy

# Create training data
x = numpy.array([[0, 1, 0, 1], [1, 0, 0, 1], [1, 1, 0, 0]]) # features
y = numpy.array([5, 5, 4]) # labels

# Build tree classifier
model = pymurtree.OptimalDecisionTreeClassifier()
model.fit(x, y, max_depth=4, max_num_nodes=5, time=400)

# Predict labels for a new set of features
ft = numpy.array([[1, 0, 0, 1], [0, 0, 1, 1], [1, 0, 1, 0]])
labels = model.predict(ft)

# Visualize tree
model.export_text()

# Export tree in DOT format for visualization with graphviz
model.export_dot()
```

### Datasets

A collection of datsets compatible with pymurtree is available in [https://github.com/MurTree/murtree-data](https://github.com/MurTree/murtree-data)


## Contributing
There are different ways in which you can contribute to pymurtree:
- Try the package and let us know if its useful for your work. 
- If its useful, please star the repo.
- Report a bug or request a feature by opening an issue.
- Contribute to the codebase by opening a pull request.
- Currently pymurtree works only on Linux, any help picking open issues and fixing them for other platforms is welcome.

## License

[MIT LICENSE](LICENSE)
