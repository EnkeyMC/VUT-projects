//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     MARTIN OMACHT <xomach00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author MARTIN OMACHT
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//


TEST(MatrixTest, InvalidInstantiation) {
	EXPECT_ANY_THROW(new Matrix(0,0));
}

class MatrixUnaryTest : public ::testing::Test {
	protected:
		Matrix* matrix;

		~MatrixUnaryTest() {
			delete matrix;
		}
};

TEST_F(MatrixUnaryTest, SetSingleValue) {
	this->matrix = new Matrix();

	EXPECT_TRUE(this->matrix->set(0,0,5.0));
}

TEST_F(MatrixUnaryTest, SetSingleValueInvalid) {
	this->matrix = new Matrix();

	EXPECT_FALSE(this->matrix->set(1,0,5.0));
}

TEST_F(MatrixUnaryTest, SetValuesFromVector) {
	this->matrix = new Matrix(3,3);
	std::vector<std::vector< double >> values{
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};

	EXPECT_TRUE(this->matrix->set(values));
}

TEST_F(MatrixUnaryTest, SetValuesFromVectorInvalid) {
	this->matrix = new Matrix(3,3);
	std::vector<std::vector< double >> values {
		{1,2},
		{3,4}
	};

	EXPECT_FALSE(this->matrix->set(values));
}

TEST_F(MatrixUnaryTest, GetValueFromMatrix) {
	this->matrix = new Matrix();

	EXPECT_EQ(this->matrix->get(0,0), 0.0);
}

TEST_F(MatrixUnaryTest, GetValueFromMatrixInvalid) {
	this->matrix = new Matrix();
	double retVal = this->matrix->get(1,1);

	EXPECT_FALSE(retVal == retVal); // NaN is not equal to NaN
}

TEST_F(MatrixUnaryTest, SolveEquation2x2) {
	this->matrix = new Matrix(2,2);
	std::vector<std::vector<double>> values {
		{4,2},
		{5,-3}
	};
	std::vector<double> b {6,13};
	std::vector<double> expectedResult {2,-1};

	this->matrix->set(values);

	auto result = this->matrix->solveEquation(b);

	ASSERT_EQ(result.size(), expectedResult.size());

	for (int i = 0; i < result.size(); i++) {
		ASSERT_EQ(result[i], expectedResult[i]);
	}
}

TEST_F(MatrixUnaryTest, SolveEquationInvalidB) {
	this->matrix = new Matrix();
	std::vector<double> b {2,2};

	EXPECT_ANY_THROW(this->matrix->solveEquation(b));
}

TEST_F(MatrixUnaryTest, SolveEquationNotSquare) {
	this->matrix = new Matrix(2,3);
	std::vector<double> b {1,1,1};

	EXPECT_ANY_THROW(this->matrix->solveEquation(b));
}

TEST_F(MatrixUnaryTest, SolveEquationSingular1x1) {
	this->matrix = new Matrix();
	std::vector<double> b {0};

	EXPECT_ANY_THROW(this->matrix->solveEquation(b));
}

TEST_F(MatrixUnaryTest, SolveEquationSingular3x3) {
	this->matrix = new Matrix(3,3);
	this->matrix->set(std::vector<std::vector<double>>(3, std::vector<double>(3, 1)));

	EXPECT_ANY_THROW(this->matrix->solveEquation(std::vector<double>(3,0)));
}

TEST_F(MatrixUnaryTest, SolveEquation4x4) {
	this->matrix = new Matrix(4,4);
	this->matrix->set({
		{4,2,0,-1},
		{8,-3,1,-4},
		{1,-1,-1,-1},
		{1,1,1,1}
	});

	std::vector<double> b {4,-11,-8,10};
	std::vector<double> expectedResult {1,2,3,4};

	auto result = this->matrix->solveEquation(b);

	ASSERT_EQ(result.size(), expectedResult.size());
	for (int i = 0; i < result.size(); ++i)
	{
		ASSERT_EQ(result[i], expectedResult[i]);
	}
}

