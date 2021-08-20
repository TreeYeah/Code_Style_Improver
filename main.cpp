/******************* Code Style Improver *******************
 *
 * Author: @TreeYeah
 * My Bilibili Account: https://space.bilibili.com/479508690
 * My GitHub Account:   https://github.com/TreeYeah
*/

#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

using namespace std;

short parenthesis;				//小括号
short keyword;					// 2:if  3:for  4:else  5:while  6:switch
short brace; 					//大括号
bool inDefinition;				//是否在#define 后面
bool inDoubleQuotationMarks;    //双引号
bool inSingleQuotationMarks;    //单引号
bool inIncluding;				//是否在#include 后面
bool inSingleLineComments;		//是否在单行注释后面
int inMultiLineComments;		//是否在多行注释内
bool afterKeyword = false;		//是否在关键字后面或
bool lastLineKeyword = false;	//上一行是否有关键字

int lineI;

bool avoidTwoConsecutiveBlankLines = true;		//避免两个连续的空行
bool spacesOnBothSidesOfSelfOperators = true;	//+=,-=,*=,/=,%= 两侧是否有空格
bool spacesOnBothSidesOfArithmeticOperators = true;
bool spacesOnBothSidesOfAssignmentOperators = true;
bool spaceAfterSemicolon = true;
bool spaceAfterComma = true;
bool spaceAfterKeyword = true;
int atMostOneSentenceOnALine = 2;		//0:允许一行多个语句 1:仅允许关键字后同行一个语句 2:仅允许一行一个语句,关键字括号内算一个语句
bool theFirstBracesNeverWrap = false;	//关键字后的'{'不换行
bool overwriteOriginalFile = false;
bool strictIndent = false;				//是否使用严格的缩进

vector<string> codeResult;

void color(int a)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

bool inComments()
{
    return (inSingleLineComments || inMultiLineComments);
}

string clearTheSpaceAtTheFront(string s)	//清除字符串前部的空格和Tab
{
    while(s[0] == ' ' || s[0] == 9)
        s.erase(0,1);
    return s;
}

char getTheNthCharacterOf(string s, int n)	//获取字符串的第N个非空字符
{
    int i = 0;
    while(i < s.length())
    {
        if(s[i] != ' ' && s[i] != 9 && s[i] != '\n')
        {
            if(--n == 0)
            {
                return s[i];
            }
        }
        i++;
    }
    return ' ';
}

char getTheNthCharacterAfter(string s, int pos, int n)	//获取s[pos]后的第N个非空字符
{
    int i = pos;
    if(n > 0)
    {
        i++;
        while(i < s.length())
        {
            if(s[i] != ' ' && s[i] != 9 && s[i] != '\n')
            {
                if(--n == 0)
                {
                    return s[i];
                }
            }
            i++;
        }
        return ' ';
    }
    if(n < 0)
    {
        i--;
        while(i >= 0)
        {
            if(s[i] != ' ' && s[i] != 9 && s[i] != '\n')
            {
                if(++n == 0)
                {
                    return s[i];
                }
            }
            i--;
        }
        return ' ';
    }
    return s[i];
}

bool isLetterOrNumber(char c) //c ( is letter, number, _, (, ), ', or " ) : return true ; else: return false.
{
    return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9'||
            c == '_' || c == '\'' || c == '\"' || c == '('|| c == ')');
}

void go(float x,float y)	//光标移动函数
{
	COORD coord;
	coord.X = x * 2;
	coord.Y = y + 2;
	HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(a,coord);
}

bool readSettings()
{
	ifstream infile("Settings.dat");
	if(!infile.is_open())
	{
		ofstream outfile("Settings.dat");
		outfile << avoidTwoConsecutiveBlankLines << ' '
				<< spacesOnBothSidesOfSelfOperators << ' '
				<< spacesOnBothSidesOfArithmeticOperators << ' '
				<< spacesOnBothSidesOfAssignmentOperators << ' '
				<< spaceAfterSemicolon << ' '
				<< spaceAfterComma << ' '
				<< spaceAfterKeyword << ' '
				<< atMostOneSentenceOnALine << ' '
				<< theFirstBracesNeverWrap << ' '
				<< strictIndent << ' '
				<< overwriteOriginalFile << '\n';
		return false;
	}

	infile >> avoidTwoConsecutiveBlankLines
		   >> spacesOnBothSidesOfSelfOperators
		   >> spacesOnBothSidesOfArithmeticOperators
		   >> spacesOnBothSidesOfAssignmentOperators
		   >> spaceAfterSemicolon
		   >> spaceAfterComma
		   >> spaceAfterKeyword
		   >> atMostOneSentenceOnALine
		   >> theFirstBracesNeverWrap
		   >> strictIndent
		   >> overwriteOriginalFile;

	return true;
}

