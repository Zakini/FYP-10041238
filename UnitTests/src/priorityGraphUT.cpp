#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/priorityGraph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(priorityGraph)
	{
	public:
		TEST_METHOD(at)
		{
			jw::priorityGraph<std::string, char> testGraph;

			jw::priorityGraph<std::string, char>::value_type expectedPair = { "one", { { { 1, 2 }, 'a' }, { { 5, 2 }, 'b' } } };

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");

			testGraph.insertEdge(1, 2, 'a');	// auto priority
			testGraph.insertEdge(1, 2, 'b', 5);	// forced priority

			Assert::IsTrue(testGraph.at(1) == expectedPair);
		}

		TEST_METHOD(insertGetNode)
		{
			jw::priorityGraph<std::string, char> testGraph;

			testGraph.insertNode(1, "one");

			Assert::IsTrue(testGraph.nodeAt(1) == "one");
		}

		TEST_METHOD(insertGetEdges)
		{
			jw::priorityGraph<std::string, char> testGraph;

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");

			testGraph.insertEdge(1, 2, 'a');			// auto priority
			testGraph.insertEdge(1, 2, 'b', 5);			// forced priority
			testGraph.insertEdge(1, 2, 'c', true);		// auto priority, bidirectional
			testGraph.insertEdge(1, 2, 'd', 2, true);	// forced priority, bidirectional

			Assert::IsTrue(testGraph.edgeBetween(1, 2) == 'a');		// highest priority
			Assert::IsTrue(testGraph.edgeBetween(1, 2, 5) == 'b');
			Assert::IsTrue(testGraph.edgeBetween(1, 2, 6) == 'c');
			Assert::IsTrue(testGraph.edgeBetween(2, 1, 6) == 'c');
			Assert::IsTrue(testGraph.edgeBetween(1, 2, 2) == 'd');
			Assert::IsTrue(testGraph.edgeBetween(2, 1, 2) == 'd');

			jw::priorityGraph<std::string, char>::edge_container_type expectedEdges1 = { {{1, 2}, 'a'}, {{2, 2}, 'd'}, {{5, 2}, 'b'}, {{6, 2}, 'c'} };
			jw::priorityGraph<std::string, char>::edge_container_type expectedEdges2 = { {{2, 1}, 'd'}, {{6, 1}, 'c'} };
			Assert::IsTrue(testGraph.edgesAt(1) == expectedEdges1);
			Assert::IsTrue(testGraph.edgesAt(2) == expectedEdges2);
		}

		TEST_METHOD(sizeAndEmpty)
		{
			jw::priorityGraph<std::string, char> testGraph;

			Assert::IsTrue(testGraph.empty());

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");
			testGraph.insertNode(3, "three");

			Assert::IsTrue(testGraph.size() == 3);
			Assert::IsFalse(testGraph.empty());
		}

		TEST_METHOD(begin)
		{
			jw::priorityGraph<std::string, char> testGraph;

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");
			testGraph.insertEdge(1, 2, 'a', true);	// bidirectional

			jw::priorityGraph<std::string, char>::container_type::value_type expectedPair1 = { 1,{ "one",{ { {1, 2}, 'a' } } } };
			jw::priorityGraph<std::string, char>::container_type::value_type expectedPair2 = { 2,{ "two",{ { {1, 1}, 'a' } } } };

			jw::priorityGraph<std::string, char>::iterator testIterator = testGraph.begin();
			jw::priorityGraph<std::string, char>::const_iterator testConstIterator = testGraph.begin();

			Assert::IsTrue(*testIterator == expectedPair1);
			Assert::IsTrue(*++testIterator == expectedPair2);

			Assert::IsTrue(*testConstIterator == expectedPair1);
			Assert::IsTrue(*++testConstIterator == expectedPair2);
		}

		TEST_METHOD(end)
		{
			jw::priorityGraph<std::string, char> testGraph;

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");
			testGraph.insertEdge(1, 2, 'a', true);	// bidirectional

			jw::priorityGraph<std::string, char>::container_type::value_type expectedPair1 = { 1,{ "one",{ { {1, 2}, 'a' } } } };
			jw::priorityGraph<std::string, char>::container_type::value_type expectedPair2 = { 2,{ "two",{ { {1, 1}, 'a' } } } };

			jw::priorityGraph<std::string, char>::iterator testIterator = testGraph.end();
			jw::priorityGraph<std::string, char>::const_iterator testConstIterator = testGraph.end();

			Assert::IsTrue(*--testIterator == expectedPair2);
			Assert::IsTrue(*--testIterator == expectedPair1);

			Assert::IsTrue(*--testConstIterator == expectedPair2);
			Assert::IsTrue(*--testConstIterator == expectedPair1);
		}
	};
}