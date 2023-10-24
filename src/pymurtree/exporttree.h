#include "solver_result.h"
#include <string>

class ExportTree {

public:

    /*
     * @brief Export the tree structure in text format
     * @param[in] tree      top node of the tree
     * @param[in] filepath  path to output file, std::cout will be used if not specified 
     */
    static void exportText(MurTree::DecisionNode* tree = nullptr, std::string filepath = "");

    /*
     * @brief Export the tree structure in DOT format
     * @param[in] tree      top node of the tree
     * @param[in] filepath  path to output file, "tree.dot" will be used if not specified 
     */
    static void exportDot(MurTree::DecisionNode* tree = nullptr, std::string filepath = "");

private:

    ExportTree(MurTree::DecisionNode* tree, std::ostream* os, bool textformat);

    /*
     * @brief Write the tree structure to m_os
     * @param[in] parentnode    parent node of current edge
     * @param[in] rightedge     true if current edge connects the parent node to its right child node, false otherwise
     * @param[in] indentationlevel      level of indentation
     * @note In MurTree, the right child of a node indicates that the feature is present, the left node indicates the feature is missing
    */
    void writeEdgeInTextFormat(MurTree::DecisionNode* parentnode, bool rightedge, unsigned int indentationlevel = 0);

    /*
     * @brief Write the tree structure to m_os
     * @param[in] node          the current node
     * @param[in] rightedge     true if node is the right child of its parent, false otherwise
     * @param[in] parentid      node id of the parent of the current node
     * @note In MurTree, the right child of a node indicates that the feature is present, the left node indicates the feature is missing
    */
    void writeNodeInDotFormat(MurTree::DecisionNode* node, bool rightedge, int parentid);

    std::ostream* m_os;
    const MurTree::DecisionNode* m_tree;    

    // Utility counter used only when exporting in DOT format
    unsigned int nodecount;


};