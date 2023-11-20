'''
Decription of test cases for readdata.py as user stories and acceptance criteria

Story: As a user I want to read a dataset from a file so that I can use it with pymurtree
AC: The file is written in a MurTree-compatible format (see https://github.com/MurTree/murtree-data)
AC: The file is read and converted to numpy arrays (x,y) where y is a 1D array holding the first column of the dataset 
and x is a 2D array holding the rest of the columns.
AC: Features are binary values and labels are positive integers
'''
import pytest
import pandas as pd
import numpy as np
from pymurtree.readdata import *
import pymurtree.lib as lib
import pprint

TRAIN_DATA = "./tests/fixtures/test_dataset.txt"

@pytest.fixture
def dl_from_file() -> np.ndarray:
    """ Read data from MurTree formatted file and return as numpy array """
    x, y = load_murtree_dataset_to_pandas_dataframes(TRAIN_DATA)
    x = x.to_numpy()
    y = y.to_numpy()
    return np.concatenate((y.reshape(-1,1), x), axis=1).astype(np.int32)


@pytest.fixture
def dl_data_sample() -> np.ndarray:

    return np.array([[1, 0, 1, 0, 1, 0, 1, 0, 1, 0],
                     [1, 0, 1, 0, 1, 0, 1, 0, 1, 0],
                     [1, 0, 1, 0, 1, 0, 1, 0, 1, 0]
                    ]).astype(np.int32)
                    

def test_load_murtree_dataset_to_pandas_dataframes():
    x, y = load_murtree_dataset_to_pandas_dataframes(TRAIN_DATA)
    assert x is not None
    assert y is not None
    assert type(x) == pd.core.frame.DataFrame
    assert type(y) == pd.core.series.Series
    assert x.shape == (399,17)
    assert y.shape == (399,)
    # check first and last values
    assert y.iloc[0] == 2
    assert y.iloc[-1] == 3
    assert x.iloc[0,:].tolist() == [0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0]
    assert x.iloc[-1,:].tolist() == [1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0]

def test_load_murtree_dataset_to_numpy_arrays():
    x, y = load_murtree_dataset_to_numpy_arrays(TRAIN_DATA)
    assert x is not None
    assert y is not None
    assert type(x) == np.ndarray
    assert type(y) == np.ndarray
    assert x.shape == (399,17)
    assert y.shape == (399,)
    # check first and last values
    assert y[0] == 2
    assert y[-1] == 3
    assert (x[0] == [0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0]).all()
    assert (x[-1] == [1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0]).all()

    
def test_compare_feature_vectors(dl_from_file):
    """ Test that we get exactly the same feature vectors from the file and from the numpy array """
    feature_vectors_from_file = lib._read_data_dl(TRAIN_DATA, 1)
    assert feature_vectors_from_file is not None
    assert type(feature_vectors_from_file) == list
    assert type(feature_vectors_from_file[0]== lib.FeatureVectorBinary)

    feature_vectors = lib._nparray_to_feature_vectors(dl_from_file, 1)
    assert len(feature_vectors[0]) == len(feature_vectors_from_file[0])
    assert len(feature_vectors) == len(feature_vectors_from_file)