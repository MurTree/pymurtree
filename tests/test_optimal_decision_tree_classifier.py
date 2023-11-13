'''
For predict we use only 10 samples of iris_categorical_bin.txt which can be found
and downloaded here: https://github.com/MurTree/murtree-data/blob/main/NL/iris_categorical_bin.txt
'''

import pytest
import numpy as np
import os
import textwrap
from src.pymurtree import read_from_file
import pymurtree

TRAIN_DATA = "./tests/fixtures/test_dataset.txt"
EXPECTED_PREDICT_OUTPUT = "./tests/fixtures/expected_predict_output.txt"

# Define a fixture for input data with known output values
@pytest.fixture
def train_data():
    x, y = read_from_file(TRAIN_DATA)
    x = x.to_numpy()
    y = y.to_numpy()
    return x, y

@pytest.fixture
def x_train_data(train_data):
    return train_data[0]

@pytest.fixture
def y_train_data(train_data):
    return train_data[1]

# Define a fixture for the decision tree
@pytest.fixture
def decision_tree(x_train_data, y_train_data):
    decision_tree = pymurtree.OptimalDecisionTreeClassifier(max_depth=4, duplicate_factor=1, max_num_nodes=15)
    decision_tree.fit(x_train_data, y_train_data)
    return decision_tree

@pytest.fixture
def invalid_input():
    # Define input data with invalid values
    # ...
    # make a numpy arrau where features are not 0 or 1
    return np.array([   [1, 0, 0, 0, 0, 1, 1, 0, 3 , 1, 0, 0 ],
                        [1, 0, 0, 0, 1, 0, 1, 0, 0 , 1, 0, 0 ],
                        [1, 0, 0, 0, 1, 0, 1, 0, 0 , 1, 0, 0 ],
                        [1, 0, 0, 0, 1, 0, 1, 4, 0 , 1, 0, 0 ],
                        [1, 0, 0, 0, 0, 1, 1, 0, 0 , 1, 0, 0 ],
                        [1, 0, 0, 0, 0, 1, 1, 5, 0 , 1, 0, 0 ],
                        [1, 0, 0, 0, 0, 1, 1, 0, 0 , 1, 0, 0 ],
                        [1, 0, 0, 0, 0, 1, 1, 0, 0 , 1, 0, 0 ],
                        [1, 0, 0, 1, 0, 0, 1, 0, 0 , 1, 0, 0 ],
                        [1, 0, 0, 0, 1, 0, 1, 0, 0 , 1, 0, 0 ]
                        ])

# Define a fixture for output values that are expected to be predicted by the decision tree
@pytest.fixture
def expected_fit_output(decision_tree, expected_predict_output):
    # Define output values that are expected to be predicted by the decision tree
    # ...
    # make it a numpy array from anneal.txt
    # This we do from generating the model with cpp library
    # then classifying it and storing it in a 1d array
    pass

@pytest.fixture
def expected_predict_output():
    return np.loadtxt(EXPECTED_PREDICT_OUTPUT, dtype=np.int32)

@pytest.fixture
def decision_tree_for_export_tests():
    x = np.array([[0, 1, 0, 1, 0],
                  [1, 0, 0, 1, 1],
                  [1, 1, 0, 0, 1],
                  [0, 0, 0, 1, 1],
                  [1, 0, 1, 1, 1]]) # features
    y = np.array([2, 3, 4, 2, 3]) # labels
    decision_tree_for_export_tests = pymurtree.OptimalDecisionTreeClassifier()
    decision_tree_for_export_tests.fit(x, y)
    return decision_tree_for_export_tests

@pytest.fixture
def feature_names_export_tests():
    feature_names_export_tests = np.array(['myft0', 'myft1', 'myft2', 'myft3', 'myft4'])
    return feature_names_export_tests

@pytest.fixture
def class_names_export_tests():
    label_names_export_tests = {
        0 : 'mylb0',
        1 : 'mylb1',
        2 : 'mylb2',
        3 : 'mylb3',
        4 : 'mylb4',
        5 : 'mylb5'
    }
    return label_names_export_tests

@pytest.fixture
def decision_tree_text_output():
    decision_tree_text_output = textwrap.dedent('''\
        |---feature #0 is true
        |   |---feature #1 is true
        |   |   |---class: 4
        |   |---feature #1 is false
        |   |   |---class: 3
        |---feature #0 is false
        |   |---class: 2''')
    return decision_tree_text_output

