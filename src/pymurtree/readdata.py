import pandas
import numpy

def load_murtree_dataset_to_pandas_dataframes(path: str) -> tuple:
        """ 
        Reads a dataset from a plain text file with the following format:
            The first column contains the target variable (y), and the remaining columns 
            contain the features (x).
            All features must be 0 or 1 and classes should be equal or greater than zero
            All values should be space-separated integers.
            No headers or foooters allowed.
        The function is a wrapper for the pandas.read_csv function.
        It's provided as a utility function to easily load the datasets in https://github.com/MurTree/murtree-data 
            
        Parameters
        ----------
            path (str): path to the dataset

        Returns
        -------
            tuple: conatining:
                x (pandas.DataFrame): all columns from dataset except the first 
                y (pandas.Series): first column of dataset
        """
        data = pandas.read_csv(path, sep=' ', header=None)
        y = data.iloc[:, 0]
        x = data.iloc[:, 1:]
        return x, y


def load_murtree_dataset_to_numpy_arrays(path: str) -> tuple:
        """
        Reads a dataset from a plain text file with the following format:
            The first column contains the target variable (y), and the remaining columns 
            contain the features (x).
            All features must be 0 or 1 and classes should be equal or greater than zero
            All values should be space-separated integers.
            No headers or foooters allowed.
        The function is a wrapper for the numpy.loadtxt function
        It's provided as a utility function to easily load the datasets in https://github.com/MurTree/murtree-data 

        Parameters
        ----------
            path (str): path to the dataset

        Returns
        -------
            tuple: containing:
                x (numpy.ndarray): 2D array with all columns except the first 
                y (numpy.ndarray): 1D array with the first column
        """
        data = numpy.loadtxt(path, dtype=int, delimiter=' ')
        x = data[:, 1:]
        y = data[:, 0]
        return x, y