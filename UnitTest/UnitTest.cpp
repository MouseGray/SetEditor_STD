#include "pch.h"
#include "CppUnitTest.h"
#include "../SetEditor V1.0.0 (STD)/Transforms.h"
#include "../SetEditor V1.0.0 (STD)/TermTool.h"
#include "../SetEditor V1.0.0 (STD)/DATerm.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			//TermTool::CreateTerm("{T}#{M}#{K}#(T#M$T#K$K#M)");
			//TermTool::CreateTerm("{T}#{M}#{K}#(T#(M$K)$K#M)");
			Assert::AreEqual(true, T___IntersectParantheses(TermTool::CreateTerm("(T#M$T#K$K#M)"), TermTool::CreateTerm("(T#(M$K)$K#M)")));
		}
	};
}
