#include<iostream>
#include<fstream>
#include<ctype.h>
#include<string>
#include <list>
#include <vector>
#include <stack>
using namespace std;



enum TOKEN {
    ASSIGN_SY,
equlty , notEquil , lessThan , LessOrEquil , greatThan , greatOrEquil,
ELSE ,IF,
LPARN, RPARN,
Var,
Const,
MULT , DIVI ,PLUS ,MINUS,
integer, floating , character ,
SEMICOL,
$,
ERROR,
/////
  Statements,
    Statement,
    tempStmt,
 footerStmt,
 Variable_Decl ,
  Type,
  Expression,
  footExpression ,
  Add_Op ,
   Term,
    FootTerm,
      Mul_Op,
       Factor,
         If_Statement,
          FootIf_Statement,
           Condition_Expression,
           Comparison_Op
};

typedef struct
{
    int number;
    TOKEN tok;
}koko;

typedef struct
{
    int number=-1;
   TOKEN toks[6] ;
}element;

element table[17][22];
int line=1;
char c;


void showTokenName(TOKEN tok);
void showexpectedToken(TOKEN nontok);


class Scanner{
	public:
	    Scanner(string filename);
	    ~Scanner(void);
	    TOKEN get_token(void);
	    void display_tokens(void);
	    list <koko> get_list_tokens(void);
private :
		ifstream f;
	    TOKEN check_reserved(string s){

            if (s== "int")return integer;
            else if (s== "float")return floating;
			else if (s== "char")return character;

			else if (s== "if")return IF;
			else if (s== "else")return ELSE;

			else return Var;
	    }
};
//*****************************************************//

		Scanner :: Scanner(string filename){
		f.open(filename.c_str());
		}
		Scanner :: ~Scanner(void){
		f.close();
		}
//*****************************************************//
TOKEN Scanner ::  get_token(void){
		char ch;
		string S;
		f.get(ch);

		while (isspace(ch))
        {
            if(ch=='\n') line++;
            if (f.eof())return $;
            f.get(ch);
        }
		if (f.eof()) return $;
		else if (ch == ';')return SEMICOL;
		else if (ch == '(')return LPARN;
		else if (ch == ')')return RPARN;
		else if (ch == '+')return PLUS;
		else if (ch == '-')return MINUS;
		else if (ch == '*')return MULT;
		else if (ch == '/')return DIVI;

		else if (ch == '='){
			f.get(ch);
        if (f.eof())return ASSIGN_SY;
			if (ch == '=')return equlty;
			else{
				f.putback(ch);
				return ASSIGN_SY;
			}
		}
		else if (ch == '<'){
			f.get(ch);
			if (f.eof())return lessThan;
			if (ch == '=')return LessOrEquil;
			else if (ch == '>')return notEquil;
			else {
				f.putback(ch);
				return lessThan;
			}
		}
		else if (ch == '>'){
			f.get(ch);
						if (f.eof())return greatThan;
			if (ch == '=')return greatOrEquil;
			else {
				f.putback(ch);
				return greatThan;
			}
		}
		else if (isalpha(ch)){
            S= ch; f.get(ch);
            if (f.eof())return check_reserved(S);

			while (isalnum(ch)) {
				S += ch;
				f.get(ch);
                if (f.eof())return check_reserved(S);//putback

			}
			f.putback(ch);
			return check_reserved(S);
		}

		else if (isdigit(ch)) {      // real or integer
			S = ch;
			f.get(ch);
            if (f.eof())return Const;
			while (isdigit(ch)){
				S += ch;

				f.get(ch);
                    if (f.eof())return Const;
			}
				{
				    if (ch == '.')
                    {
                        S+=ch;
                        f.get(ch);
                        if (f.eof())return Const;
                        while (isdigit(ch)){
				        S += ch;
				        f.get(ch);
                        if (f.eof())return Const;
                        }
                        return Const;
                    }
				}
				f.putback(ch);
                return Const;
		}
		else {return ERROR;}
	}
