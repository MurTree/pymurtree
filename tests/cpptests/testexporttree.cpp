#include <gtest/gtest.h>
#include "exporttree.h"
#include <unordered_map>

std::string readFile(std::string filepath) {
    // open the file for reading
	std::ifstream ifs(filepath, std::ifstream::in);
	if (!ifs.is_open() || ifs.fail()) {
		throw std::runtime_error("Failed to read testing file.");
	}

	// read
	std::string line, file;
	while (std::getline(ifs, line)) {
		file.append(line + "\n");
	}

	ifs.close();
	return file;
}

using ClassNames = std::unordered_map<unsigned int, std::string>;

class ExportTreeTest : public testing::Test 
{
protected:

    ExportTreeTest() {
        InitTreeSingleLabelNode();
        InitTreeSingleFeatureNode();
        InitTree5Nodes4Edges();
    }

    ~ExportTreeTest() {
        delete c1;
	    delete c2;
	    delete c3;
	    delete f3;
	    delete f7;
        delete tree_single_label_node;
        delete tree_single_feature_node;
    }

    MurTree::DecisionNode* tree_single_label_node;
    MurTree::DecisionNode* tree_single_feature_node;
    MurTree::DecisionNode* tree_5nodes_4edges;
    ClassNames tree_5nodes_4edges_classnames;
    std::vector<std::string> tree_5nodes_4edges_featurenames;

private:

    void InitTreeSingleLabelNode() {
        tree_single_label_node = MurTree::DecisionNode::CreateLabelNode(4);
    }

    void InitTreeSingleFeatureNode() {
        tree_single_feature_node = MurTree::DecisionNode::CreateFeatureNodeWithNullChildren(11);
    }

    void InitTree5Nodes4Edges() {
        // Create label nodes
	    c1 = MurTree::DecisionNode::CreateLabelNode(1);
	    c2 = MurTree::DecisionNode::CreateLabelNode(2);
	    c3 = MurTree::DecisionNode::CreateLabelNode(3);

	    // Create feature nodes
	    f3 = MurTree::DecisionNode::CreateFeatureNodeWithNullChildren(3);
	    f7 = MurTree::DecisionNode::CreateFeatureNodeWithNullChildren(7);
	
	    // Assign children
	    f7->left_child_ = c1;
	    f7->right_child_ = f3;
	    f3->left_child_ = c2;
    	f3->right_child_ = c3;

        tree_5nodes_4edges = f7;

        // Feature names
        tree_5nodes_4edges_featurenames = {
            "fftt0", "fftt1", "fftt2", "fftt3", "fftt4", "fftt5", "fftt6", "fftt7", "fftt8", "fftt9"
        };
        tree_5nodes_4edges_classnames = {
            {0, "cls0"}, {1, "cls1"}, {2, "cls2"}, {3, "cls3"}, {4, "cls4"}, {5, "cls5"}
        };
    }

    MurTree::DecisionNode *c1, *c2, *c3; // label nodes
    MurTree::DecisionNode *f3, *f7; // feature nodes
};

TEST_F(ExportTreeTest, NullTreeTextExportDoesNotThrowException) {
    std::vector<std::string> fn;
    ClassNames cn;
    EXPECT_NO_THROW(ExportTree::exportText(nullptr, fn, cn, ""));
}

TEST_F(ExportTreeTest, NullTreeDotExportDoesNotThrowException) {
    std::vector<std::string> fn;
    ClassNames cn;
    EXPECT_NO_THROW(ExportTree::exportDot(nullptr, fn, cn, ""));
}

TEST_F(ExportTreeTest, MessagePrintedIfTextFileCreationFails) {
    testing::internal::CaptureStdout();
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, fn, cn, "/etc/tree.txt");
    std::string expectedmsg = "Failed to write text output file. Message:";
    EXPECT_NE(testing::internal::GetCapturedStdout().find(expectedmsg), std::string::npos);
}

TEST_F(ExportTreeTest, MessagePrintedIfDotFileCreationFails) {
    testing::internal::CaptureStdout();
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, fn, cn, "/etc/tree.txt");
    std::string expectedmsg = "Failed to write dot output file. Message:";
    EXPECT_NE(testing::internal::GetCapturedStdout().find(expectedmsg), std::string::npos);
}

