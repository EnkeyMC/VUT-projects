//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     MARTIN OMACHT <xomach00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author MARTIN OMACHT
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

/// Fixture for testing class BinaryTree
class BinaryTreeTest : public ::testing::Test {
	protected:
		BinaryTree* tree;
		
		BinaryTreeTest() {
			this->tree = new BinaryTree();
		}

		~BinaryTreeTest() {
			delete this->tree;
		}
};

// PUBLIC INTERFACE TESTS

TEST_F(BinaryTreeTest, InsertNodeToEmptyTree) {
	auto retVal = this->tree->InsertNode(1);
	EXPECT_TRUE(retVal.first);
	EXPECT_NE(retVal.second, nullptr);
}

TEST_F(BinaryTreeTest, InsertDuplicateNode) {
	auto firstNode = this->tree->InsertNode(1);
	auto duplicate = this->tree->InsertNode(1);
	
	EXPECT_FALSE(duplicate.first);
	EXPECT_EQ(duplicate.second, firstNode.second);
}

TEST_F(BinaryTreeTest, InsertNodeToNotEmptyTree) {
	this->tree->InsertNode(1);
	auto retVal = this->tree->InsertNode(2);

	EXPECT_TRUE(retVal.first);
	EXPECT_NE(retVal.second, nullptr);
}


TEST_F(BinaryTreeTest, DeleteNodeFromEmptyTree) {
	EXPECT_FALSE(this->tree->DeleteNode(1));
}

TEST_F(BinaryTreeTest, DeleteExistingNode) {
	this->tree->InsertNode(1);
	
	EXPECT_TRUE(this->tree->DeleteNode(1));
}

TEST_F(BinaryTreeTest, DeleteNonExistingNode) {
	this->tree->InsertNode(1);

	EXPECT_FALSE(this->tree->DeleteNode(2));
}


TEST_F(BinaryTreeTest, FindNodeInEmptyTree) {
	EXPECT_EQ(this->tree->FindNode(1), nullptr);
}

TEST_F(BinaryTreeTest, FindExistingNode) {
	auto node = this->tree->InsertNode(1);
	
	EXPECT_EQ(this->tree->FindNode(1), node.second);
} 

TEST_F(BinaryTreeTest, FindNonExistingNode) {
	this->tree->InsertNode(1);

	EXPECT_EQ(this->tree->FindNode(2), nullptr);
}


// AXIOM TESTS

// Fixture class for axiom testing

class BinaryTreeAxiomTest : public ::testing::Test {
	protected:
		BinaryTree* tree;

		BinaryTreeAxiomTest() {
			this->tree = new BinaryTree();

			// Fill the tree with some nodes for testing
			for (int i = 0; i < 15; i++) {
				this->tree->InsertNode(i);
			}
		}

		~BinaryTreeAxiomTest() {
			delete this->tree;
		}
};


TEST_F(BinaryTreeAxiomTest, BlackLeafTest) {
	std::vector<BinaryTree::Node_t *> leafNodes;
	this->tree->GetLeafNodes(leafNodes);

	for(auto leaf : leafNodes) {
		EXPECT_EQ(leaf->color, BinaryTree::BLACK);
	}
}

TEST_F(BinaryTreeAxiomTest, BlackChildrenTest) {
	std::vector<BinaryTree::Node_t *> nodes;
	this->tree->GetAllNodes(nodes);

	for(auto node : nodes) {
		if (node->color == BinaryTree::RED) {
			EXPECT_EQ(node->pLeft->color, BinaryTree::BLACK);
			EXPECT_EQ(node->pRight->color, BinaryTree::BLACK);
		}
	}
}

TEST_F(BinaryTreeAxiomTest, BlackNodesInPathTest) {
	std::vector<BinaryTree::Node_t *> leafNodes;
	this->tree->GetLeafNodes(leafNodes);

	BinaryTree::Node_t* currentNode;
	int blackNodeCount, prevBlackNodeCount = -1;

	for(auto leaf : leafNodes) {
		currentNode = leaf;
		blackNodeCount = 0;

		while (currentNode != nullptr) {
			if (currentNode->color == BinaryTree::BLACK) {
				blackNodeCount++;
			}

			currentNode = currentNode->pParent;
		}

		if (prevBlackNodeCount != -1) {
			ASSERT_EQ(blackNodeCount, prevBlackNodeCount);
		} else {
			prevBlackNodeCount = blackNodeCount;
		}
	}
}

// MAIN

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}



/*** Konec souboru black_box_tests.cpp ***/