//*****************************************************//
void  Scanner :: display_tokens(void){
		TOKEN A ;
        list <koko> listTokens;
        koko ko;
        listTokens=get_list_tokens();
       int number =-1;
          while (!listTokens.empty())
        {
          ko=listTokens.front();
          listTokens.pop_front() ;

          if(number!=ko.number)
          {
              number=ko.number;
              if(number!=1)
              {
                   cout << " ]";
              }

              cout << endl;
            if(ko .tok== $)
            {
            break;
            }
                          cout << number ;
              cout <<"                ";
              cout << "[ ";
          }
          else
          {
                        cout <<",";

          }
          printf(" ");
          showTokenName(ko.tok);
}


	}

    list <koko>  Scanner :: get_list_tokens(void)
    {

        TOKEN A ;
        line=1;
        f.clear ();
        f.seekg(0, std::ios::beg);
        if (f.eof())cout << "end_of_file " << endl;
        list <koko> listTokens;
        while (!f.eof()){
        A = get_token();
        koko toke;
        toke.number=line;
        toke.tok=A;
        listTokens.push_back(toke );
        }
        return listTokens;
    }

////#######################################################################
int parser(stack<koko > InputStack)
{
stack<TOKEN> ParsingStack;
ParsingStack.push($);
ParsingStack.push(Statements);
 TOKEN Parsunit = ParsingStack.top();
 koko unitinput = InputStack.top();

		while((Parsunit!=$)||(unitinput.tok!=$))
		{

            if((Parsunit<22)&&(unitinput.tok==Parsunit))
            {
                ParsingStack.pop();
                Parsunit = ParsingStack.top();
                InputStack.pop();
                unitinput = InputStack.top();
            }
            else
            {
                 if((Parsunit<22))
                 {
                     element t;
                    t=table[Parsunit-23][unitinput.tok];
                    {
                        printf("error in line %d not matched token ",unitinput.number);
                        showTokenName(unitinput.tok);
                        printf("\nexpected ",unitinput.number);
                        showTokenName(Parsunit);
                        return -1;
                    }
                 }
                if((Parsunit>=22))
                {
                    element t;
                    t=table[Parsunit-23][unitinput.tok];
                    if(t.number==-1)
                    {
                        printf("error in line %d not matched token ",unitinput.number);
                        showTokenName(unitinput.tok);
                        printf("\nexpected ",unitinput.number);
                        showTokenName(Parsunit);
                        printf("shoud start with  ");
                        showexpectedToken(Parsunit);
                        return -1;
                    }
                     ParsingStack.pop();
                    int i;
                    for(i=t.number-1;i>=0;i--)
                    {

                        ParsingStack.push(t.toks[i]);
                    }
                     Parsunit = ParsingStack.top();
                }
            }
            if((Parsunit==$)&&(unitinput.tok==$))
            {
                printf("accept");
                return 0;
            }
		}
	    }