@pytest.fixture
def decision_tree_text_output_with_feature_names():
    decision_tree_text_output_with_feature_names = textwrap.dedent('''\
        |---myft0 is true
        |   |---myft1 is true
        |   |   |---class: 4
        |   |---myft1 is false
        |   |   |---class: 3
        |---myft0 is false
        |   |---class: 2''')
    return decision_tree_text_output_with_feature_names

@pytest.fixture
def decision_tree_text_output_with_class_names():
    decision_tree_text_output_with_class_names = textwrap.dedent('''\
        |---feature #0 is true
        |   |---feature #1 is true
        |   |   |---mylb4
        |   |---feature #1 is false
        |   |   |---mylb3
        |---feature #0 is false
        |   |---mylb2''')
    return decision_tree_text_output_with_class_names

@pytest.fixture
def decision_tree_text_output_with_feature_and_class_names():
    decision_tree_text_output_with_feature_and_class_names = textwrap.dedent('''\
        |---myft0 is true
        |   |---myft1 is true
        |   |   |---mylb4
        |   |---myft1 is false
        |   |   |---mylb3
        |---myft0 is false
        |   |---mylb2''')
    return decision_tree_text_output_with_feature_and_class_names

@pytest.fixture
def decision_tree_dot_output():
    decision_tree_dot_output = textwrap.dedent('''\
		digraph Tree {
		node [shape=box, style="filled, rounded", fontname="helvetica", fontsize="8"] ;
		edge [fontname="helvetica", fontsize="6"] ;
		0 [label=<feature #0>, color="#8CB77F", fillcolor="#8CB77F"] ;
		1 [label=<class 2>, color="#B77F8C" fillcolor="#B77F8C"] ;
		0 -> 1 [label=" 0 "] ;
		2 [label=<feature #1>, color="#8CB77F", fillcolor="#8CB77F"] ;
		0 -> 2 [label=" 1 "] ;
		3 [label=<class 3>, color="#B77F8C" fillcolor="#B77F8C"] ;
		2 -> 3 [label=" 0 "] ;
		4 [label=<class 4>, color="#B77F8C" fillcolor="#B77F8C"] ;
		2 -> 4 [label=" 1 "] ;
		}''')
    return decision_tree_dot_output

@pytest.fixture
def decision_tree_dot_output_with_feature_names():
    decision_tree_dot_output_with_feature_names = textwrap.dedent('''\
		digraph Tree {
		node [shape=box, style="filled, rounded", fontname="helvetica", fontsize="8"] ;
		edge [fontname="helvetica", fontsize="6"] ;
		0 [label=<myft0>, color="#8CB77F", fillcolor="#8CB77F"] ;
		1 [label=<class 2>, color="#B77F8C" fillcolor="#B77F8C"] ;
		0 -> 1 [label=" 0 "] ;
		2 [label=<myft1>, color="#8CB77F", fillcolor="#8CB77F"] ;
		0 -> 2 [label=" 1 "] ;
		3 [label=<class 3>, color="#B77F8C" fillcolor="#B77F8C"] ;
		2 -> 3 [label=" 0 "] ;
		4 [label=<class 4>, color="#B77F8C" fillcolor="#B77F8C"] ;
		2 -> 4 [label=" 1 "] ;
		}''')
    return decision_tree_dot_output_with_feature_names

@pytest.fixture
def decision_tree_dot_output_with_class_names():
    decision_tree_dot_output_with_class_names = textwrap.dedent('''\
		digraph Tree {
		node [shape=box, style="filled, rounded", fontname="helvetica", fontsize="8"] ;
		edge [fontname="helvetica", fontsize="6"] ;
		0 [label=<feature #0>, color="#8CB77F", fillcolor="#8CB77F"] ;
		1 [label=<mylb2>, color="#B77F8C" fillcolor="#B77F8C"] ;
		0 -> 1 [label=" 0 "] ;
		2 [label=<feature #1>, color="#8CB77F", fillcolor="#8CB77F"] ;
		0 -> 2 [label=" 1 "] ;
		3 [label=<mylb3>, color="#B77F8C" fillcolor="#B77F8C"] ;
		2 -> 3 [label=" 0 "] ;
		4 [label=<mylb4>, color="#B77F8C" fillcolor="#B77F8C"] ;
		2 -> 4 [label=" 1 "] ;
		}''')
    return decision_tree_dot_output_with_class_names

