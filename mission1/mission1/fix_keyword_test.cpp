// ������ TC�� ���������� ���� �� �ȿͿ� ��.��
#if 0
#include "gmock/gmock.h"
#include <vector>
#include <string>
#include "fix_keyword.cpp"

using namespace testing;
using std::vector;
using std::string;

TEST(KeyWord, TC1) {
	//vector<string> strs = { "water", "monday" };
	SearchKeywordAndDayoftheweek strs = { "water", "monday" };

	//PerfectSearchKeywordCorrectionService app;
	//string actual = app.input(strs);
	//EXPECT_EQ(strs.keyword, actual);
}
#endif