#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TerminalGameEngine2/Core/Transform.hpp"
#include <debugapi.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			Transform tf1;
			Transform tf2;
			Transform tf3 = tf1*tf2;
			OutputDebugString(L"TESTING");
		}

	};
}