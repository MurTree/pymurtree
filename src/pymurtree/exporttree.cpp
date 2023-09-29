#include "exporttree.h"

void ExportTree::exportText(MurTree::DecisionNode* tree, const std::vector<std::string>& featurenames,
    const std::vector<std::string>& classnames, std::string filepath) {

    if (tree == nullptr) {
        return;
    }

    try {
        if (filepath.empty()) {
            ExportTree tmp(tree, featurenames, classnames, &std::cout, true);
        }
        else {
            std::ofstream ofs(filepath, std::ofstream::out);
            if(!ofs.is_open() || ofs.fail()){
                throw std::runtime_error("Failed to open text output file.");        
            }
	        ExportTree tmp(tree, featurenames, classnames, &ofs, true);
	        ofs.close();
            std::cout << "Tree saved in " << filepath << std::endl;
        }
    }
    catch(std::runtime_error err) {
        std::cout << "Failed to write text output file. Message: "
              << err.what() << std::endl;
    }

}

void ExportTree::exportDot(MurTree::DecisionNode* tree, const std::vector<std::string>& featurenames,
    const std::vector<std::string>& classnames, std::string filepath) {
   
    if (tree == nullptr) {
        return;
    }

    if (filepath.empty()) {
        filepath = "tree.dot";
    }

    try {
        std::ofstream ofs(filepath, std::ofstream::out);
        if (!ofs.is_open() || ofs.fail()) {
            throw std::runtime_error("Failed to open output file.");
        }
        ExportTree tmp(tree, featurenames, classnames, &ofs, false);
        ofs.close(); 
        std::cout << "Tree saved in " << filepath << std::endl;
    }
    catch(std::runtime_error err) {
        std::cout << "Failed to write dot output file. Message: "
              << err.what() << std::endl;
    }
}

ExportTree::ExportTree(MurTree::DecisionNode* tree, const std::vector<std::string>& featurenames,
    const std::vector<std::string>& classnames, std::ostream* os, bool textformat)
    : m_tree(tree), featurenames(featurenames), classnames(classnames), m_os(os), nodecount(0)
{
    if(textformat) {
        // print right-side first
        writeEdgeInTextFormat(tree, true, 0);
        writeEdgeInTextFormat(tree, false, 0);
    }
    else {
        // write file header 
        std::string output = "digraph Tree {\n";
        output.append("node [shape=box, style=\"filled, rounded\", fontname=\"helvetica\", fontsize=\"8\"] ;\n");
        output.append("edge [fontname=\"helvetica\", fontsize=\"6\"] ;\n");
        m_os->write(output.data(), output.size());
        writeNodeInDotFormat(tree, false, -1);
        output = "}";
        m_os->write(output.data(), output.size());

    }
}


void ExportTree::writeEdgeInTextFormat(MurTree::DecisionNode* parentnode, bool rightedge, unsigned int indentationlevel) {

    // iterate over the edges of the tree

    if(parentnode == nullptr) {
        return;
    }

    std::string output = "|---";

    if(parentnode->IsLabelNode()) {
        output.append("class: " + std::to_string(parentnode->label_));
    }
    else {
        if (featurenames.size() >= parentnode->feature_ + 1) {
            output.append(featurenames[parentnode->feature_] + " is ");
        }
        else {
            output.append("feature #" + std::to_string(parentnode->feature_) + " is ");
        }
        rightedge ? output.append("true") : output.append("false");  
    }

    for (int i = 0; i < indentationlevel; i++) {
        output.insert(0, "|   ");
    }
    output.append("\n");

    m_os->write(output.data(), output.size());

    if(parentnode->IsFeatureNode()) {
        if(rightedge) {
            if (parentnode->right_child_ != nullptr) {
                writeEdgeInTextFormat(parentnode->right_child_, true, indentationlevel+1); 
                if(parentnode->right_child_->IsFeatureNode()) {
                    writeEdgeInTextFormat(parentnode->right_child_, false, indentationlevel+1);
                }
            }
        }
        else {
            if (parentnode->left_child_ != nullptr) {
                writeEdgeInTextFormat(parentnode->left_child_, true, indentationlevel+1); 
                if(parentnode->left_child_->IsFeatureNode()) {
                    writeEdgeInTextFormat(parentnode->left_child_, false, indentationlevel+1);
                }
            }
        }   
    }

}

void ExportTree::writeNodeInDotFormat(MurTree::DecisionNode* node, bool rightedge, int parentid) {

    if(node == nullptr) {
        return;
    }

    std::string output = std::to_string(nodecount);

    if(node->IsLabelNode()) {
        output.append(" [label=<class " + std::to_string(node->label_) + ">, color=\"#B77F8C\" fillcolor=\"#B77F8C\"] ;\n");
    }
    else {
        output.append(" [label=<feature #" + std::to_string(node->feature_) + ">, color=\"#8CB77F\", fillcolor=\"#8CB77F\"] ;\n"); 
    }

    if (parentid >= 0) {
        output.append(std::to_string(parentid) + " -> " + std::to_string(nodecount));
        if(rightedge) {
            output.append(" [label=\" 1 \"] ;\n");
        }
        else {
            output.append(" [label=\" 0 \"] ;\n");   
        }
    }

    m_os->write(output.data(), output.size());

    int newparentid = nodecount;
    nodecount++;

    if (node->IsFeatureNode()) {
        writeNodeInDotFormat(node->left_child_, false, newparentid);
        writeNodeInDotFormat(node->right_child_, true, newparentid);
    }
}