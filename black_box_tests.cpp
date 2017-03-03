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
	ASSERT_TRUE(retVal.first);
	ASSERT_NE(retVal.second, nullptr);
}

TEST_F(BinaryTreeTest, InsertDuplicateNode) {
	auto firstNode = this->tree->InsertNode(1);
	auto duplicate = this->tree->InsertNode(1);
	
	EXPECT_FALSE(duplicate.first);
	EXPECT_EQ(duplicate.second, firstNode.second);
}

// AXIOM TESTS

// MAIN

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}



/*** Konec souboru black_box_tests.cpp ***/