// Binary operands test

class MatrixBinaryTest : public ::testing::Test {
	protected:
		Matrix* matrix1;
		Matrix* matrix2;

		~MatrixBinaryTest () {
			delete matrix1;
			delete matrix2;
		}
};

TEST_F(MatrixBinaryTest, OperatorEqualTest) {
	this->matrix1 = new Matrix(2,2);
	this->matrix2 = new Matrix(2,2);

	EXPECT_EQ(*this->matrix1, *this->matrix2);
}

TEST_F(MatrixBinaryTest, OperatorEqualTestFalse) {
	this->matrix1 = new Matrix (2,2);
	this->matrix2 = new Matrix(2,2);
	this->matrix2->set(1,1, 2.0);

	EXPECT_FALSE(*this->matrix1 == *this->matrix2);
}

TEST_F(MatrixBinaryTest, OperatorEqualTestInvalid) {
	this->matrix1 = new Matrix(5,5);
	this->matrix2 = new Matrix(2,2);

	EXPECT_ANY_THROW(*this->matrix1 == *matrix2);
}

TEST_F(MatrixBinaryTest, OperatorPlusTest) {
	this->matrix1 = new Matrix(3,3);
	this->matrix2 = new Matrix(3,3);
	Matrix* expectedResultMatrix = new Matrix(3,3);

	std::vector<std::vector< double >> matrix1vals {
		{ 0,  1, 5},
		{ 8,  5,23},
		{47,154, 2}
	};

	std::vector<std::vector< double >> matrix2vals {
		{ 5,  4,  3},
		{10, 20, 30},
		{ 7,-54,-12}
	};

	std::vector<std::vector< double >> resultVals {
		{ 5,  5,  8},
		{18, 25, 53},
		{54,100,-10}
	};

	this->matrix1->set(matrix1vals);
	this->matrix2->set(matrix2vals);
	expectedResultMatrix->set(resultVals);

	EXPECT_EQ(*this->matrix1 + *this->matrix2, *expectedResultMatrix);

	delete expectedResultMatrix;
}

TEST_F(MatrixBinaryTest, OperatorPlusTestInvalid) {
	this->matrix1 = new Matrix();
	this->matrix2 = new Matrix(2,2);

	EXPECT_ANY_THROW(*this->matrix1 + *this->matrix2);
}

TEST_F(MatrixBinaryTest, OperatorMultiplyTest) {
	this->matrix1 = new Matrix(2,2);
	this->matrix2 = new Matrix(2,2);
	Matrix* expectedResultMatrix = new Matrix(2,2);

	std::vector<std::vector< double >> matrix1vals {
		{1,2},
		{3,4}
	};

	std::vector<std::vector< double >> matrix2vals {
		{5,6},
		{7,8}
	};

	std::vector<std::vector< double >> resultVals {
		{19,22},
		{43,50}
	};

	this->matrix1->set(matrix1vals);
	this->matrix2->set(matrix2vals);
	expectedResultMatrix->set(resultVals);

	EXPECT_EQ(*this->matrix1 * *this->matrix2, *expectedResultMatrix);

	delete expectedResultMatrix;
}

TEST_F(MatrixBinaryTest, OperatorMultiplyTestInvalid) {
	this->matrix1 = new Matrix();
	this->matrix2 = new Matrix(2,2);

	EXPECT_ANY_THROW(*this->matrix1 * *this->matrix2);
}

TEST_F(MatrixBinaryTest, OperatorMultiplyByDblTest) {
	this->matrix1 = new Matrix(2,2);
	this->matrix2 = new Matrix(2,2); // expected result

	std::vector<std::vector<double>> values{
		{5,3},
		{4,2}
	};

	std::vector<std::vector<double>> result{
		{10,6},
		{8,4}
	};	

	this->matrix1->set(values);
	this->matrix2->set(result);

	EXPECT_EQ(*this->matrix1 * 2.0, *this->matrix2);
}

// MAIN

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

/*** Konec souboru white_box_tests.cpp ***/