@pytest.fixture
def decision_tree_dot_output_with_feature_and_class_names():
    decision_tree_dot_output_with_feature_and_class_names = textwrap.dedent('''\
		digraph Tree {
		node [shape=box, style="filled, rounded", fontname="helvetica", fontsize="8"] ;
		edge [fontname="helvetica", fontsize="6"] ;
		0 [label=<myft0>, color="#8CB77F", fillcolor="#8CB77F"] ;
		1 [label=<mylb2>, color="#B77F8C" fillcolor="#B77F8C"] ;
		0 -> 1 [label=" 0 "] ;
		2 [label=<myft1>, color="#8CB77F", fillcolor="#8CB77F"] ;
		0 -> 2 [label=" 1 "] ;
		3 [label=<mylb3>, color="#B77F8C" fillcolor="#B77F8C"] ;
		2 -> 3 [label=" 0 "] ;
		4 [label=<mylb4>, color="#B77F8C" fillcolor="#B77F8C"] ;
		2 -> 4 [label=" 1 "] ;
		}''')
    return decision_tree_dot_output_with_feature_and_class_names


##### TESTS #####

def test_predict(decision_tree, x_train_data, expected_predict_output):
    predict_output = decision_tree.predict(x_train_data)
    # check that predict_output is a numpy array
    print(predict_output)
    print(type(predict_output))
    assert predict_output is not None
    assert isinstance(predict_output, np.ndarray)
    assert (predict_output == expected_predict_output).all()

def test_predict_empty_input(decision_tree):
    # Ensure that an error or exception is raised when no input data is provided
    with pytest.raises(Exception):
        decision_tree._predict([])

def test_predict_invalid_input(decision_tree):
    # Ensure that an error or exception is raised when invalid input data is provided
    # ...
    with pytest.raises(Exception):
        decision_tree._predict(invalid_input)

##### export text tests #####

def test_export_text_exception_null_tree():
    # Exception is thrown when tree has not been created
    clf = pymurtree.OptimalDecisionTreeClassifier()
    with pytest.raises(Exception):
        clf._export_text()

def test_export_text_file_is_created(decision_tree, tmp_path):
    # Text export file is created with correct name
    txtfile = str(tmp_path) + "mytesttree.txt"
    decision_tree.export_text(txtfile)
    assert(os.path.isfile(txtfile)) 

def test_export_text_success_msg(decision_tree, tmp_path, capsys):
    # Message is printed after text file is created
    txtfile = str(tmp_path) + "testexporttextsuccessmsg.txt"
    decision_tree.export_text(txtfile)
    captured = capsys.readouterr()
    expected_message = "Tree saved in " + txtfile
    assert expected_message in captured.out

def test_export_text_file_output_matches_console_output(decision_tree, tmp_path, capsys):
    # Text console output is the same as text file
    decision_tree.export_text()
    captured = capsys.readouterr()
    txtfile = str(tmp_path) + "testexporttextfileoutputmatchesconsoleoutput.txt"
    decision_tree.export_text(txtfile)
    # Read the contents of the text file
    with open(txtfile, 'r') as file:
        txtfile_contents = file.read()
    # Check that the captured output is the same as the contents of the text file
    assert captured.out.strip() == txtfile_contents.strip()

def test_export_text_output_is_correct(decision_tree_for_export_tests, decision_tree_text_output, capsys):
    # Export Text console output is correct
    decision_tree_for_export_tests.export_text()
    captured = capsys.readouterr()
    assert captured.out.strip() == decision_tree_text_output

def test_export_text_output_with_feature_names_is_correct(decision_tree_for_export_tests,
                                                          feature_names_export_tests,
                                                          decision_tree_text_output_with_feature_names,
                                                          capsys):
	# Export Text console output with feature names is correct
	decision_tree_for_export_tests.export_text(featurenames = feature_names_export_tests)
	captured = capsys.readouterr()
	assert captured.out.strip() == decision_tree_text_output_with_feature_names

