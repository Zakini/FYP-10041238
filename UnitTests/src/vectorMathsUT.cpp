#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/vectorMaths.h"
#include <SFML/System/Vector2.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(vectorMaths)
	{
	public:
		TEST_METHOD(length)
		{
			sf::Vector2f testVector(3, 4);
			Assert::IsTrue(jw::length(testVector) == 5.0f);
		}
	};
}