void inti_table(void)
{
//0
element f;
f.number=0;
table[0][7]=f;
f.number=2;f.toks[0]=  Statement;f.toks[1]=  Statements;
table[0][8]=f;
f.number=2;f.toks[0]=  Statement;f.toks[1]=  Statements;
table[0][17]=f;
f.number=2;f.toks[0]=  Statement;f.toks[1]=  Statements;
table[0][18]=f;
f.number=2;f.toks[0]=  Statement;f.toks[1]=  Statements;
table[0][19]=f;
f.number=0;
table[0][21]=f;
//1
f.number=1;f.toks[0]=  If_Statement;
table[1][8]=f;
f.number=1;f.toks[0]=  tempStmt;
table[1][17]=f;
f.number=1;f.toks[0]=  tempStmt;
table[1][18]=f;
f.number=1;f.toks[0]=  tempStmt;
table[1][19]=f;
//2
f.number=1;f.toks[0]=  If_Statement;
table[2][8]=f;
f.number=2;f.toks[0]=  Variable_Decl;f.toks[1]=  footerStmt;
table[2][17]=f;
f.number=2;f.toks[0]=  Variable_Decl;f.toks[1]=  footerStmt;
table[2][18]=f;
f.number=2;f.toks[0]=  Variable_Decl;f.toks[1]=  footerStmt;
table[2][19]=f;
//3
f.number=3;f.toks[0]=  ASSIGN_SY;f.toks[1]=  Expression;f.toks[2]=  SEMICOL;
table[3][0]=f;
f.number=1;f.toks[0]=  SEMICOL;
table[3][20]=f;
//4
f.number=2;f.toks[0]=  Type;f.toks[1]=  Var;
table[4][17]=f;
f.number=2;f.toks[0]=  Type;f.toks[1]=  Var;
table[4][18]=f;
f.number=2;f.toks[0]=  Type;f.toks[1]=  Var;
table[4][19]=f;
//5
f.number=1;f.toks[0]=  integer;
table[5][17]=f;
f.number=1;f.toks[0]=  floating;
table[5][18]=f;
f.number=1;f.toks[0]=  character;
table[5][19]=f;
//6
f.number=2;f.toks[0]=  Term;f.toks[1]=  footExpression;
table[6][9]=f;
f.number=2;f.toks[0]=  Term;f.toks[1]=  footExpression;
table[6][11]=f;
f.number=2;f.toks[0]=  Term;f.toks[1]=  footExpression;
table[6][12]=f;
//7
f.number=0;
table[7][1]=f;
f.number=0;
table[7][2]=f;
f.number=0;
table[7][3]=f;
f.number=0;
table[7][4]=f;
f.number=0;
table[7][5]=f;
f.number=0;
table[7][6]=f;
f.number=0;
table[7][10]=f;
f.number=2;f.toks[0]=  Add_Op;f.toks[1]=  Expression;
table[7][15]=f;
f.number=2;f.toks[0]=  Add_Op;f.toks[1]=  Expression;
table[7][16]=f;
f.number=0;
table[7][20]=f;
//8
f.number=1;f.toks[0]=  PLUS;
table[8][15]=f;
f.number=1;f.toks[0]=  MINUS;
table[8][16]=f;
//9
f.number=2;f.toks[0]=  Factor;f.toks[1]=  FootTerm;
table[9][9]=f;
f.number=2;f.toks[0]=  Factor;f.toks[1]=  FootTerm;
table[9][11]=f;
f.number=2;f.toks[0]=  Factor;f.toks[1]=  FootTerm;
table[9][12]=f;
//10
f.number=0;
table[10][1]=f;
f.number=0;
table[10][2]=f;
f.number=0;
table[10][3]=f;
f.number=0;
table[10][4]=f;
f.number=0;
table[10][5]=f;
f.number=0;
table[10][6]=f;
f.number=0;
table[10][10]=f;
f.number=2;f.toks[0]=  Mul_Op;f.toks[1]=  Term;
table[10][13]=f;
f.number=2;f.toks[0]=  Mul_Op;f.toks[1]=  Term;
table[10][14]=f;
f.number=0;
table[10][15]=f;
f.number=0;
table[10][16]=f;
f.number=0;
table[10][20]=f;
//11
f.number=1;f.toks[0]=  MULT;
table[11][13]=f;
f.number=1;f.toks[0]=  DIVI;
table[11][14]=f;
//12
f.number=3;f.toks[0]=  LPARN;f.toks[1]=  Expression;f.toks[2]=  RPARN;
table[12][9]=f;
f.number=1;f.toks[0]=  Var;
table[12][11]=f;
f.number=1;f.toks[0]=  Const;
table[12][12]=f;
//13<If_Statement>→if(<Condition_Expression>)<Statements><FootIf_Statement>
f.number=6;f.toks[0]=  IF;f.toks[1]=  LPARN;f.toks[2]=  Condition_Expression;
    f.toks[3]=  RPARN;;f.toks[4]=  Statements;f.toks[5]=  FootIf_Statement;
table[13][8]=f;
//14else <Statements>
f.number=2;f.toks[0]=  ELSE;f.toks[1]=  Statements;
table[14][7]=f;
f.number=0;
table[14][8]=f;
f.number=0;
table[14][17]=f;
f.number=0;
table[14][18]=f;
f.number=0;
table[14][19]=f;
f.number=0;
table[14][21]=f;
//15 <Expression> <Comparison_Op> <Expression>
f.number=3;f.toks[0]=  Expression;f.toks[1]=  Comparison_Op;f.toks[2]=  Expression;
table[15][9]=f;
f.number=0;
table[15][10]=f;
f.number=3;f.toks[0]=  Expression;f.toks[1]=  Comparison_Op;f.toks[2]=  Expression;
table[15][11]=f;
f.number=3;f.toks[0]=  Expression;f.toks[1]=  Comparison_Op;f.toks[2]=  Expression;
table[15][12]=f;
//16    equlty , notEquil , lessThan , LessOrEquil , greatThan , greatOrEquil,
f.number=1;f.toks[0]=  equlty;
table[16][1]=f;
f.number=1;f.toks[0]=  notEquil;
table[16][2]=f;
f.number=1;f.toks[0]=  lessThan;
table[16][3]=f;
f.number=1;f.toks[0]=  LessOrEquil;
table[16][4]=f;
f.number=1;f.toks[0]=  greatThan;
table[16][5]=f;
f.number=1;f.toks[0]=  greatOrEquil;
table[16][6]=f;

}

