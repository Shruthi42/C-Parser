#ifndef PARSE
#define PARSE
#include"parse.h"
#endif


char * returnToken(int num)
{
	char * token = (char*)malloc(MAXTOKENSIZE);
if(
num==';'
||num=='{'
||num=='}'
||num==','
||num==':'
||num=='='
||num=='('
||num==')'
||num=='['
||num==']'
||num=='.'
||num=='&'
||num=='!'
||num=='~'
||num=='-'
||num=='+'
||num=='*'
||num=='/'
||num=='%'
||num=='<'
||num=='>'
||num=='^'
||num=='|'
||num=='?'
) 
{char a[2]; a[0] = num; a[1] = '\0'; strcpy(token,a);}
else if(num==258) strcpy(token,"IDENTIFIER");
else if(num==259) strcpy(token,"CONSTANT");
else if(num==260) strcpy(token,"STRING_LITERAL");
else if(num==261) strcpy(token,"SIZE_OF");
else if(num==262) strcpy(token,"PTR_OP");
else if(num==263) strcpy(token,"INC_OP");
else if(num==264) strcpy(token,"DEC_OP");
else if(num==265) strcpy(token,"LEFT_OP");
else if(num==266) strcpy(token,"RIGHT_OP");
else if(num==267) strcpy(token,"LE_OP");
else if(num==268) strcpy(token,"GE_OP");
else if(num==269) strcpy(token,"EQ_OP");
else if(num==270) strcpy(token,"NE_OP");
else if(num==271) strcpy(token,"AND_OP");
else if(num==272) strcpy(token,"OR_OP");
else if(num==273) strcpy(token,"MUL_ASSIGN");
else if(num==274) strcpy(token,"DIV_ASSIGN");
else if(num==275) strcpy(token,"MOD_ASSIGN");
else if(num==276) strcpy(token,"ADD_ASSIGN");
else if(num==277) strcpy(token,"SUB_ASSIGN");
else if(num==278) strcpy(token,"LEFT_ASSIGN");
else if(num==279) strcpy(token,"RIGHT_ASSIGN");
else if(num==280) strcpy(token,"AND_ASSIGN");
else if(num==281) strcpy(token,"XOR_ASSIGN");			
else if(num==282) strcpy(token,"OR_ASSIGN");
else if(num==283) strcpy(token,"TYPE_NAME");
else if(num==284) strcpy(token,"TYPEDEF");
else if(num==285) strcpy(token,"EXTERN");
else if(num==286) strcpy(token,"STATIC");
else if(num==287) strcpy(token,"AUTO");
else if(num==288) strcpy(token,"REGISTER");
else if(num==289) strcpy(token,"CHAR");
else if(num==290) strcpy(token,"SHORT");
else if(num==291) strcpy(token,"INT");
else if(num==292) strcpy(token,"LONG");
else if(num==293) strcpy(token,"SIGNED");
else if(num==294) strcpy(token,"UNSIGNED");
else if(num==295) strcpy(token,"FLOAT");
else if(num==296) strcpy(token,"DOUBLE");
else if(num==297) strcpy(token,"CONST");
else if(num==298) strcpy(token,"VOLATILE");
else if(num==299) strcpy(token,"VOID");
else if(num==300) strcpy(token,"STRUCT");
else if(num==301) strcpy(token,"UNION");
else if(num==302) strcpy(token,"ENUM");
else if(num==303) strcpy(token,"ELLIPSIS");
else if(num==304) strcpy(token,"CASE");
else if(num==305) strcpy(token,"DEFAULT");
else if(num==306) strcpy(token,"IF");
else if(num==307) strcpy(token,"ELSE");
else if(num==308) strcpy(token,"SWITCH");
else if(num==309) strcpy(token,"WHILE");
else if(num==310) strcpy(token,"DO");
else if(num==311) strcpy(token,"FOR");
else if(num==312) strcpy(token,"GOTO");
else if(num==313) strcpy(token,"CONTINUE");
else if(num==314) strcpy(token,"BREAK");
else if(num==315) strcpy(token,"RETURN");
else if(num==316) strcpy(token,"UN_OP");
else if(num==317) strcpy(token,"ASSIGN");
else if(num==318) strcpy(token,"TYPE");
else if(num==319) strcpy(token,"SHIFT_OP");
else if(num==320) strcpy(token,"LE_GE_OP");
else if(num==321) strcpy(token,"EQ_NE_OP");
else if(num==322) strcpy(token,"INC_DEC_OP");
return token;
}
