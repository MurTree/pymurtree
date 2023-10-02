#include <gtest/gtest.h>
#include "exporttree.h"

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
    }

    MurTree::DecisionNode *c1, *c2, *c3; // label nodes
    MurTree::DecisionNode *f3, *f7; // feature nodes
};

TEST_F(ExportTreeTest, NullTreeTextExportDoesNotThrowException) {
    std::vector<std::string> v;
    EXPECT_NO_THROW(ExportTree::exportText(nullptr, v, v, ""));
}

TEST_F(ExportTreeTest, NullTreeDotExportDoesNotThrowException) {
    std::vector<std::string> v;
    EXPECT_NO_THROW(ExportTree::exportDot(nullptr, v, v, ""));
}

TEST_F(ExportTreeTest, MessagePrintedIfTextFileCreationFails) {
    testing::internal::CaptureStdout();
    std::vector<std::string> v;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, v, v, "/etc/tree.txt");
    std::string expectedmsg = "Failed to write text output file. Message:";
    EXPECT_NE(testing::internal::GetCapturedStdout().find(expectedmsg), std::string::npos);
}

TEST_F(ExportTreeTest, MessagePrintedIfDotFileCreationFails) {
    testing::internal::CaptureStdout();
    std::vector<std::string> v;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, v, v, "/etc/tree.txt");
    std::string expectedmsg = "Failed to write dot output file. Message:";
    EXPECT_NE(testing::internal::GetCapturedStdout().find(expectedmsg), std::string::npos);
}

TEST_F(ExportTreeTest, TextFileIsCreatedAndHasExpectedName) {
    std::vector<std::string> v;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, v, v,
     "TextFileIsCreatedAndHasExpectedName.txt");
    EXPECT_FALSE(readFile("TextFileIsCreatedAndHasExpectedName.txt").empty());
}

TEST_F(ExportTreeTest, DotFileIsCreatedAndHasExpectedName) {
    std::vector<std::string> v;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, v, v,
     "DotFileIsCreatedAndHasExpectedName.txt");
    EXPECT_FALSE(readFile("DotFileIsCreatedAndHasExpectedName.txt").empty());
}

TEST_F(ExportTreeTest, MessageIsPrintedAfterTextFileIsCreated) {
    testing::internal::CaptureStdout();
    std::vector<std::string> v;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, v, v,
     "MessageIsPrintedAfterTextFileIsCreated.txt");
    std::string expectedmsg = "Tree saved in MessageIsPrintedAfterTextFileIsCreated.txt";
    EXPECT_NE(testing::internal::GetCapturedStdout().find(expectedmsg), std::string::npos);
}

TEST_F(ExportTreeTest, MessageIsPrintedAfterDotFileIsCreated) {
    testing::internal::CaptureStdout();
    std::vector<std::string> v;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, v, v,
     "MessageIsPrintedAfterDotFileIsCreated.dot");
    std::string expectedmsg = "Tree saved in MessageIsPrintedAfterDotFileIsCreated.dot";
    EXPECT_NE(testing::internal::GetCapturedStdout().find(expectedmsg), std::string::npos);
}

TEST_F(ExportTreeTest, TextConsoleOutputIsTheSameAsTextFile) {
    testing::internal::CaptureStdout();
    std::vector<std::string> v;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, v, v, "");
    std::string console = testing::internal::GetCapturedStdout();
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, v, v,
     "TextConsoleOutputIsTheSameAsTextFile.txt");
    EXPECT_EQ(console, readFile("TextConsoleOutputIsTheSameAsTextFile.txt"));
}

TEST_F(ExportTreeTest, DotFileHeaderIsCorrect) {
    std::vector<std::string> v;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, v, v,
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
    std::vector<std::string> v;
    ExportTree::exportText(ExportTreeTest::tree_single_feature_node, v, v,
     "SingleFeatureNodeTreeTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/singlefeaturenodetree.txt"),
     readFile("SingleFeatureNodeTreeTextFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, SingleFeatureNodeTreeDotFileOutputIsCorrect) {
    std::vector<std::string> v;
    ExportTree::exportDot(ExportTreeTest::tree_single_feature_node, v, v,
     "SingleFeatureNodeTreeDotFileOutputIsCorrect.dot");
    EXPECT_EQ(readFile("../data/singlefeaturenodetree.dot"),
     readFile("SingleFeatureNodeTreeDotFileOutputIsCorrect.dot"));
}

TEST_F(ExportTreeTest, SingleLabelNodeTreeTextFileOutputIsCorrect) {
    std::vector<std::string> v;
    ExportTree::exportText(ExportTreeTest::tree_single_label_node, v, v,
     "SingleLabelNodeTreeTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/singlelabelnodetree.txt"),
     readFile("SingleLabelNodeTreeTextFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, SingleLabelNodeTreeDotFileOutputIsCorrect) {
    std::vector<std::string> v;
    ExportTree::exportDot(ExportTreeTest::tree_single_label_node, v, v,
     "SingleLabelNodeTreeDotFileOutputIsCorrect.dot");
    EXPECT_EQ(readFile("../data/singlelabelnodetree.dot"),
     readFile("SingleLabelNodeTreeDotFileOutputIsCorrect.dot"));
}

TEST_F(ExportTreeTest, FiveNodesFourEdgesTreeTextFileOutputIsCorrect) {
    std::vector<std::string> v;
    ExportTree::exportText(ExportTreeTest::tree_5nodes_4edges, v, v,
     "FiveNodesFourEdgesTreeTextFileOutputIsCorrect.txt");
    EXPECT_EQ(readFile("../data/fivenodesfouredgestree.txt"),
     readFile("FiveNodesFourEdgesTreeTextFileOutputIsCorrect.txt"));
}

TEST_F(ExportTreeTest, FiveNodesFourEdgesTreeDotFileOutputIsCorrect) {
    std::vector<std::string> v;
    ExportTree::exportDot(ExportTreeTest::tree_5nodes_4edges, v, v,
     "FiveNodesFourEdgesTreeDotFileOutputIsCorrect.dot");
    EXPECT_EQ(readFile("../data/fivenodesfouredgestree.dot"),
     readFile("FiveNodesFourEdgesTreeDotFileOutputIsCorrect.dot"));
}