def test_export_text_output_with_class_names_is_correct(decision_tree_for_export_tests,
                                                        class_names_export_tests,
                                                        decision_tree_text_output_with_class_names,
                                                        capsys):
	# Export Text console output with class names is correct
	decision_tree_for_export_tests.export_text(classnames = class_names_export_tests)
	captured = capsys.readouterr()
	assert captured.out.strip() == decision_tree_text_output_with_class_names


def test_export_text_output_with_feature_and_class_names_is_correct(decision_tree_for_export_tests,
                                                          			feature_names_export_tests,
                                                          			class_names_export_tests,
																	decision_tree_text_output_with_feature_and_class_names,
                                                          			capsys):
	# Export Text console output with feature and class names is correct
	decision_tree_for_export_tests.export_text(featurenames = feature_names_export_tests, classnames = class_names_export_tests)
	captured = capsys.readouterr()
	assert captured.out.strip() == decision_tree_text_output_with_feature_and_class_names

##### export dot tests #####

def test_export_dot_exception_null_tree(decision_tree):
    # Exception is thrown when tree has not been created
    clf = pymurtree.OptimalDecisionTreeClassifier()
    with pytest.raises(Exception):
        clf._export_dot()

def test_export_dot_file_is_created(decision_tree, tmp_path):
    # Dot export file is created with correct name
    dotfile = str(tmp_path) + "mytesttree.txt"
    decision_tree.export_dot(dotfile)
    assert(os.path.isfile(dotfile)) 

def test_export_dot_success_msg(decision_tree, tmp_path, capsys):
    # Message is printed after dot file is created
    dotfile = str(tmp_path) + "testexporttextsuccessmsg.dot"
    decision_tree.export_text(dotfile)
    captured = capsys.readouterr()
    expected_message = "Tree saved in " + dotfile
    assert expected_message in captured.out
    
def test_export_dot_output_is_correct(decision_tree_for_export_tests, decision_tree_dot_output, tmp_path):
    # Export Dot file output is correct
    dotfile = str(tmp_path) + "testexportdotoutputiscorrect.dot"
    decision_tree_for_export_tests.export_dot(dotfile)
    # Read the contents of the text file
    with open(dotfile, 'r') as file:
        dotfile_contents = file.read()
    # Check that the captured output is the same as the contents of the text file
    assert dotfile_contents == decision_tree_dot_output

def test_export_dot_output_with_feature_names_is_correct(decision_tree_for_export_tests,
    											        feature_names_export_tests,
                                                        decision_tree_dot_output_with_feature_names,
														tmp_path):
	# Export Dot file output with feature names is correct
	dotfile = str(tmp_path) + "testexportdotoutputwithfeaturenamesiscorrect.dot"
	decision_tree_for_export_tests.export_dot(dotfile, feature_names_export_tests)
    # Read the contents of the text file
	with open(dotfile, 'r') as file:
		dotfile_contents = file.read()
    # Check that the captured output is the same as the contents of the text file
	assert dotfile_contents == decision_tree_dot_output_with_feature_names

def test_export_dot_output_with_class_names_is_correct(decision_tree_for_export_tests,
    											       class_names_export_tests,
                                                       decision_tree_dot_output_with_class_names,
													   tmp_path):
	# Export Dot file output with class names is correct
	dotfile = str(tmp_path) + "testexportdotoutputwithclassnamesiscorrect.dot"
	decision_tree_for_export_tests.export_dot(filepath = dotfile, classnames = class_names_export_tests)
    # Read the contents of the text file
	with open(dotfile, 'r') as file:
		dotfile_contents = file.read()
    # Check that the captured output is the same as the contents of the text file
	assert dotfile_contents == decision_tree_dot_output_with_class_names
     
def test_export_dot_output_with_feature_and_class_names_is_correct(decision_tree_for_export_tests,
																feature_names_export_tests,
																class_names_export_tests,
                                                        		decision_tree_dot_output_with_feature_and_class_names,
																tmp_path):
	# Export Dot file output with feature and class names is correct
	dotfile = str(tmp_path) + "testexportdotoutputwithfeatureandclassnamesiscorrect.dot"
	decision_tree_for_export_tests.export_dot(dotfile, feature_names_export_tests, class_names_export_tests)
    # Read the contents of the text file
	with open(dotfile, 'r') as file:
		dotfile_contents = file.read()
    # Check that the captured output is the same as the contents of the text file
	assert dotfile_contents == decision_tree_dot_output_with_feature_and_class_names