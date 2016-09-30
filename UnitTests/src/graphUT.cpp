#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(graph)
	{
	public:
		TEST_METHOD(at)
		{
			jw::graph<std::string, char> testGraph;

			jw::graph<std::string, char>::value_type expectedPair = { "one", {{ 2, 'a' }} };

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");

			testGraph.insertEdge(1, 2, 'a', true);	// bidirectional

			Assert::IsTrue(testGraph.at(1) == expectedPair);
		}

		TEST_METHOD(insertGetNode)
		{
			jw::graph<std::string, char> testGraph;

			testGraph.insertNode(1, "one");

			Assert::IsTrue(testGraph.nodeAt(1) == "one");
		}

		TEST_METHOD(insertGetEdges)
		{
			jw::graph<std::string, char> testGraph;

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");

			testGraph.insertEdge(1, 2, 'a', true);	// bidirectional

			Assert::IsTrue(testGraph.edgesAt(1).at(2) == 'a');
			Assert::IsTrue(testGraph.edgesAt(2).at(1) == 'a');

			Assert::Fail();	// TODO edgeBetween test
		}

		TEST_METHOD(sizeAndEmpty)
		{
			jw::graph<std::string, char> testGraph;

			Assert::IsTrue(testGraph.empty());

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");
			testGraph.insertNode(3, "three");

			Assert::IsTrue(testGraph.size() == 3);
			Assert::IsFalse(testGraph.empty());
		}

		TEST_METHOD(begin)
		{
			jw::graph<std::string, char> testGraph;

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");
			testGraph.insertEdge(1, 2, 'a', true);	// bidirectional

			jw::graph<std::string, char>::container_type::value_type expectedPair1 = { 1, { "one", {{ 2, 'a' }} } };
			jw::graph<std::string, char>::container_type::value_type expectedPair2 = { 2, { "two", {{ 1, 'a' }} } };

			jw::graph<std::string, char>::iterator testIterator = testGraph.begin();
			jw::graph<std::string, char>::const_iterator testConstIterator = testGraph.begin();

			Assert::IsTrue(*testIterator == expectedPair1);
			Assert::IsTrue(*++testIterator == expectedPair2);

			Assert::IsTrue(*testConstIterator == expectedPair1);
			Assert::IsTrue(*++testConstIterator == expectedPair2);
		}

		TEST_METHOD(end)
		{
			jw::graph<std::string, char> testGraph;

			testGraph.insertNode(1, "one");
			testGraph.insertNode(2, "two");
			testGraph.insertEdge(1, 2, 'a', true);	// bidirectional

			jw::graph<std::string, char>::container_type::value_type expectedPair1 = { 1,{ "one",{ { 2, 'a' } } } };
			jw::graph<std::string, char>::container_type::value_type expectedPair2 = { 2,{ "two",{ { 1, 'a' } } } };

			jw::graph<std::string, char>::iterator testIterator = testGraph.end();
			jw::graph<std::string, char>::const_iterator testConstIterator = testGraph.end();

			Assert::IsTrue(*--testIterator == expectedPair2);
			Assert::IsTrue(*--testIterator == expectedPair1);

			Assert::IsTrue(*--testConstIterator == expectedPair2);
			Assert::IsTrue(*--testConstIterator == expectedPair1);
		}
	};
}