TEST_F(ExportTreeTest, TextFileIsCreatedAndHasExpectedName) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "TextFileIsCreatedAndHasExpectedName.txt");
    EXPECT_FALSE(readFile("TextFileIsCreatedAndHasExpectedName.txt").empty());
}

TEST_F(ExportTreeTest, DotFileIsCreatedAndHasExpectedName) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "DotFileIsCreatedAndHasExpectedName.txt");
    EXPECT_FALSE(readFile("DotFileIsCreatedAndHasExpectedName.txt").empty());
}

TEST_F(ExportTreeTest, MessageIsPrintedAfterTextFileIsCreated) {
    testing::internal::CaptureStdout();
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "MessageIsPrintedAfterTextFileIsCreated.txt");
    std::string expectedmsg = "Tree saved in MessageIsPrintedAfterTextFileIsCreated.txt";
    EXPECT_NE(testing::internal::GetCapturedStdout().find(expectedmsg), std::string::npos);
}

TEST_F(ExportTreeTest, MessageIsPrintedAfterDotFileIsCreated) {
    testing::internal::CaptureStdout();
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "MessageIsPrintedAfterDotFileIsCreated.dot");
    std::string expectedmsg = "Tree saved in MessageIsPrintedAfterDotFileIsCreated.dot";
    EXPECT_NE(testing::internal::GetCapturedStdout().find(expectedmsg), std::string::npos);
}

TEST_F(ExportTreeTest, TextConsoleOutputIsTheSameAsTextFile) {
    testing::internal::CaptureStdout();
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, 
    ExportTreeTest::tree_5nodes_4edges_featurenames, ExportTreeTest::tree_5nodes_4edges_classnames, "");
    std::string console = testing::internal::GetCapturedStdout();
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, ExportTreeTest::tree_5nodes_4edges_featurenames,
    ExportTreeTest::tree_5nodes_4edges_classnames, "TextConsoleOutputIsTheSameAsTextFile.txt");
    EXPECT_EQ(console, readFile("TextConsoleOutputIsTheSameAsTextFile.txt"));
}

TEST_F(ExportTreeTest, DotFileHeaderIsCorrect) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "DotFileHeaderIsCorrect.dot");
    std::ifstream ifs("DotFileHeaderIsCorrect.dot", std::ifstream::in);
	std::string line, fileheader;
	int i = 0;
    while (std::getline(ifs, line) && i < 3) {
		fileheader.append(line + "\n");
        i++;
	}
	ifs.close();
    EXPECT_EQ(readFile("../data/dotfileheader.txt"), fileheader);
}

TEST_F(ExportTreeTest, SingleFeatureNodeTreeTextFileOutputIsCorrect) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportText(ExportTreeTest::tree_single_feature_node, fn, cn,
     "SingleFeatureNodeTreeTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/singlefeaturenodetree.txt"),
     readFile("SingleFeatureNodeTreeTextFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, SingleFeatureNodeTreeWithNamesTextFileOutputIsCorrect) {
    std::vector<std::string> fn = {"fftt0", "fftt1", "fftt2", "fftt3", "fftt4",
    "fftt5", "fftt6", "fftt7", "fftt8", "fftt9", "fftt10", "fftt11"};
    ClassNames cn = ExportTreeTest::tree_5nodes_4edges_classnames;
    ExportTree::exportText(ExportTreeTest::tree_single_feature_node, fn, cn,
    "SingleFeatureNodeTreeWithNamesTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/singlefeaturenodetreewithnames.txt"),
     readFile("SingleFeatureNodeTreeWithNamesTextFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, SingleFeatureNodeTreeDotFileOutputIsCorrect) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportDot(ExportTreeTest::tree_single_feature_node, fn, cn,
     "SingleFeatureNodeTreeDotFileOutputIsCorrect.dot");
    EXPECT_EQ(readFile("../data/singlefeaturenodetree.dot"),
     readFile("SingleFeatureNodeTreeDotFileOutputIsCorrect.dot"));
}