void showexpectedToken(TOKEN nontok)
{
    int i,j=0;
    int number;
    element elm;
    TOKEN tok= static_cast<TOKEN>(4);

    for(i=0;i<21;i++)
    {
         elm=table[nontok-23][i];
        number=elm.number;
       if(number!=-1)
       {
           if(j!=0)
           {
               printf("or");
           }
              printf("  '");
              showTokenName(static_cast<TOKEN>(i));
              printf("'  ");
            j++;
       }

    }
}
void showTokenName(TOKEN tok)
{
    switch (tok){
				case Var: cout << "variable"; break;
				case PLUS: cout << "+"; break;
				case MINUS: cout << "-" ; break;
				case MULT: cout << "*" ; break;
				case DIVI: cout << "/" ; break;
				case LessOrEquil: cout << "<=" ; break;
				case lessThan: cout << "<"; break;
				case equlty: cout << "==" ; break;
				case greatThan: cout <<">" ; break;
				case greatOrEquil: cout << ">=" ; break;
				case notEquil: cout << "<>" ; break;
				case character: cout<<"character" ; break;
				case floating: cout<<"floating" ; break;
				case integer: cout<<"integer" ; break;
				case Const: cout<<"Constant" ; break;
				case $: cout << "$" ; break;
				case LPARN: cout << "(" ; break;
				case RPARN: cout << ")" ; break;
				case IF: cout << "if" ; break;
				case ELSE: cout << "else" ; break;
				case ASSIGN_SY: cout << "=" ; break;
				case SEMICOL: cout << ";" ; break;

				case Statements: cout << "Statements nonToken"; break;
				case Statement: cout << "Statement nonToken" ; break;
				case tempStmt: cout << "tempStmt nonToken" ; break;
				case footerStmt: cout << "footerStmt nonToken" ; break;
				case Variable_Decl: cout << "Variable_Decl nonToken" ; break;
				case Type: cout << "Type nonToken" ; break;
				case Expression: cout << "Expression nonToken" ; break;
				case footExpression: cout << "footExpression nonToken" ; break;
				case Add_Op: cout << "Add_Op nonToken" ; break;
				case Term: cout << "Term nonToken" ; break;
				case Factor: cout << "Factor nonToken" ; break;
				case If_Statement: cout << "If_Statement nonToken"; break;
				case FootIf_Statement: cout << "FootIf_Statement nonToken"; break;
				case Condition_Expression: cout << "Condition_Expression nonToken"; break;
				case Comparison_Op: cout << "Comparison_Op nonToken" ; break;

					default : cout <<"not Know character";

		    }
}


int main(){

    string name ;
    name="third.txt";
    Scanner S(name);
    cout<<"++++++++++|| tokens || +++++++" <<endl<< endl;
    S.display_tokens();
    cout<<endl ;
    list <koko> listTokens;
    listTokens=S.get_list_tokens();
    S.~Scanner();
    stack<koko > InputStack;
    while (!listTokens.empty())
    {
        InputStack.push(listTokens.back());
        listTokens.pop_back() ;
    }
    inti_table();
    parser(InputStack);

	return 0 ;
}
