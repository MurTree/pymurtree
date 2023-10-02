#include "parameters.h"
#include "parameter_handler.h"
#include "solver.h"
#include "solver_result.h"
#include "feature_vector_binary.h"
#include "file_reader.h"
#include "exporttree.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>

namespace py = pybind11;
using namespace MurTree;
// This function is used to convert a numpy array to a vector of vectors holding feature vector binary types
// that were defined in the murtree library
std::vector<std::vector<FeatureVectorBinary>> ReadDataDL(const std::vector<std::vector<int>>& vector, int duplicate_instances_factor)
{
    runtime_assert(duplicate_instances_factor > 0);
    runtime_assert(vector.size() > 0);

    std::vector<std::vector<FeatureVectorBinary>> feature_vectors;

    int nrows = vector.size();
    int ncols = vector[0].size();

    int id = 0;
    int num_features = ncols - 1;
    for (int i = 0; i < nrows; i++)
    {
        int label = vector[i][0];

        std::vector<bool> v(num_features);
        for (int j = 0; j < num_features; j++)
        {
            v[j] = (vector[i][j+1] == 1);
        }

        if (feature_vectors.size() <= label) { feature_vectors.resize(label+1); } //adjust the vector to take into account the new label (recall that labels are expected to be from 0..num_labels-1
        for (int i = 0; i < duplicate_instances_factor; i++)
        {
            feature_vectors[label].push_back(FeatureVectorBinary(v, id));
            id++;            
        }       
    }
    return feature_vectors;
}

// Utility function to construct a ParameterHandler object
ParameterHandler createParameters( unsigned int time, unsigned int max_depth,
unsigned int max_num_nodes, float sparse_coefficient, bool verbose,
bool all_trees, bool incremental_frequency, bool similarity_lower_bound,
unsigned int node_selection, unsigned int feature_ordering,
int random_seed, unsigned int cache_type, int duplicate_factor)
{
    ParameterHandler ph = DefineParameters();
    // ph.SetStringParameter("file", "./pymurtree_data/data.txt");
    ph.SetFloatParameter("time", time);
    ph.SetIntegerParameter("max-depth", max_depth);
    ph.SetIntegerParameter("max-num-nodes", max_num_nodes);
    ph.SetFloatParameter("sparse-coefficient", sparse_coefficient);
    ph.SetBooleanParameter("verbose", verbose);
    ph.SetBooleanParameter("all-trees", all_trees);
    ph.SetBooleanParameter("incremental-frequency", incremental_frequency);
    ph.SetBooleanParameter("similarity-lower-bound", similarity_lower_bound);
    ph.SetIntegerParameter("random-seed", random_seed);
    ph.SetIntegerParameter("duplicate-factor", duplicate_factor);
    // Node selection
    if (node_selection == 1) {
        ph.SetStringParameter("node-selection", "post-order");
    }
    else {
        ph.SetStringParameter("node-selection", "dynamic");
    }
    // Feature ordering
    if (feature_ordering == 1) {
        ph.SetStringParameter("feature-ordering", "random");
    } 
    else if (feature_ordering == 2) {
        ph.SetStringParameter("feature-ordering", "gini");
    }
    else {
        ph.SetStringParameter("feature-ordering", "in-order");
    }
    // Cache Type
    if (cache_type == 1) {
        ph.SetStringParameter("cache-type", "branch");
    }
    else if (cache_type == 2) {
        ph.SetStringParameter("cache-type", "closure");
    }
    else {
        ph.SetStringParameter("cache-type", "dataset");
    }
    return ph;
}


