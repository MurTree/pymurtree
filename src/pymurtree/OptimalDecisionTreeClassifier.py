import pandas as pd
import numpy as np
from . import lib
from typing import Dict
from pymurtree.parameters import Parameters

def standardize_to_dtype_int32(np_array: np.ndarray) -> np.ndarray:
    """
    By using a dtype object, we can make the method more robust 
    and ensure consistent behavior across different systems and configurations.

    Parameters
    ----------
        np_array (numpy.ndarray): A 2D array that represents the input features of the training data.
            Each row corresponds to an instance, and each column corresponds to a feature.
    
    Returns
    -------
        numpy.ndarray: A 2D array that represents the input features of the training data.

    """
    int32_dtype = np.dtype(np.int32)
    if np_array.dtype != int32_dtype:
        np_array = np_array.astype(int32_dtype)
        return np_array
    else:
        return np_array
    
class OptimalDecisionTreeClassifier:
    """
    OptimalDecisionTreeClassifier is a class that represents a PyMurTree model.

    """
    def __init__(self,
                 time: int = 600,
                 max_depth: int = 3,
                 max_num_nodes: int = None,
                 sparse_coefficient: float = 0.0,
                 verbose: bool = False,
                 all_trees: bool = False,
                 incremental_frequency: bool = True,
                 similarity_lower_bound: bool = True,
                 node_selection: int = 0,
                 feature_ordering: int = 0,
                 random_seed: int = 3,
                 cache_type: int = 0,
                 duplicate_factor: int = 1
                ) -> None:

        self.__solver = None
        self.__tree = None

        if max_num_nodes is None:
            max_num_nodes = 2**max_depth - 1
        max_num_nodes = min(2**max_depth - 1, max_num_nodes)

        self.__params = Parameters(time, max_depth, max_num_nodes,
                                   sparse_coefficient, verbose,
                                   all_trees, incremental_frequency, 
                                   similarity_lower_bound, node_selection,
                                   feature_ordering, random_seed,
                                   cache_type, duplicate_factor)
    

    def fit(self,
            x: np.ndarray, 
            y: np.ndarray,
            time: int = None,
            max_depth: int = None,
            max_num_nodes: int = None,
            sparse_coefficient: float = None,
            verbose: bool = None,
            all_trees: bool = None,
            incremental_frequency: bool = None,
            similarity_lower_bound: bool = None,
            node_selection: int = None,
            feature_ordering: int = None,
            random_seed: int = None,
            cache_type: int = None,
            duplicate_factor: int = None) -> None:
        """   
        Fits a PyMurTree model to the given training data.

        Parameters
        ----------
            x : (numpy.ndarray)
                A 2D array that represents the input features of the training data.
            y : 
                (numpy.ndarray): A 1D array that represents the target variable of the training data.
            time : (int, optional) 
                The maximum time budget in seconds allowed for fitting the model. Defaults to None.
            max_depth : (int, optional)
                The maximum depth of the trees in the ensemble. Defaults to None.
            max_num_nodes :(int, optional) 
                The maximum number of nodes for each tree in the ensemble. Defaults to None.
            sparse_coefficient : (float, optional)
                The sparsity coefficient used for tree pruning. Defaults to None.
            verbose : (bool, optional)
                If True, prints the progress of the training process. Defaults to None.
            all_trees : (bool, optional)
                If True, returns all trees generated during the training process. Defaults to None.
            incremental_frequency : (bool, optional) 
                If True, uses incremental frequency counting. Defaults to None.
            similarity_lower_bound : (bool, optional)
                If True, uses similarity lower bound pruning. Defaults to None.
            node_selection :(int, optional)
                The method used for node selection. Defaults to None.
            feature_ordering : (int, optional) 
                The method used for feature ordering. Defaults to None.
            random_seed : (int, optional) 
                The random seed for the training process. Defaults to None.
            cache_type (int, optional): The type of cache used for storing the intermediate results. Defaults to None.
            duplicate_factor (int, optional): The duplicate factor used for parallelization. Defaults to None.

        Returns
        -------
            None

        Raises
        ------
            ValueError: If x or y is None or if they have a different number of rows.

        Examples
        --------
            >>> model = PyMurTree()
            >>> x_train = np.array([[1, 2], [3, 4]])
            >>> y_train = np.array([0, 1])
            >>> model.fit(x_train, y_train)
        """
        # Check data entry
        if x is None:
            raise ValueError('x is None')
        if y is None:
            raise ValueError('y is None')
        if x is not None and y is not None:
            x = standardize_to_dtype_int32(x) # These are required otherwise the model will not work and test will fail
            y = standardize_to_dtype_int32(y)
            if x.shape[0] == y.shape[0]:
                    arr = np.concatenate((y.reshape(-1,1), x), axis=1)
            else: 
                raise ValueError('x and y have different number of rows')
            
        if time is not None:
            self.__params.time = time
        if max_depth is not None:
            self.__params.max_depth = max_depth
        if max_num_nodes is not None:
            self.__params.max_num_nodes = max_num_nodes
        if sparse_coefficient is not None:
            self.__params.sparse_coefficient = sparse_coefficient
        if verbose is not None:
            self.__params.verbose = verbose
        if all_trees is not None:
            self.__params.all_trees = all_trees
        if incremental_frequency is not None:
            self.__params.incremental_frequency = incremental_frequency
        if similarity_lower_bound is not None:
            self.__params.similarity_lower_bound = similarity_lower_bound
        if node_selection is not None:
            self.__params.node_selection = node_selection
        if feature_ordering is not None:
            self.__params.feature_ordering = feature_ordering
        if random_seed is not None:
            self.__params.random_seed = random_seed
        if cache_type is not None:
            self.__params.cache_type = cache_type
        if duplicate_factor is not None:
            self.__params.duplicate_factor = duplicate_factor

        # Initialize solver (call cpp Solver class constructor)
        if self.__solver is None:
            self.__solver = lib.Solver(arr,
                                       self.__params.time,
                                       self.__params.max_depth,
                                       self.__params.max_num_nodes,
                                       self.__params.sparse_coefficient,
                                       self.__params.verbose,
                                       self.__params.all_trees,
                                       self.__params.incremental_frequency,
                                       self.__params.similarity_lower_bound,
                                       self.__params.node_selection,
                                       self.__params.feature_ordering,
                                       self.__params.random_seed,
                                       self.__params.cache_type,
                                       self.__params.duplicate_factor)
        
        # Creates the tree that will be used for predictions
        self.__tree = self.__solver.solve(arr,
                                          self.__params.time,
                                          self.__params.max_depth,
                                          self.__params.max_num_nodes,
                                          self.__params.sparse_coefficient,
                                          self.__params.verbose,
                                          self.__params.all_trees,
                                          self.__params.incremental_frequency,
                                          self.__params.similarity_lower_bound,
                                          self.__params.node_selection,
                                          self.__params.feature_ordering,
                                          self.__params.random_seed,
                                          self.__params.cache_type,
                                          self.__params.duplicate_factor)
        
        # This should return a tree object that will be used for predictions
        return self.__tree        

    def predict(self, x: np.ndarray) -> np.ndarray:
        """
        Predicts the target variable for the given input features.

        Parameters
        ----------
            x (numpy.ndarray): A 2D array that represents the input features of the test data.
                Each row corresponds to an instance, and each column corresponds to a feature.

        Returns
        -------
            numpy.ndarray: A 1D array that represents the predicted target variable of the test data.
                The i-th element in this array corresponds to the predicted target variable for the i-th instance in `x`.
        """
        # Check data entry
        x = standardize_to_dtype_int32(x)
        predictions = self.__tree._predict(x) # Obtain predictions from the decision tree model using the _predict binding
        return predictions # Return the 1D array of predictions/labels

    

    def score(self) -> int:
        """
        Returns the misclassification score of the tree.

        Parameters
        ----------
            None
        
        Returns
        -------
            int: The misclassification score of the tree.
        """
        return self.__tree.misclassification_score()

    def depth(self) -> int:
        """
        Returns the depth of the tree.

        Parameters
        ----------
            None

        Returns
        -------
            int: The depth of the tree.    

        """
        return self.__tree.tree_depth()

    def num_nodes(self) -> int:
        '''
        Returns the number of nodes in the tree.

        Parameters
        ----------
            None
        
        Returns
        -------
            int: The number of nodes in the tree.
        '''

        return self.__tree.tree_nodes()

    def export_text(self,
                    out_file: str = '',
                    feature_names: np.ndarray = None,
                    class_names: Dict[int, str] = None) -> None:
        '''
        Create a text representation of all the rules in the decision tree. 
        Text is written to out_file if given, otherwise it is displayed on screen (standard ouput).

        Parameters
        ----------
            out_file (str, optional): Name of the output file.
            feature_names (numpy.ndarray, optional): 1D Numpy array that represents the names of the features.
            class_names (dict, optional): Dictionary with int keys and str values that represent the class names.
        
        Returns
        -------
            None
        '''
        
        if self.__tree is None:
            raise ValueError('self.__tree is None')
        else:
            if feature_names is None:
                if class_names is None:
                    self.__tree.export_text(out_file)
                else:
                    self.__tree.export_text(out_file, classnames = class_names)
            else:
                if class_names is None:
                    self.__tree.export_text(out_file, feature_names)
                else:
                    self.__tree.export_text(out_file, feature_names, class_names)


    def export_dot(self,
                   out_file: str = '',
                   feature_names: np.ndarray = None,
                   class_names: Dict[int, str] = None) -> None:
        '''
        Export the decision tree in DOT format for visualization with Graphviz.
        DOT representation is written to out_file if give, otherwise it is displayed on screen (standard output)

        Parameters
            out_file (str, optional): Name of the output file.
            feature_names (numpy.ndarray, optional): 1D Numpy array that represents the names of the features.
            class_names (dict, optional): Dictionary with int keys and str values that represent the class names.
        
        Returns
            None
        '''
        if self.__tree is None:
            raise ValueError('self.__tree is None')
        else:
            if feature_names is None:
                if class_names is None:
                    self.__tree.export_dot(out_file)
                else:
                    self.__tree.export_dot(out_file, classnames = class_names)
            else:
                if class_names is None:
                    self.__tree.export_dot(out_file, feature_names)
                else:
                    self.__tree.export_dot(out_file, feature_names, class_names)