bool saveSettings()
{
	ofstream outfile("Settings.dat");

	if(!outfile.is_open())
		return false;

	outfile << avoidTwoConsecutiveBlankLines << ' '
			<< spacesOnBothSidesOfSelfOperators << ' '
			<< spacesOnBothSidesOfArithmeticOperators << ' '
			<< spacesOnBothSidesOfAssignmentOperators << ' '
			<< spaceAfterSemicolon << ' '
			<< spaceAfterComma << ' '
			<< spaceAfterKeyword << ' '
			<< atMostOneSentenceOnALine << ' '
			<< theFirstBracesNeverWrap << ' '
			<< strictIndent << ' '
			<< overwriteOriginalFile << '\n';
	return true;
}

void printSetting(string s)
{
	int c[2] = {112,240}, ci = 0;

	color(c[ci]);
	printf("        ");

    for(int i = 0; i < s.length(); i++)
    {
    	if(s[i] == '`')
    	{
			ci = 1 - ci;
			color(c[ci]);
    	}
    	else
    	{
    		printf("%c",s[i]);
    	}
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    system("color 70");

    /** 隐藏光标 **/
    	CONSOLE_CURSOR_INFO cursor_info={1,0};
    	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

    /** 设置字体大小 **/
        HANDLE hcsb = CreateFileA("CONOUT$", GENERIC_WRITE | GENERIC_READ,
                                  FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        CONSOLE_FONT_INFOEX cfi = {sizeof(cfi)};

        GetCurrentConsoleFontEx(hcsb, FALSE, &cfi);

        cfi.dwFontSize.X = 18;
        cfi.dwFontSize.Y = 18;

        SetCurrentConsoleFontEx(hcsb, FALSE, &cfi);

    readSettings();

    if(argc > 1)	//如果有文件被使用该程序打开
    {
    	SetConsoleTitle("码风自动改善工具");
    	for(int i = 1; i < argc; i++)
        {
            ifstream fin(argv[i]);
            if(!fin.is_open())
            {
                printf("文件%s打开出错！\n",argv[i]);
                continue;
            }

            string s;
            int ci;
            int improvementN = 0;

            while(getline(fin,s))
            {
                codeResult.emplace_back("");
                for(ci = 0; ci < s.length(); ci++)
                {
                    if (s[ci] == '\"' && (ci == 0 || s[ci-1] != '\\' && !inSingleQuotationMarks) && !inComments())
                        inDoubleQuotationMarks = 1 - inDoubleQuotationMarks;

                    if (s[ci] == '\'' && (ci == 0 || s[ci-1] != '\\' && !inDoubleQuotationMarks) && !inComments())
                        inSingleQuotationMarks = 1 - inSingleQuotationMarks;

                    if (s[ci] == '/' && ci < s.length()-1 && s[ci+1] == '/' && !inDoubleQuotationMarks)
                        inSingleLineComments = true;

                    if (s[ci] == '/' && ci < s.length()-1 && s[ci+1] == '*' && !inSingleLineComments)
                        inMultiLineComments++;

                    if (s[ci] == '*' && ci < s.length()-1 && s[ci+1] == '/')
                        inMultiLineComments--;

                    if (s[ci] != ' ' && s[ci] != 9 || getTheNthCharacterAfter(s,ci,1) != ' '
                    	&& (strictIndent == false || !codeResult[lineI].empty() || lastLineKeyword))
                    {
                        if(s[ci] != 9)
                        {
                            codeResult[lineI].push_back(s[ci]);
                        }
                        else
                        {
                        	improvementN--;
                            codeResult[lineI].push_back(' ');
                            codeResult[lineI].push_back(' ');
                            codeResult[lineI].push_back(' ');
                            codeResult[lineI].push_back(' ');
                        }
                    }
                }
                codeResult[lineI].push_back('\n');
                lineI++;
                inSingleQuotationMarks = false;
                inDoubleQuotationMarks = false;
                inSingleLineComments = false;
            }

            lineI--;

            inDoubleQuotationMarks = inSingleQuotationMarks = inSingleLineComments = false;
            inMultiLineComments = 0;

            go(0,i-2);
            printf("Progress: Line");
            for(int k = 0; k < codeResult.size(); k++)	//逐行处理
            {
                if(getTheNthCharacterOf(codeResult[k],1) == '#')
                {
                    inIncluding = true;
                    if (codeResult[k].find("include") != string::npos && (codeResult[k][codeResult[k].find("include")-1] == '#' || codeResult[k][codeResult[k].find("include")-1] == ' ') && codeResult[k][codeResult[k].find("include") + 7] != ' ')
                    {
                        codeResult[k].insert(codeResult[k].find("include") + 7, " ");
                        improvementN++;
                        if(codeResult[k][codeResult[k].find("include")-1] == ' ')
                        {
                        	codeResult[k].erase(codeResult[k].find("include")-1);
                        	improvementN++;
                        }
                    }
                }

                int spaceN = 0, spaceI = 0;
                while(spaceI < codeResult[k].length() && (codeResult[k][spaceI] == ' ' || codeResult[k][spaceI] == 9))
                {
                    if(codeResult[k][spaceI] == ' ')
                        spaceN++;
                    else
                        spaceN += 4;
                    spaceI++;
                }
                if(getTheNthCharacterOf(codeResult[k],1) != '}' && spaceN < brace * 4)
                {
                    for(int l = spaceN + 1; l <= brace * 4; l++)
                        codeResult[k].insert(0," ");
                }
                else if(spaceN < brace * 4 - 4)
                {
                    for(int l = spaceN + 1; l <= brace * 4 - 4; l++)
                        codeResult[k].insert(0," ");
                }

                if(!inIncluding)
                {
                	for(int l = 0; k < codeResult.size() && l < codeResult[k].length(); l++)	//逐字符处理
                    {
                        if(!inComments() && !inDoubleQuotationMarks && (l == 0 || codeResult[k][l-1] == ' ' || codeResult[k][l-1] == 9))
                        {
                            if(codeResult[k].find("if") == l
                               && (codeResult[k][l+2] == ' ' || codeResult[k][l+2] == 9 || codeResult[k][l+2] == '('))
                            {
                                keyword = 2;
                                if(codeResult[k][l+keyword] == '(' && spaceAfterKeyword)
                                    codeResult[k].insert(l+keyword," "), improvementN++;
                            }

                            if(codeResult[k].find("for") == l
                               && (codeResult[k][l+3] == ' ' || codeResult[k][l+3] == 9 || codeResult[k][l+3] == '('))
                            {
                                keyword = 3;
                                if(codeResult[k][l+keyword] == '(' && spaceAfterKeyword)
                                	codeResult[k].insert(l+keyword," "), improvementN++;
                            }

                            if(codeResult[k].find("else") == l
                               && (codeResult[k][l+4] == ' ' || codeResult[k][l+4] == 9))
                            {
                                keyword = 4;
                                if (getTheNthCharacterAfter(codeResult[k],l+3,1) != 'i'
                                	&& getTheNthCharacterAfter(codeResult[k],l+3,2) != 'f'
                                	&& getTheNthCharacterAfter(codeResult[k],l+3,3) != '('
                                	&& getTheNthCharacterAfter(codeResult[k],l+3,1) != '/'
                                	&& getTheNthCharacterAfter(codeResult[k],l+3,1) != ' '
                                	&& atMostOneSentenceOnALine == 2)
                                {
                                	improvementN++;
                                	codeResult.insert(codeResult.begin()+k+1, codeResult[k].substr(l+4,codeResult[k].length()-l-4));
                                	codeResult[k+1] = clearTheSpaceAtTheFront(codeResult[k+1]);
                                	codeResult[k].erase(l+4,codeResult[k].length()-l-4);
                                	codeResult[k].push_back('\n');
                                	for(int p = 0; p < codeResult[k].length() && (codeResult[k][p] == ' ' || codeResult[k][p] == 9); p++)
                                		codeResult[k+1].insert(codeResult[k+1].begin(),codeResult[k][p]);
                                	codeResult[k+1].insert(codeResult[k+1].begin(),9);

                                	keyword = 0;
                                }
                            }

                            if(codeResult[k].find("while") == l
                               && (codeResult[k][l+5] == ' ' || codeResult[k][l+5] == 9 || codeResult[k][l+5] == '('))
                            {
                                keyword = 5;
                                if(codeResult[k][l+keyword] == '(' && spaceAfterKeyword)
                                	codeResult[k].insert(l+keyword," "), improvementN++;
                            }

                            if(codeResult[k].find("switch") == l
                               && (codeResult[k][l+6] == ' ' || codeResult[k][l+6] == 9 || codeResult[k][l+6] == '('))
                            {
                                keyword = 6;
                                if(codeResult[k][l+keyword] == '(' && spaceAfterKeyword)
                                	codeResult[k].insert(l+keyword," "), improvementN++;
                            }

                            /********************************/

                            if(codeResult[k].find("int") == l || codeResult[k].find("map") == l || codeResult[k].find("set") == l
                               && (codeResult[k][l+3] == ' ' || codeResult[k][l+3] == 9 || codeResult[k][l+3] == '<'))
                            {
                                inDefinition = true;
                            }
                            if(codeResult[k].find("long") == l || codeResult[k].find("char") == l || codeResult[k].find("list") == l || codeResult[k].find("pair") == l
                               && (codeResult[k][l+4] == ' ' || codeResult[k][l+4] == 9 || codeResult[k][l+4] == '<'))
                            {
                                inDefinition = true;
                            }
                            if(codeResult[k].find("float") == l ||codeResult[k].find("queue") == l || codeResult[k].find("array") == l
                               && (codeResult[k][l+5] == ' ' || codeResult[k][l+5] == 9 || codeResult[k][l+5] == '<'))
                            {
                                inDefinition = true;
                            }
                            if(codeResult[k].find("double") == l || codeResult[k].find("vector") == l
                               && (codeResult[k][l+6] == ' ' || codeResult[k][l+6] == 9 || codeResult[k][l+6] == '<'))
                            {
                                inDefinition = true;
                            }
                            if(codeResult[k].find("multiset") == l || codeResult[k].find("multimap") == l
                               && (codeResult[k][l+8] == ' ' || codeResult[k][l+8] == 9 || codeResult[k][l+8] == '<'))
                            {
                                inDefinition = true;
                            }
                        }


                        switch (codeResult[k][l])
                        {
                            case '(':
                                if(keyword && !inComments() && !inDoubleQuotationMarks && !inSingleQuotationMarks)
                                    parenthesis++;
                                break;
                            case ')':
                                if(keyword && keyword != 4 && !inComments() && !inDoubleQuotationMarks && !inSingleQuotationMarks)
                                {
                                    if(--parenthesis == 0)
                                    {
                                        keyword = 0;
                                        lastLineKeyword = true;
                                        if(codeResult[k][l+1] == '{')
                                        {
                                            if(getTheNthCharacterAfter(codeResult[k],l+1,1) != ' ' && getTheNthCharacterAfter(codeResult[k],l+1,1) != '/')
                                            {
                                            	improvementN++;
                                                codeResult.insert(codeResult.begin()+k+1,codeResult[k].substr(l+2,codeResult[k].length()-l-2));
                                                codeResult[k+1] = clearTheSpaceAtTheFront(codeResult[k+1]);
                                                codeResult[k].erase(l+2,codeResult[k].length()-l-2);
                                                codeResult[k].push_back('\n');
                                            	for(int p = 0; codeResult[k][p] == ' ' || codeResult[k][p] == 9; p++)
                                                    codeResult[k+1].insert(codeResult[k+1].begin(),codeResult[k][p]);
                                                codeResult[k+1].insert(codeResult[k+1].begin(),9);
                                                lastLineKeyword = false;
                                            }
                                            codeResult[k].insert(l+1," ");
                                        }
                                        else if(getTheNthCharacterAfter(codeResult[k],l,1) != ' ' && getTheNthCharacterAfter(codeResult[k],l,1) != '/'
                                        		&& getTheNthCharacterAfter(codeResult[k],l,1) != '{' && atMostOneSentenceOnALine == 2)
                                        {
                                        	improvementN++;
                                        	afterKeyword = true;

                                        	codeResult.insert(codeResult.begin()+k+1, codeResult[k].substr(l+1,codeResult[k].length()-l-1));
                                        	codeResult[k].erase(l+1,codeResult[k].length()-l-1);
                                        	codeResult[k].push_back('\n');
                                            codeResult[k+1] = clearTheSpaceAtTheFront(codeResult[k+1]);
                                            for(int p = 0; p < codeResult[k].length() && (codeResult[k][p] == ' ' || codeResult[k][p] == 9); p++)
                                                codeResult[k+1].insert(codeResult[k+1].begin(),codeResult[k][p]);
                                            codeResult[k+1].insert(codeResult[k+1].begin(),9);
                                            lastLineKeyword = false;
                                        }
                                        else if(getTheNthCharacterAfter(codeResult[k],l,1) == ' ' && getTheNthCharacterOf(codeResult[k+1],1) == '{' &&
												getTheNthCharacterOf(codeResult[k+1],2) == ' ' && theFirstBracesNeverWrap)
                                        {
                                        	improvementN++;
                                        	codeResult[k].pop_back();
                                        	codeResult[k].push_back(' ');
                                        	codeResult[k].push_back('{');
                                        	codeResult[k].push_back('\n');
                                        	codeResult.erase(codeResult.begin() + k + 1);
                                        }
                                    }
                                }
                                break;
                            case '{':
                                brace++;
                                break;
                            case '}':
                                brace--;
                                if(getTheNthCharacterAfter(codeResult[k],l,1) == ' ')
                                {
                                	while(k > 0 && getTheNthCharacterOf(codeResult[k-1],1) == ' ')
                                	{
                                		codeResult.erase(codeResult.begin() + --k), improvementN++;
                                	}
                                }
                                break;
                            case '\"':
                                if(l == 0 || codeResult[k][l-1] != '\\' && !inSingleQuotationMarks && !inComments())
                                    inDoubleQuotationMarks = 1 - inDoubleQuotationMarks;
                                break;
                            case '\'':
                                if(l == 0 || codeResult[k][l-1] != '\\' && !inDoubleQuotationMarks && !inComments())
                                    inSingleQuotationMarks = 1 - inSingleQuotationMarks;
                                break;
                            case '=':
                            case '>':
                                if(codeResult[k][l] != '=' && inDefinition)
                                    break;
                                if(l < codeResult[k].length() - 1 && codeResult[k][l+1] == '>')
                                {
                                    if(!inComments() && !inDoubleQuotationMarks)
                                    {
                                        if(codeResult[k][l+2] != ' ')
                                        	codeResult[k].insert(l+2," "), improvementN++;
                                        if(codeResult[k][l-1] != ' ')
                                        	codeResult[k].insert(l," "), improvementN++;
                                    }
                                    break;
                                }
                            case '<':
                            	if(codeResult[k][l] != '=' && inDefinition)
                                    break;
                                if(l < codeResult[k].length() - 1 && codeResult[k][l+1] == '<')
                                {
                                    if(!inComments() && !inDoubleQuotationMarks)
                                    {
                                        if(codeResult[k][l+2] != ' ')
                                        	codeResult[k].insert(l+2," "), improvementN++;
                                        if(codeResult[k][l-1] != ' ')
                                        	codeResult[k].insert(l," "), improvementN++;
                                    }
                                    break;
                                }
                            case '/':
                                if(l < codeResult[k].length() - 1 && codeResult[k][l+1] == '/' && !inDoubleQuotationMarks)
                                {
                                    inSingleLineComments = true;
                                    break;
                                }
                                else if(l < codeResult[k].length() - 1 && codeResult[k][l+1] == '*' && !inDoubleQuotationMarks)
                                {
                                    inMultiLineComments++;
                                    break;
                                }
                            case '*':
                                if(l < codeResult[k].length() - 1 && codeResult[k][l+1] == '/')
                                {
                                    inMultiLineComments--;
                                    break;
                                }
                            case '+':
                            case '-':
                            case '%':
                            case '!':
                                if(l < codeResult[k].length() - 1 && (codeResult[k][l+1] == '=') && !inComments() && !inDoubleQuotationMarks)
                                {
                                	if(spacesOnBothSidesOfSelfOperators || codeResult[k][l] == '!' || codeResult[k][l] == '=' || codeResult[k][l] == '>' || codeResult[k][l] == '<')
                                	{
                                		if(codeResult[k][l+2] != ' ')
                                			codeResult[k].insert(l+2," "), improvementN++;
                                		if(codeResult[k][l-1] != ' ')
                                			codeResult[k].insert(l," "), improvementN++;
                                	}
                                }
                                else if(codeResult[k][l] != '!' && l > 0 && l < codeResult[k].length() - 1 && !inComments() && !inDoubleQuotationMarks
                                        && (isLetterOrNumber(getTheNthCharacterAfter(codeResult[k], l, -1))
                                            || l > 1 && (getTheNthCharacterAfter(codeResult[k], l, -1) == '+' && getTheNthCharacterAfter(codeResult[k], l, -2) == '+'
                                                         || getTheNthCharacterAfter(codeResult[k], l, -1) == '-' && getTheNthCharacterAfter(codeResult[k], l, -2) == '-'
                                                         || getTheNthCharacterAfter(codeResult[k], l, -1) == ')' || getTheNthCharacterAfter(codeResult[k], l, 1) == '}'
                                                         || getTheNthCharacterAfter(codeResult[k], l, -1) == ']')
                                               && getTheNthCharacterAfter(codeResult[k], l, -1) != codeResult[k][l])
                                        && (isLetterOrNumber(getTheNthCharacterAfter(codeResult[k], l, 1))
                                            || l < codeResult[k].length() - 2 && (getTheNthCharacterAfter(codeResult[k], l, 1) == '+'
                                            || getTheNthCharacterAfter(codeResult[k], l, 1) == '-') || getTheNthCharacterAfter(codeResult[k], l, 1) == '!'
                                            || getTheNthCharacterAfter(codeResult[k], l, 1) == '*' || getTheNthCharacterAfter(codeResult[k], l, 1) == '&'
                                            || getTheNthCharacterAfter(codeResult[k], l, 1) == '(' || getTheNthCharacterAfter(codeResult[k], l, 1) == '{')
                                        && getTheNthCharacterAfter(codeResult[k], l, 1) != codeResult[k][l])
                                {
                                	if(spacesOnBothSidesOfArithmeticOperators && (codeResult[k][l] == '+' || codeResult[k][l] == '-' || codeResult[k][l] == '*' || codeResult[k][l] == '/' || codeResult[k][l] == '%')
                                		|| codeResult[k][l] == '>' || codeResult[k][l] == '<'
                                		|| codeResult[k][l] == '=' && spacesOnBothSidesOfAssignmentOperators)
                                	{
                                		if(codeResult[k][l+1] != ' ')
                                			codeResult[k].insert(l+1," "), improvementN++;
                                		if(codeResult[k][l-1] != ' ')
                                			codeResult[k].insert(l," "), improvementN++;
                                	}
                                }
                                break;

                            case '\n':
                                if(getTheNthCharacterOf(codeResult[k],1) == ' ' && !inComments() && !inDoubleQuotationMarks && !inSingleQuotationMarks && avoidTwoConsecutiveBlankLines)
                                {
                                    while(k < codeResult.size() -1 && getTheNthCharacterOf(codeResult[k+1],1) == ' ')
                                    	codeResult.erase(codeResult.begin()+k+1), improvementN++;
                                }
                                break;

                            case ',':
                                if(spaceAfterComma && !inComments() && !inDoubleQuotationMarks && !inSingleQuotationMarks)
                                    if(l < codeResult[k].length() - 2 && codeResult[k][l+1] != ' ')
                                    	codeResult[k].insert(l+1," "), improvementN++;
                                break;
                            case ';':
                                if(!inComments() && !inDoubleQuotationMarks && !inSingleQuotationMarks)
                                {
                                    inDefinition = false;
                                    if(!parenthesis && getTheNthCharacterAfter(codeResult[k],l,1) != ' ' && getTheNthCharacterAfter(codeResult[k],l,1) != '/')
                                    {
                                    	if(atMostOneSentenceOnALine >= 1)
										{
                                    		improvementN++;
											codeResult.insert(codeResult.begin()+k+1,codeResult[k].substr(l+1,codeResult[k].length()-l-1));
											codeResult[k+1] = clearTheSpaceAtTheFront(codeResult[k+1]);
											codeResult[k].erase(l+1,codeResult[k].length()-l-1);
											codeResult[k].push_back('\n');

											int p = 0;
											if(codeResult[k+1][0] == '}')
												p++;
											if(strictIndent && afterKeyword)
												p++, afterKeyword = 0;
											for(; codeResult[k][p] == ' ' || codeResult[k][p] == 9; p++)
												codeResult[k+1].insert(codeResult[k+1].begin(),codeResult[k][p]);
										}
                                    }
                                    else if(spaceAfterSemicolon && l < codeResult[k].length() - 2 && codeResult[k][l+1] != ' ')
                                        codeResult[k].insert(l+1," ");
                                }
                                break;
                            case ':':
                            case '?':
                                if(isLetterOrNumber(getTheNthCharacterAfter(codeResult[k], l, -1)) && isLetterOrNumber(getTheNthCharacterAfter(codeResult[k], l, 1))
                                   && !inComments() && !inDoubleQuotationMarks && !inSingleQuotationMarks)
                                {
                                    if(l < codeResult[k].length() - 1 && codeResult[k][l+1] != ' ')
                                    	codeResult[k].insert(l+1," "), improvementN++;
                                    if(l > 0 && codeResult[k][l-1] != ' ')
                                    	codeResult[k].insert(l," "), improvementN++;
                                }
                                break;
                            case '&':
                                if(l < codeResult[k].length() - 1 && codeResult[k][l+1] == '&' && !inComments() && !inDoubleQuotationMarks)
                                {
                                    if(codeResult[k][l+2] != ' ')
                                    	codeResult[k].insert(l+2," "), improvementN++;
                                    if(l > 0 && codeResult[k][l-1] != ' ')
                                    	codeResult[k].insert(l," "), improvementN++;
                                }
                                break;
                            case '|':
                                if(l < codeResult[k].length() - 1 && codeResult[k][l+1] == '|' && !inComments() && !inDoubleQuotationMarks)
                                {
                                    if(codeResult[k][l+2] != ' ')
                                    	codeResult[k].insert(l+2," "), improvementN++;
                                    if(l > 0 && codeResult[k][l-1] != ' ')
                                    	codeResult[k].insert(l," "), improvementN++;
                                }
                                break;
                            case ' ':
                                if(l > 2 && codeResult[k][l-1] == ' ' && codeResult[k][l-2] == ' ' && codeResult[k][l-3] == ' ')
                                {
                                	improvementN++;
                                    codeResult[k][l-3] = 9;
                                    codeResult[k].erase(l-2,3);
                                    l -= 3;
                                }
                                break;
                        }
                    }
                }
                inIncluding = inSingleLineComments = false;

                go(7.5,i-2);
                printf("%d",k);
            }
            string ccc = argv[i];
            if(!overwriteOriginalFile)
            {
            	while(ccc.back() != '.')
            		ccc.pop_back();
            	ccc.pop_back();
            	ccc.append("-BetterCodeStyle.cpp");
            }
            ofstream fout(ccc);
            for(int k = 0; k < codeResult.size(); k++)
                fout << codeResult[k];

            codeResult.clear();
            inIncluding = inSingleQuotationMarks = inDoubleQuotationMarks = inSingleLineComments = inDefinition = inMultiLineComments
                    	= keyword = brace = lineI = parenthesis = 0;

            ccc = argv[i];
            if(ccc.find('\\') != string::npos)
            	ccc = ccc.substr(ccc.find_last_of('\\')+1, ccc.length()-ccc.find_last_of('\\'));
            go(0,i-2);
            printf("【%d/%d】「%s」处理完成，共有%d处改善。\n",i,argc-1,ccc.c_str(),improvementN);
        }

    	printf("\n");
    	color(135);
    	system("pause");
    }
    else
	{
    	system("mode con cols=132 lines=42");
    	SetConsoleTitle("码风自动改善工具 - 设置（若要处理代码文件请用本程序打开该文件）");

    	go(0,0);
    	color(119);
    	printf(" >> Author: @Bilibili @GitHub TreeYeah    Date: 2021-8-5");
    	color(120);
    	printf("\n >> 【↑】【↓】移动光标，空格更改设置，【Esc】保存并退出。\n\n");

    	printSetting("不能有两个连续空行\n");
    	printSetting("复合赋值运算符 `+=` `-=` `*=` `\\=` `%=` 左右各一个空格\n");
    	printSetting("算术运算符 `+` `-` `*` `/` `%` 左右各一个空格\n");
    	printSetting("赋值运算符 `=` 左右各一个空格\n");
    	printSetting("`;` 仅右侧有一个空格\n");
    	printSetting("`,` 仅右侧有一个空格\n");
    	printSetting("`if` `while` `for` `switch` 右接一个空格\n");
    	printSetting("除了 `for` 中的三个语句，每行 [ A.可以有任意数量的语句 / B.仅允许`if` `else` `while` `for` 后同行存在语句 / C.不能有超过一个语句 ]\n");
    	printSetting("前大括号从不换行\n");
    	printSetting("严格的缩进\n");
    	printSetting("覆盖原始文件\n");

    	color(120);
    	printf("        （以下锁定开启）\n\n");
    	printSetting("将缩进中的四个空格替换为一个Tab\n");
    	printSetting("`:` `?` 左右各一个空格 \n");
    	printSetting("`}`前不应该有多余的空行\n");
    	printSetting("行尾不应该有多余的空格\n");
    	printSetting("比较运算符 `==` `>=` `<=` `!=` `>` `<` 左右均有一个空格\n");
    	printSetting("`<<` `>>` 左右各一个空格\n");
    	printSetting("逻辑运算符 `||` `&&` 左右各一个空格\n");

    	string OnOff[2]  = {"  ","√"};	// 0 off, 1 on
    	string ABC[3] = {"Ａ","Ｂ","Ｃ"};

    	for(int p = 1; p <= 19; p++)
    	{
    		go(2.5, p*2-1);
    		if(p > 11)
				color(135);
    		else
    			color(15);

    		if(p == 1)
    			cout << OnOff[avoidTwoConsecutiveBlankLines];
    		else if(p == 2)
    			cout << OnOff[spacesOnBothSidesOfSelfOperators];
    		else if(p == 3)
    			cout << OnOff[spacesOnBothSidesOfArithmeticOperators];
    		else if(p == 4)
    			cout << OnOff[spacesOnBothSidesOfAssignmentOperators];
    		else if(p == 5)
    			cout << OnOff[spaceAfterSemicolon];
    		else if(p == 6)
    			cout << OnOff[spaceAfterComma];
    		else if(p == 7)
    			cout << OnOff[spaceAfterKeyword];
    		else if(p == 8)
    			cout << ABC[atMostOneSentenceOnALine];
    		else if(p == 9)
    			cout << OnOff[theFirstBracesNeverWrap];
    		else if(p == 10)
    			cout << OnOff[strictIndent];
    		else if(p == 11)
    			cout << OnOff[overwriteOriginalFile];
    		else if(p != 12)
    			cout << OnOff[1];
    	}

    	int p = 1, pb, key = 0;

    	color(112);
    	go(1,p*2-1);
    	printf("->");

		while(key != 27)
		{
			key = getch();
			if(key == 224)
			{

				key = getch();
				pb = p;
				switch (key)
				{
					case 72:
						if(p > 1)
						{
							p--;
						}
						color(112);
						go(1,pb*2-1);
						printf("  ");
						go(1,p*2-1);
						printf("->");
						break;

					case 80:
						if(p < 11)
						{
							p++;
						}
						color(112);
						go(1,pb*2-1);
						printf("  ");
						go(1,p*2-1);
						printf("->");
						break;
				}
			}
			else if(key == 32)
			{
				go(2.5, p*2-1);
				color(15);
				if(p == 1)
					avoidTwoConsecutiveBlankLines = 1 - avoidTwoConsecutiveBlankLines,
					cout << OnOff[avoidTwoConsecutiveBlankLines];
				else if(p == 2)
					spacesOnBothSidesOfSelfOperators = 1 - spacesOnBothSidesOfSelfOperators,
					cout << OnOff[spacesOnBothSidesOfSelfOperators];
				else if(p == 3)
					spacesOnBothSidesOfArithmeticOperators = 1 - spacesOnBothSidesOfArithmeticOperators,
					cout << OnOff[spacesOnBothSidesOfArithmeticOperators];
				else if(p == 4)
					spacesOnBothSidesOfAssignmentOperators = 1 - spacesOnBothSidesOfAssignmentOperators,
					cout << OnOff[spacesOnBothSidesOfAssignmentOperators];
				else if(p == 5)
					spaceAfterSemicolon = 1 - spaceAfterSemicolon,
					cout << OnOff[spaceAfterSemicolon];
				else if(p == 6)
					spaceAfterComma = 1 - spaceAfterComma,
					cout << OnOff[spaceAfterComma];
				else if(p == 7)
					spaceAfterKeyword = 1 - spaceAfterKeyword,
					cout << OnOff[spaceAfterKeyword];
				else if(p == 8)
					atMostOneSentenceOnALine = ( atMostOneSentenceOnALine + 1 ) % 3,
					cout << ABC[atMostOneSentenceOnALine];
				else if(p == 9)
					theFirstBracesNeverWrap = 1 - theFirstBracesNeverWrap,
					cout << OnOff[theFirstBracesNeverWrap];
				else if(p == 10)
				{
					if(strictIndent || MessageBox(NULL,"此选项下所有被判定为多余的缩进会被删除，这可能导致某些注释处或其它地方不再对齐，是否继续？","风险提示",MB_YESNO) == 6)
						strictIndent = 1 - strictIndent,
						cout << OnOff[strictIndent];
				}
				else if(p == 11)
					if(overwriteOriginalFile || MessageBox(NULL,"此选项下文件被处理后将无法还原，是否继续？","风险提示",MB_YESNO) == 6)
						overwriteOriginalFile = 1 - overwriteOriginalFile,
						cout << OnOff[overwriteOriginalFile];
			}
		}

		if(saveSettings())
		{
			SetConsoleTitle("保存成功！");
			Sleep(500);
		}
	}

    return 0;
}