PYBIND11_MODULE(lib, m) {

    // This exposure is to test that we can properly pass the data type we need to pass to the solver
    py::class_<FeatureVectorBinary> feature_vector_binary(m, "FeatureVectorBinary");
    
    // Expose the ReadDataDL function in the file reader for testing purposes
    m.def("_read_data_dl", &FileReader::ReadDataDL, py::arg("filename"), py::arg("duplicate_instances_factor"), 
          "Reads file and returns feature vectors");
    
    // Read data coming as a numpy array that is then converted to a vector of vectors
    // To do that we need to use the numpy_to_vectors function and then
    // pass the result to the ReadDataDL function
    m.def("_nparray_to_feature_vectors", [](py::array_t<int, py::array::c_style>& arr, int duplicate_instances_factor) -> std::vector<std::vector<FeatureVectorBinary>> {
        std::vector<std::vector<int>> vec;
        // Convert the pybind11 array to a std::vector<std::vector<int>>
        for (py::size_t i = 0; i < arr.shape(0); ++i) {
            auto row = arr.data(i);
            vec.emplace_back(row, row + arr.shape(1));
        }
        // Pass the converted vector to ReadDataDLFromVector
        return ReadDataDL(vec, duplicate_instances_factor);
    }, py::arg("np_array"), py::arg("duplicate_instances_factor"), "Turns numpy array data into a vector of vectors of feature vectors");

    py::class_<ParameterHandler> parameter_handler(m, "ParameterHandler");

    // Expose the create parameters function to python so that we can create a parameter handler object from python
    // To invoke the Solver constructor
    m.def("_create_parameters", createParameters, py::arg("time"), py::arg("max_depth"),
            py::arg("max_num_nodes"), py::arg("sparse_coefficient"), py::arg("verbose"),
            py::arg("all_trees"), py::arg("incremental_frequency"), py::arg("similarity_lower_bound"),
            py::arg("node_selection"), py::arg("feature_ordering"), py::arg("random_seed"),
            py::arg("cache_type"), py::arg("duplicate_factor"), "Creates a parameter handler object");



    // Bindings for the SolverResult class

    py::class_<SolverResult> solver_result(m, "SolverResult");

    solver_result.def("_predict", [](const SolverResult &solverresult, const std::vector<std::vector<int>> arr){
        std::vector<int> predictions;
        for (int i = 0; i < arr.size(); i++) {
            MurTree::FeatureVectorBinary row({std::vector<bool>(arr[i].begin(), arr[i].end())}, i);
            predictions.push_back(solverresult.decision_tree_->Classify(&row));
        }
        return py::array_t<int>(predictions.size(), predictions.data()); 
    });

    solver_result.def("misclassification_score", [](const SolverResult &solverresult) {
        py::scoped_ostream_redirect stream(std::cout, py::module_::import("sys").attr("stdout"));
        return solverresult.misclassifications;
    });

    solver_result.def("tree_depth", [](const SolverResult &solverresult) {
        py::scoped_ostream_redirect stream(std::cout, py::module_::import("sys").attr("stdout"));
        return solverresult.decision_tree_->Depth();
    });

    solver_result.def("tree_nodes", [](const SolverResult &solverresult) {
        py::scoped_ostream_redirect stream(std::cout, py::module_::import("sys").attr("stdout"));
        return solverresult.decision_tree_->NumNodes();
    });
    

    // Bindings for the MurTree::Solver class

    py::class_<Solver> solver(m, "Solver");
    
    solver.def(py::init([](std::vector<std::vector<int>> arr,
    unsigned int time, unsigned int max_depth,
    unsigned int max_num_nodes, float sparse_coefficient, bool verbose,
    bool all_trees, bool incremental_frequency, bool similarity_lower_bound,
    unsigned int node_selection, unsigned int feature_ordering,
    int random_seed, unsigned int cache_type, int duplicate_factor) 
    {
        py::scoped_ostream_redirect stream(std::cout, py::module_::import("sys").attr("stdout"));

        ParameterHandler ph = createParameters(time, max_depth, max_num_nodes,
        sparse_coefficient, verbose, all_trees, incremental_frequency,
        similarity_lower_bound, node_selection, feature_ordering, random_seed,
        cache_type, duplicate_factor);

        // Carry-out actions from murtree function main in main.cpp
        CheckParameters(ph); // TODO: This should be uncommented later
        if (verbose) { 
            ph.PrintParameterValues();
        }
        // Construct the Solver object
        // We turn the numpy array into a vector of feature vectors before
        return new Solver(ph, ReadDataDL(arr, duplicate_factor));

    }), py::keep_alive<0, 1>());

    solver.def("solve", [](Solver &solver, 
    py::array_t<int, py::array::c_style>& arr, unsigned int time, 
    unsigned int max_depth, unsigned int max_num_nodes, 
    float sparse_coefficient, bool verbose, bool all_trees, 
    bool incremental_frequency, bool similarity_lower_bound,
    unsigned int node_selection, unsigned int feature_ordering,
    int random_seed, unsigned int cache_type, int duplicate_factor)
    {
        py::scoped_ostream_redirect stream(std::cout, py::module_::import("sys").attr("stdout"));

        ParameterHandler ph = createParameters(time, max_depth, max_num_nodes,
        sparse_coefficient, verbose, all_trees, incremental_frequency,
        similarity_lower_bound, node_selection, feature_ordering, random_seed,
        cache_type, duplicate_factor);
        CheckParameters(ph);

        return solver.Solve(ph);
    });
    

    // Bindings for the ExportTree class

    solver_result.def("export_text", [](const SolverResult &solverresult, std::string filepath) {
        py::scoped_ostream_redirect stream(std::cout, py::module_::import("sys").attr("stdout"));
        std::vector<std::string> featurenames;
        std::unordered_map<unsigned int, std::string> classnames;
        ExportTree::exportText(solverresult.decision_tree_, featurenames, classnames, filepath);
    });

    solver_result.def("export_dot", [](const SolverResult &solverresult, std::string filepath) {
        py::scoped_ostream_redirect stream(std::cout, py::module_::import("sys").attr("stdout"));
        std::vector<std::string> featurenames;
        std::unordered_map<unsigned int, std::string> classnames;
        ExportTree::exportDot(solverresult.decision_tree_, featurenames, classnames, filepath);
    }); 

}

