#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/road.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(roadTest)
	{
	public:
		TEST_METHOD(costTest)
		{
			jw::road testRoad;

			Assert::IsTrue(testRoad.cost() == 0.0f);
		}
	};
}