TEST_F(ExportTreeTest, SingleFeatureNodeTreeWithNamesDotFileOutputIsCorrect) {
    std::vector<std::string> fn = {"fftt0", "fftt1", "fftt2", "fftt3", "fftt4",
    "fftt5", "fftt6", "fftt7", "fftt8", "fftt9", "fftt10", "fftt11"};
    ClassNames cn = ExportTreeTest::tree_5nodes_4edges_classnames;
    ExportTree::exportDot(ExportTreeTest::tree_single_feature_node, fn, cn,
    "SingleFeatureNodeTreeWithNamesDotFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/singlefeaturenodetreewithnames.dot"),
     readFile("SingleFeatureNodeTreeWithNamesDotFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, SingleLabelNodeTreeTextFileOutputIsCorrect) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportText(ExportTreeTest::tree_single_label_node, fn, cn,
     "SingleLabelNodeTreeTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/singlelabelnodetree.txt"),
     readFile("SingleLabelNodeTreeTextFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, SingleLabelNodeTreeWithNamesTextFileOutputIsCorrect) {
    std::vector<std::string> fn = ExportTreeTest::tree_5nodes_4edges_featurenames;
    ClassNames cn = ExportTreeTest::tree_5nodes_4edges_classnames;
    ExportTree::exportText(ExportTreeTest::tree_single_label_node, fn, cn,
     "SingleLabelNodeTreeWithNamesTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/singlelabelnodetreewithnames.txt"),
     readFile("SingleLabelNodeTreeWithNamesTextFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, SingleLabelNodeTreeDotFileOutputIsCorrect) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportDot(ExportTreeTest::tree_single_label_node, fn, cn,
     "SingleLabelNodeTreeDotFileOutputIsCorrect.dot");
    EXPECT_EQ(readFile("../data/singlelabelnodetree.dot"),
     readFile("SingleLabelNodeTreeDotFileOutputIsCorrect.dot"));
}

TEST_F(ExportTreeTest, SingleLabelNodeTreeWithNamesDotFileOutputIsCorrect) {
    std::vector<std::string> fn = ExportTreeTest::tree_5nodes_4edges_featurenames;
    ClassNames cn = ExportTreeTest::tree_5nodes_4edges_classnames;
    ExportTree::exportDot(ExportTreeTest::tree_single_label_node, fn, cn,
     "SingleLabelNodeTreeWithNamesDotFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/singlelabelnodetreewithnames.dot"),
     readFile("SingleLabelNodeTreeWithNamesDotFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, FiveNodesFourEdgesTreeTextFileOutputIsCorrect) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "FiveNodesFourEdgesTreeTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/fivenodesfouredgestree.txt"),
     readFile("FiveNodesFourEdgesTreeTextFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, FiveNodesFourEdgesTreeWithNamesTextFileOutputIsCorrect) {
    std::vector<std::string> fn = ExportTreeTest::tree_5nodes_4edges_featurenames;
    ClassNames cn = ExportTreeTest::tree_5nodes_4edges_classnames;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "FiveNodesFourEdgesTreeWithNamesTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/fivenodesfouredgestreewithnames.txt"),
     readFile("FiveNodesFourEdgesTreeWithNamesTextFileOutputIsCorrect.txt"));
}


TEST_F(ExportTreeTest, FiveNodesFourEdgesTreeDotFileOutputIsCorrect) {
    std::vector<std::string> fn;
    ClassNames cn;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "FiveNodesFourEdgesTreeDotFileOutputIsCorrect.dot");
    EXPECT_EQ(readFile("../data/fivenodesfouredgestree.dot"),
     readFile("FiveNodesFourEdgesTreeDotFileOutputIsCorrect.dot"));
}

TEST_F(ExportTreeTest, FiveNodesFourEdgesTreeWithNamesDotFileOutputIsCorrect) {
    std::vector<std::string> fn = ExportTreeTest::tree_5nodes_4edges_featurenames;
    ClassNames cn = ExportTreeTest::tree_5nodes_4edges_classnames;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, fn, cn,
     "FiveNodesFourEdgesTreeWithNamesDotFileOutputIsCorrect.dot");
    EXPECT_EQ(readFile("../data/fivenodesfouredgestreewithnames.dot"),
     readFile("FiveNodesFourEdgesTreeWithNamesDotFileOutputIsCorrect.dot"));
}