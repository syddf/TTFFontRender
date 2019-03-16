#include <stdio.h>
#include <assert.h>
#include <string>
#include <time.h>

const int integer_range = 100;

typedef struct
{
	int numerator;					//分子
	int denominator;				//分母
	bool is_positive;				//是否为正数
}fraction_s;

typedef enum {
	operation_add,				//操作符-加
	operation_sub,					//操作符-减
	operation_mul,				//操作符-乘
	operation_div,					//操作符-除
	operation_left_bracket,	//操作符-左括号
	operation_right_bracket	//操作符-右括号
}operation_e;

char operation_str[6] = { '+' , '-' , '*' , '/' , '(' , ')'};

//表达式
typedef struct {
	int operand_num;				//操作数个数		
	fraction_s * operands;			//操作数序列
	operation_e * operations;	//操作符序列（个数为操作数个数-1)
	int bracket_num;					//括号对数
	int * left_bracket_index;		//所有的左括号下标
	int * right_bracket_index;	//所有的右括号下标
}expression_s;

//-----分数四则计算-----待补充-----//
//用ShowFraction来测试结果
//建议：四则运算全部分开 运算的实现部分只算绝对值 
//但是要在计算之前进行转换 
//比如 -1 加 3 转为 求 3 减 1 
//-1 - 3 转为求 1 + 3 然后改变 is_positive 即可
//在计算完绝对值之后再考虑结果的正负号 这样可能会更加清晰一些
//不过如果能处理好各种情况也可以不用这样做

//返回值：运算结果 参数：f1-被加数 f2：加数
fraction_s FractionAdd(const fraction_s & f1 , const fraction_s & f2)
{

}

//返回值：运算结果 参数：f1-被减数 f2：减数
fraction_s FractionSub(const fraction_s & f1, const fraction_s & f2)
{

}

//返回值：运算结果 参数：f1-乘号左侧的数 f2-乘号右侧的数
fraction_s FractionMul(const fraction_s & f1, const fraction_s & f2)
{

}

//返回值：运算结果 参数：f1-被除数 f2-除数 (注意0的处理)
fraction_s FractionDiv(const fraction_s & f1, const fraction_s & f2)
{

}

//------------------------------------------------------------------------------------

bool FractionEqual(const fraction_s & f1, const fraction_s & f2)
{
	return (f1.numerator == 0 && f2.numerator == 0) || 
		(f1.numerator == f2.numerator && f1.denominator == f2.denominator);
}

//分数的约分

int GetGCD(int a, int b)
{
	if (a == 0 || b == 0 ) return 1;
	if (a < b)
	{
		int c = a;
		a = b;
		b = c;
	}
	int remain;
	do {
		remain = a % b;
		a = b;
		b = remain;
	} while (remain != 0);
	return a;
}

void FractionReduction(fraction_s & f)
{
	int gcd = GetGCD(f.numerator, f.denominator);
	f.numerator /= gcd;
	f.denominator /= gcd;
}

//输出分数
void ShowFraction(const fraction_s & f)
{
	if (!f.is_positive) printf("-");
	printf("%d", f.numerator);
	if (f.denominator == 1)  return;
	printf("/%d", f.denominator);
}

//创建一个算术表达式

expression_s CreateExpression(int operand_num , bool need_bracket , bool is_only_integer)
{
	expression_s new_expression;
	new_expression.operand_num = operand_num;
	new_expression.operands = (fraction_s*)malloc(sizeof(fraction_s) * operand_num);
	new_expression.operations = (operation_e*)malloc(sizeof(operation_e) * (operand_num - 1));

	//随机生成操作数
	for (int i = 0; i < operand_num; i++)
	{
		new_expression.operands[i].numerator = rand() % integer_range;
		if (is_only_integer)
		{
			new_expression.operands[i].denominator = 1;
		}
		else {
			int r;
			do {
				r = rand() % integer_range;
			} while (r == 0); //确保分母不为0
			new_expression.operands[i].denominator = r;
		}
		FractionReduction(new_expression.operands[i]);
	}

	//生成操作符
	for (int i = 0; i < operand_num - 1; i++)
	{
		int r = rand() % 4;
		if (new_expression.operands[i].numerator == 0) r = rand() % 3; //避免除以0
		new_expression.operations[i] = (operation_e)r;
	}

	//生成括号
	if (need_bracket)
	{
		new_expression.bracket_num = operand_num >> 2;
		new_expression.left_bracket_index = (int*)malloc(sizeof(int) * new_expression.bracket_num);
		new_expression.right_bracket_index = (int*)malloc(sizeof(int) * new_expression.bracket_num);

		for (int i = 0; i < new_expression.bracket_num; i++)
		{
			int l_b, r_b;
			bool has_same_bracket;
			do {
				has_same_bracket = false;
				l_b = rand() % (new_expression.operand_num - 2);
				int r_b_range = new_expression.operand_num - l_b - 1;
				if (r_b_range == 0)
				{
					r_b = new_expression.operand_num - 2;
				}
				else {
					r_b = l_b + 1 + rand() % r_b_range;
				}
				if (l_b == r_b)
				{
					continue;
				}
				for (int j = 0; j < i; j++)
				{
					if (new_expression.left_bracket_index[j] == l_b &&
						new_expression.right_bracket_index[j] == r_b)
					{
						has_same_bracket = true;
					}
				}
			} while (has_same_bracket );

			new_expression.left_bracket_index[i] = l_b;
			new_expression.right_bracket_index[i] = r_b;

		}
	}

	return new_expression;
}

//输出算术表达式
void ShowExpression(const expression_s & s)
{
	for (int i = 0; i < s.operand_num; i++)
	{
		//输出可能存在的左括号
		for (int j = 0; j < s.bracket_num; j++)
		{
			if (s.left_bracket_index[j] == i) printf("(");			
		}

		//输出操作数
		ShowFraction(s.operands[i]);

		//输出可能存在的右括号
		for (int j = 0; j < s.bracket_num; j++)
		{
			if (s.right_bracket_index[j] == i) printf(")");
		}

		//输出操作符
		if (i < s.operand_num - 1)
		{
			if (s.operations[i] == operation_div)
			{
				char * s = "÷";
				printf("%c%c", s[0] , s[1]);
			}
			else printf("%c", operation_str[s.operations[i]]);
		}

	}

	printf("\n");
}

//------- 表达式求值 -------  待补充//

//返回值：最终求得结果
//用ShowFraction来测试结果
//按照《数据结构》表达式求值的方法 判定某个操作数之前和之后是否有括号 需要遍历所有的括号的下标 可以参考 ShowExpression()
//（操作符优先级暂未定义 待补充)
fraction_s GetExpressionVal(const expression_s & s)
{

}


//------------------------------------------------------------------------------------

int main()
{
	srand(time(0));
	/*
	创建并显示一个表达式的过程 用这个来进行测试
	expression_s s = CreateExpression(10 , true , false);
	ShowExpression(s);
	*/
	return 0;
}