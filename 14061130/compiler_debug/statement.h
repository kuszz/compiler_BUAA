#ifndef STATEMENT_H_INCLUDED
#define STATEMENT_H_INCLUDED
#define IDENTSIZE 50

char fStack[pTBLLENGTH][IDENTSIZE];//��������λ��
int period,period_func;       //��ǰ�������崦��ʲô�׶Σ������ж�
char varible[TOKEN_SIZE] = {0};     //���ڴ�����һ��token�����飬��������
int temp_vaddr;             //���ű�λ��
int temp_faddr;             //��Ԫʽλ��
int temp_vTBL_addr;             //����ջλ��
int expressionResult;       //expression�Ľ�����������switch��
int elmNum = 1;                 //��ǰ��Ԫʽ��Ԫ��
char elm[3][20] = {{0}};
char result[1024] = {0};
char tempReg[5];   //��ʱ�Ĵ���
char tempFunc[5];   //��ʱfunction
int tempRegNum = 0;
int tempFuncNum = 0;
int ifGlobal = 0;      //�����жϵ�ǰȫ�ֱ����Ƿ�ȫ������
int currentType = 0;  //0:void 1:int 2:char
int fStack_length = -1;
int stateAmount = 0;
int caseNum = 0;
int currentItem = 0;

int inBranch = 0;
int returnNeed = 1;

//������������
int statement();
int expression();  //���ʽ
int factor();      //��
int item();        //����
int condition();
void paranInStack();
void brackInStack();
void quaInStack();
void paranOutStack();
void brackOutStack();
void quaOutStack();
int situationTBL();
int defaultSituation();
int sentence();
int complexSentence();
int conststatement();
int variblestatement(char var[],int type);
int identityItemStatement(char var[],int factorNum);
void addReg();

//statement
void initArray(char ch[]){
    int i;
    for(i = 0; i < FELMSIZE; i++){
        ch[i] = '\0';
    }
}

void addReg()
{
    char num[4] = {0};
    tempReg[0] = '@';
    tempReg[1] = '_';
    tempReg[2] = '\0';
    itoa(tempRegNum,num,10);
    strcat(tempReg,num);
    tempRegNum ++;
}

void addFunc()
{
    char num[4] = {0};
    tempFunc[0] = '#';
    tempFunc[1] = '_';
    tempFunc[2] = '\0';
    itoa(tempFuncNum,num,10);
    strcat(tempFunc,num);
    tempFuncNum ++;
}


int condition()
{
    char temp_elm[3][20] = {{0}};
    char op[TOKEN_SIZE] = {0};

    if(expression() == -1)
    {
        return -1;
    }
    strcpy(temp_elm[1],elm[1]);
    getsym();
    if(!strcmp(symbol,"EQLSYM") ||
            !strcmp(symbol,"LSSSYM") ||
            !strcmp(symbol,"GRTSYM") ||
            !strcmp(symbol,"GRTEQLSYM") ||
            !strcmp(symbol,"LSSEQLSYM") ||
            !strcmp(symbol,"NEQSYM"))
    {
        strcpy(op,token);
        getsym();
        if(expression() == -1)
        {
            return -1;
        }
        strcpy(temp_elm[2],elm[1]);
        //����������һ��С���ţ�֮��Ż���label�������������ĵط�������λ��
        //@action
        emit(op,"",temp_elm[1],temp_elm[2]);
        conditionStack[++conditionStack_length] = fTBL_length;
    }
    else
    {
        ungets(token);
        //����������һ��С���ţ�֮��Ż���label�������������ĵط�������λ��
        //@action
        emit(op,"",temp_elm[1],temp_elm[2]);
        conditionStack[++conditionStack_length] = fTBL_length;
    }
    return 1;
}

int expression()
{
    char temp_elm[3][20] = {{0}};
    int tempStartSignal = 0;
    int tempSignal = 0;
    char tempChar[1024];

    tempChar[0]= '\0';
    expressType = 1;
    if(!strcmp(token,"+") || !strcmp(token,"-"))
    {
        if(!strcmp(token,"+"))
        {
            tempStartSignal = 1;
            tempChar[0] = '+';
        }
        else if(!strcmp(token,"-"))
        {
            tempStartSignal = 2;
            tempChar[0] = '-';
        }
        getsym();
    }
    elmNum = 1;
    if(factor(elmNum) == -1)
    {
        return -1;
    }
    if(tempStartSignal == 2)
    {
        addReg();
        emit("-",tempReg,"0",elm[1]);
        strcpy(dTBL[++dTBL_length].name,tempReg);
        strcpy(temp_elm[1],tempReg);
        //strcpy(temp_elm[1],elm[1]);
    }
    else
    {
        strcpy(temp_elm[1],elm[1]);
    }
    getsym();
    if(!strcmp(token,"+") || !strcmp(token,"-"))
    {
        while(!strcmp(token,"+") || !strcmp(token,"-"))
        {
            elmNum = 2;
            if(!strcmp(token,"+"))
            {
                tempSignal = 1;
            }
            else if(!strcmp(token,"-"))
            {
                tempSignal = 2;
            }

            getsym();
            if(factor(elmNum) == -1)
            {
                return -1;
            }
            strcpy(temp_elm[2],elm[2]);
            //@action
            if(tempSignal == 1)
            {
                //���صĵ�ַ������ջ�ĵ�һ�ڶ��ĵ�ַ�������������ڵڶ���ַ��
                addReg();
                emit("+",tempReg,temp_elm[1],temp_elm[2]);
                strcpy(dTBL[++dTBL_length].name,tempReg);

            }
            else
            {
                addReg();
                emit("-",tempReg,temp_elm[1],temp_elm[2]);
                strcpy(dTBL[++dTBL_length].name,tempReg);
            }

            getsym();
            strcpy(elm[1],tempReg);
            strcpy(temp_elm[1],tempReg);
        }
    }

    else
    {
        strcpy(elm[1],temp_elm[1]);
    }
    ungets(token);
    return 1;
}

int factor(int expressionNum)
{
    char temp_elm[3][20] = {{0}};
    elmNum = 1;
    int tempSignal = 0;
    if(item(elmNum) == -1)
    {
        return -1;
    }
    strcpy(temp_elm[1],elm[1]);
    getsym();

    if(!strcmp(token,"*") || !strcmp(token,"/"))
    {

        while(!strcmp(token,"*") || !strcmp(token,"/"))
        {
            elmNum = 2;
            if(!strcmp(token,"*"))
            {
                tempSignal = 1;
            }
            else if(!strcmp(token,"/"))
            {
                tempSignal = 2;
            }
            getsym();
            if(item(elmNum) == -1)
            {
                return -1;
            }
            getsym();
            strcpy(temp_elm[2],elm[2]);
            //@action
            if(tempSignal == 1)
            {
                //���صĵ�ַ������ջ�ĵ�һ�ڶ��ĵ�ַ�������������ڵڶ���ַ��
                addReg();
                emit("*",tempReg,temp_elm[1],temp_elm[2]);
                strcpy(dTBL[++dTBL_length].name,tempReg);
            }
            else
            {
                addReg();
                emit("/",tempReg,temp_elm[1],temp_elm[2]);
                strcpy(dTBL[++dTBL_length].name,tempReg);
            }
            strcpy(elm[expressionNum],tempReg);
            strcpy(temp_elm[1],tempReg);
        }
    }
    else
    {
        strcpy(elm[expressionNum],temp_elm[1]);
    }
    ungets(token);
    return 1;
}

int item(int factorNum)
{
    int tempSignal = 0;
    char _tempNum[FELMSIZE];
    int tempNum;
    char tempchar[20];
    currentItem = 0;
    if(!strcmp(symbol,"IDENTSYM"))   //�������û��߱�ʶ����������
    {
        strcpy(varible,token);

        if(identityItemStatement(varible,factorNum) == -1)
        {
            return -1;
        }

        if(elm[factorNum][0] == '#'){

                itoa(4 * vTBL[lookUp(elm[factorNum],currentlevel)].raddr,_tempNum,10);
                emit("+","$t3","$s3",_tempNum);
                storeData("0($t3)","$s4");
        }

    }
    else if(!strcmp(token,"+") || !strcmp(token,"-"))
    {
        if(!strcmp(token,"+"))
        {
            tempSignal = 1;
        }
        else
        {
            tempSignal = 2;
        }
        getsym();
        if(!!strcmp(symbol,"NUM"))
        {
            error(15);
            nextlineRead();
            return -1;
        }
        if(token[0] == '0'){
            error(17);
            nextlineRead();
            return -1;
        }

        //@action
        tempNum = atoi(token);
        if(tempSignal == 2)
        {
            tempNum = 0 - tempNum;
            elm[factorNum][0] = '-';
            tempNum = 0 - tempNum;
        }
        else
        {
            elm[factorNum][0] = '+';
        }
        strcat(elm[factorNum],token);
    }
    else if(!strcmp(symbol,"NUM"))
    {
        //@action
        strcpy(elm[factorNum],token);
    }
    else if(!strcmp(symbol,"SIG_QUATATION"))
    {
        //@action
        tempNum = token[1] - '\0';
        itoa(tempNum,tempchar,10);
        addReg();
        strcpy(dTBL[++dTBL_length].name,tempReg);
        li("$t3",tempNum);
        itoa(dTBL_length * 4,tempchar,10);
        strcat(tempchar,"($s5)");
        storeData(tempchar,"$t3");
        strcpy(elm[factorNum],tempReg);
        expressType = 2;
    }
    else if(!strcmp(token,"("))
    {
        paranInStack();
        getsym();
        if(expression() == -1)
        {
            return -1;
        }
        getsym();
        if(!!strcmp(token,")"))
        {
            error(6);
            ungets(token);
        }
        paranOutStack();

        //@action
        strcpy(elm[factorNum],elm[1]);
    }
    return 1;
}

int situationTBL(char caseNum[])
{
    char tempChar[20];
    char tempSig[4];
    char judge[20];
    //�����浱ǰ���������
    addReg();
    emit("=",tempReg,caseNum," ");
    li("$s6",0);
    strcpy(dTBL[++dTBL_length].name,tempReg);
    strcpy(judge,tempReg);
    if(!!strcmp(token,"case"))
    {
        error(19);
        nextstatementRead();
        return -1;
    }
    while(!strcmp(token,"case"))
    {
        getsym();
        strcpy(tempChar,"");
        if(!strcmp(token,"+") || !strcmp(token,"-")){
            strcpy(tempChar,token);
            getsym();
        }
        if(strcmp(symbol,"SIG_QUATATION") && strcmp(symbol,"NUM"))
        {
            error(20);
            nextlineRead();
            return -1;
        }
        if(!strcmp(symbol,"SIG_QUATATION")){
            itoa(token[1] - '\0',tempSig,10);
            strcat(tempChar,tempSig);
        }
        else{
            strcat(tempChar,token);
        }

        //@action
        emit("=="," ",judge,tempChar);
        conditionStack[++conditionStack_length] = fTBL_length;
        emit("!="," ","-1","$s6");
        conditionStack[++conditionStack_length] = fTBL_length;

        getsym();
        if(!!strcmp(token,":"))
        {
            error(21);
            nextlineRead();
            return -1;
        }
        getsym();
        if(sentence() == -1)
        {
            return -1;
        }
        getsym();
        emit("=","$s6","-1"," ");
        genlabel();
        strcpy(tempChar,fTBL[fTBL_length].op);
        tempChar[strlen(tempChar) - 1] = '\0';
        strcpy(fTBL[conditionStack[conditionStack_length--]].result,tempChar);
        strcpy(fTBL[conditionStack[conditionStack_length--]].result,tempChar);
        //caseNum++;

    }
    ungets(token);

    return 1;
}

int defaultSituation()
{
    char tempChar[FELMSIZE];
    getsym();
    if(!!strcmp(token,":"))
    {
        error(21);
        nextlineRead();
        return -1;
    }
    getsym();
    emit("!="," ","-1","$s6");
    conditionStack[++conditionStack_length] = fTBL_length;
    if(sentence() == -1)
    {
        return -1;
    }

    genlabel();
    strcpy(tempChar,fTBL[fTBL_length].op);
    tempChar[strlen(tempChar) - 1] = '\0';
    strcpy(fTBL[conditionStack[conditionStack_length--]].result,tempChar);
    return 1;
}

int sentence()
{
    int temp_vTBL_addr;
    int temp_label_fTBL;
    char tempChar[50];
    int testtest = -3;
    if(!strcmp(symbol,"IFSYM"))           //�������
    {
        getsym();
        if(!!strcmp(symbol,"LPARANSYM"))  //���if���治�������ţ�����������һ��
        {
            error(5);
            nextlineRead();
            paranOutStack();
            return -2;
        }
        paranInStack();

        getsym();
        if(condition() == -1)
        {
            return -2;
        }
        getsym();
        if(!!strcmp(symbol,"RPARANSYM"))
        {
            error(6);
            paranOutStack();
            nextlineRead();
            ungets(token);
        }
        paranOutStack();

        getsym();
        inBranch = 1;
        if(sentence() == -1)
        {
            return -2;
        }
        inBranch = 0;

        getsym();
        jump("j","");
        temp_label_fTBL = fTBL_length;
        unconditionStack[++unconditionStack_length] = fTBL_length;
        //@action ��ת
        genlabel();

        strcpy(tempChar,fTBL[fTBL_length].op);
        tempChar[strlen(tempChar) - 1] = '\0';
        //����ط����ص�result�������beq֮��
        strcpy(fTBL[conditionStack[conditionStack_length--]].result,tempChar);


        if(!strcmp(symbol,"ELSESYM"))
        {
            getsym();
            inBranch = 1;

            if(sentence() == -1)
            {
                return -2;
            }
            inBranch = 0;
            //�������label��ָ�봫��ȥ
            //@action ��ת
            genlabel();
            strcpy(tempChar,fTBL[fTBL_length].op);
            tempChar[strlen(tempChar) - 1] = '\0';
            strcpy(fTBL[unconditionStack[unconditionStack_length--]].elm1,tempChar);
            //fTBL[unconditionStack[unconditionStack_length+1]].result[strlen(fTBL[unconditionStack[unconditionStack_length+1]].result) - 1] = '\0';
        }

        else
        {
            ungets(token);
            //@action
            strcpy(fTBL[unconditionStack[unconditionStack_length--]].elm1,tempChar);

        }



    }

    else if(!strcmp(symbol,"WHILESYM"))   //ѭ��ָ��
    {
        //@action
        genlabel();
        temp_label_fTBL = fTBL_length;
        getsym();
        if(!!strcmp(token,"("))
        {
            error(5);
            nextlineRead();
            paranOutStack();
            return -2;
        }

        paranInStack();

        getsym();
        condition();
        getsym();
        if(!!strcmp(token,")"))
        {
            error(6);
            ungets(token);
        }
        paranOutStack();

        getsym();
        inBranch = 1;

        if(sentence() == -1)
        {
            return -2;
        }
        inBranch = 0;
        //@action
        strcpy(tempChar,fTBL[temp_label_fTBL].op);
        tempChar[strlen(tempChar) - 1] = '\0';
        jump("j",tempChar);
        genlabel();
        strcpy(fTBL[conditionStack[conditionStack_length--]].result,fTBL[fTBL_length].op);
        fTBL[conditionStack[conditionStack_length+1]].result[strlen(fTBL[conditionStack[conditionStack_length+1]].result) - 1] = '\0';
    }

    else if(!strcmp(symbol,"IDENTSYM"))      //�������û��߸�ֵ���////////////////////////////////////////////////////////////////////////
    {
        char _array[FELMSIZE];
        initArray(_array);
        strcpy(tempChar,token);
        _array[0] = '[';
        getsym();
        if(!strcmp(token,"["))       //����
        {
            brackInStack();
            //strcat(tempChar,token);
            getsym();
            expression();

            //strcat(tempChar,elm[1]);
            getsym();
            if(!!strcmp(token,"]"))
            {
                error(8);
                nextlineRead();
                brackOutStack();
                return -2;
            }
            brackOutStack();
            //strcat(tempChar,token);
            temp_vTBL_addr = lookUp(tempChar,currentlevel);
            if(temp_vTBL_addr == -1)
            {
                temp_vTBL_addr = lookUpGlobal(tempChar,0);
                if(temp_vTBL_addr == -1){
                    error(29);
                    nextlineRead();
                    return -2;
                }
                else{
                    strcat(_array,elm[1]);
                    strcat(_array,"]");
                    strcat(_array,tempChar);
                    strcpy(tempChar,_array);
                }
            }
            else{
                strcat(_array,elm[1]);
                strcat(_array,"]");
                strcat(_array,tempChar);
                strcpy(tempChar,_array);
            }
            getsym();
        }
        else if(!strcmp(token,"("))      //��������
        {
            //ע�⣡���������������˼ǵø�1024�����ҵĸ��ƴ��룡
            int temp = 16;//������ջƫ��
            int amount = 0;
            char addr[FELMSIZE];
            char rStackaddr[FELMSIZE];//Ϊ�˼�������ջ�е�����
            char tempLabel[FELMSIZE];

            temp_vTBL_addr = lookUpGlobal(tempChar,0);
            if(temp_vTBL_addr == -1)
            {
                error(29);
                nextlineRead();
                return -2;
            }

            paranInStack();
            getsym();

            //@action ������identity�Ǹ��ط���ô�޸ĵ�
            if(strcmp(token,")"))
            {
                ungets(token);
                do
                {
                    getsym();
                    if(expression() == -1)
                    {
                        return -2;
                    }
                    //�ҵ�ƫ��λ��
                    amount++;//��������++
                    itoa(temp,addr,10);
                    strcat(addr,"($sp)");

                    if(isNum(elm[1]) == 1)
                    {
                        li("$t0",atoi(elm[1]));
                        storeData(addr,"$t0");
                    }
                    else if(lookUp(elm[1],currentlevel) != -1)
                    {
                        itoa(vTBL[lookUp(elm[1],currentlevel)].raddr * 4,rStackaddr,10);
                        strcat(rStackaddr,"($s3)");//��ǰ����ջ��
                        loadData(rStackaddr,"$t0");
                        storeData(addr,"$t0");
                    }
                    else if(lookUpGlobal(elm[1],currentlevel) != -1)
                    {
                        itoa(vTBL[lookUpGlobal(elm[1],currentlevel)].raddr * 4,rStackaddr,10);
                        strcat(rStackaddr,"($s2)");//����ջջ��
                        loadData(rStackaddr,"$t0");
                        storeData(addr,"$t0");
                    }
                    else{
                        itoa(lookUpDTBL(elm[1]) * 4,rStackaddr,10);
                        strcat(rStackaddr,"($s5)");//���ݱ�ջ��
                        loadData(rStackaddr,"$t0");
                        storeData(addr,"$t0");
                    }
                    temp += 4;
                    getsym();
                }
                while(!strcmp(token,","));
                if(amount > vTBL[temp_vTBL_addr].amount)
                {
                    error(35);
                    nextlineRead();
                    return -2;
                }
                //@action
                li("$s1",amount * 4);
                findLabel(tempChar,tempLabel);
                jump("jal",tempLabel);
            }
            else{
                //@action
                findLabel(tempChar,tempLabel);
                jump("jal",tempLabel);
            }




            if(!!strcmp(token,")"))
            {
                error(6);
                paranOutStack();
                nextlineRead();
                return -2;
            }
            paranOutStack();
            getsym();
        }
        else{
            temp_vTBL_addr = lookUp(tempChar,currentlevel);
            if(temp_vTBL_addr == -1)
            {
                temp_vTBL_addr = lookUpGlobal(tempChar,0);
                if(temp_vTBL_addr == -1){
                    error(29);
                    nextlineRead();
                    return -2;
                }
            }
        }

        if(!strcmp(symbol,"BECOMESSYM"))
        {
            getsym();
            if(expression() == -1){
                return -2;
            }
            getsym();
            if(strcmp(";",token))
            {
                error(11);
                ungets(token);
            }
            emit("=",tempChar,elm[1]," ");
            //dTBL[lookUp(tempChar)].val = dTBL[lookUp(elm[1])].val;
        }

    }

    else if(!strcmp(symbol,"SCANFSYM"))  //��ָ��        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    {

        getsym();
        if(!!strcmp(token,"("))
        {
            error(5);
            nextlineRead();
            paranOutStack();
            return -2;
        }

        paranInStack();
        getsym();

        if(!strcmp(symbol,"IDENTSYM"))
        {
            if(lookUp(token,currentlevel) == -1){
                if(lookUpGlobal(token,0) == -1){
                    error(29);
                    nextlineRead();
                    return -2;
                }
                else if(vTBL[lookUpGlobal(token,0)].kind != 0){
                    error(38);
                    nextlineRead();
                    return -2;
                }
                else if(vTBL[lookUp(token,currentlevel)].kind != 0){
                    error(38);
                    nextlineRead();
                    return -2;
                }
            }

            scanfAction(token);
        }
        else{
            error(37);
            nextlineRead();
            return -2;
        }

        getsym();
        while(!strcmp(token,","))
        {
            getsym();
            if(!strcmp(symbol,"IDENTSYM"))
            {
                if(lookUp(token,currentlevel) == -1){
                    if(lookUpGlobal(token,0) == -1){
                        error(29);
                        nextlineRead();
                        return -2;
                    }
                }
                scanfAction(token);
            }
            else{
                error(37);
                nextlineRead();
                return -2;
            }
            getsym();
        }

        if(!!strcmp(token,")"))
        {
            error(6);
            ungets(token);
            paranOutStack();
            nextlineRead();
            return -2;
        }
        paranOutStack();

        getsym();
        if(!!strcmp(token,";"))
        {
            error(11);
            ungets(token);
        }
    }

    else if(!strcmp(symbol,"PRINTFSYM"))   //дָ�� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    {
                                    //ע������ط����������Ǳ��ʽ+���ʽ�Ľṹ
        int printfNum = 0;
        getsym();
        if(!!strcmp(token,"("))
        {
            error(5);
            paranOutStack();
            nextlineRead();
            return -2;
        }
        paranInStack();
        getsym();
        if(!strcmp(symbol,"STRING"))
        {
            printfAction(token,0);//0��������Ǹ��ַ���
            getsym();
        }
        else
        {
            if(expression() == -1)
            {
                return -2;
            }
            printfAction(elm[1],1);
            getsym();
            printfNum = 1;
        }

        if(!strcmp(token,","))
        {
            if(printfNum == 1){
                error(39);
                nextlineRead();
                return -2;
            }
            getsym();
            if(expression() == -1)
            {
                return -2;
            }
            printfAction(elm[1],1);
            getsym();
        }

        if(!!strcmp(token,")"))
        {
            error(6);
            ungets(token);
            paranOutStack();
            nextlineRead();
            return -2;
        }
        paranOutStack();


        getsym();
        if(strcmp(token,";"))
        {
            error(11);
            ungets(token);
        }
    }

    else if(!strcmp(symbol,"SWITCHSYM"))  //switchָ��
    {
        getsym();
        if(!!strcmp(token,"("))
        {
            error(5);
            paranOutStack();
            nextstatementRead();
            return -2;
        }
        paranInStack();
        getsym();
        expression();
        strcpy(tempChar,elm[1]);

        //addReg();
        //emit("=",tempReg,tempChar," ");

        getsym();
        if(!!strcmp(token,")"))
        {
            error(6);
            ungets(token);
            paranOutStack();
            nextlineRead();
            return -2;
        }
        paranOutStack();

        getsym();
        if(!!strcmp(token,"{"))
        {
            error(9);

        }
        quaInStack();
        getsym();

        situationTBL(tempChar);

        getsym();
        if(!strcmp(token,"default"))
        {
            defaultSituation();
        }
        else
        {
            ungets(token);
        }

        getsym();
        if(!!strcmp(token,"}"))
        {
            error(10);
        }
        quaOutStack();

    }

    else if(!strcmp(symbol,"RETURNSYM"))  //����ָ��///////////////////////////////////////////////////////////////
    {
        //returnAction(tempChar);
        char getResult[FELMSIZE];//��ñ��ʽ�ķ���ֵ
        if(inBranch == 0){
            returnNeed--;
        }
        getsym();
        if(!strcmp(token,"("))
        {
            if(currentType == 0)
            {
                error(33);
            }
            paranInStack();
            getsym();
            if(expression() == -1)
            {
                return -2;
            }

            //@action load return
            expressionTranslate(elm[1],getResult);
            if(isNum(getResult) == 1)
            {
                li("$s4",atoi(getResult));
            }
            else
            {
                loadData(getResult,"$s4");
            }
            strcpy(tempChar,elm[1]);
            getsym();
            if(strcmp(token,")"))
            {
                error(6);
            }
            paranOutStack();
        }
        else
        {
            if(currentType != 0)
            {
                error(34);
            }
            ungets(token);
        }
        getsym();
        if(strcmp(token,";"))
        {
            error(24);
            nextlineRead();
        }
        returnAction();
    }

    else if(!strcmp(token,"{"))           //�����
    {

        quaInStack();
        getsym();
        if(!!strcmp(token,"}"))  //��䲻Ϊ��
        {
            while(!!strcmp(token,"}"))
            {
                testtest = sentence();
                if(testtest == -1)
                {
                    return -2;
                }
                getsym();
            }
            quaOutStack(); //��ջ�������ǲ���}��Ҫ��ջ����������
        }
        else
        {
            quaOutStack();
        }

    }

    else if(!strcmp(token,";"))           //�����
    {
        ;
    }
    else
    {
        nextlineRead();
        error(27);
    }
    return 1;
}

int variblestatement(char var[],int type)               //��������ʱ�õĺ���
{
    char tempChar[IDENTSIZE];
    //char tempCharArray[IDENTSIZE];
    int tempNum;
    char _tempNum[IDENTSIZE];
    int amount = 0;
    int i = 0;
    int identNum = 0;
    int ifFirst = 0;
    // var[]������Ҫ����ű�ı�������
    strcpy(tempChar,var);
    do
    {
        getsym();
        if(!strcmp(symbol,"IDENTSYM"))          //������������������Ĵ���
        {
            if(ifFirst == 0){
                error(40);
                nextlineRead();
                return -2;
            }
            //���ű��¼
            strcpy(tempChar,token);
            getsym();
        }
        if(!strcmp(token,";"))
        {
            //@action /vTBL
            if(!strcmp(lastToken,",")){
                ungets(token);
                error(38);
                nextlineRead();
                return -1;
            }
            if(lookUp(tempChar,currentlevel) != -1)
            {
                    ungets(token);
                    error(32);
                    nextlineRead();
                    return -1;

            }
            if(currentlevel == 0 && lookUpGlobal(tempChar,0) != -1)
                {
                    ungets(token);
                    error(32);
                    nextlineRead();
                    return -1;
                }
            amount++;
            vTBL_length++;

            strcpy(vTBL[vTBL_length].name,tempChar);
            vTBL[vTBL_length].type = type;
            vTBL[vTBL_length].kind = 0;
            vTBL[vTBL_length].level = currentlevel;
            if(currentlevel != 0){
                identNum++;
                vTBL[vTBL_length].raddr = ++rStack_length;
            }
            else
                vTBL[vTBL_length].raddr = ++global_length;

            itoa(identNum * 4,tempChar,10);
            emit("+","$sp","$sp",tempChar);
            ungets(token);
            return 1;
        }
        else if(!strcmp("[",token))  //����
        {
            char _array[FELMSIZE];
            brackInStack();

            getsym();
            if(!!strcmp("NUM",symbol))
            {
                error(15);
                brackOutStack();
                nextlineRead();
                return -1;
            }
            tempNum = atoi(token);
            if(tempNum < 0)
            {
                error(31);
                nextlineRead();
                return -1;
            }

            getsym();
            if(!!strcmp("]",token))
            {
                error(8);
                brackOutStack();
                nextlineRead();
                return -1;
            }
            brackOutStack();
            //strcpy(tempCharArray,tempChar);
            //@action /vTBL
            if(lookUp(tempChar,currentlevel) != -1)
            {
                //if(lookUpGlobal(tempChar,0) != -1){
                error(32);
                nextlineRead();
                return -1;
                //}
            }

            if(currentlevel == 0 && lookUpGlobal(tempChar,0) != -1)
            {
                //if(lookUpGlobal(tempChar,0) != -1){
                error(32);
                nextlineRead();
                return -1;
                //}
            }
            vTBL_length++;

            strcpy(vTBL[vTBL_length].name,tempChar);
            vTBL[vTBL_length].type = type;
            vTBL[vTBL_length].kind = 2;
            vTBL[vTBL_length].level = currentlevel;
            if(currentlevel != 0)
                vTBL[vTBL_length].raddr = rStack_length + 1;
            else
                vTBL[vTBL_length].raddr = global_length + 1;
            //��������ռ�ڴ�ռ�
            for(i = 0; i <= tempNum; i++)
            {
                //@action /vTBL

                vTBL_length++;
                //strcpy(tempChar,tempCharArray);
                itoa(i,_tempNum,10);
                strcpy(_array,"[");
                strcat(_array,_tempNum);
                strcat(_array,"]");
                strcat(_array,tempChar);

                strcpy(vTBL[vTBL_length].name,_array);
                vTBL[vTBL_length].type = type;
                vTBL[vTBL_length].kind = 0;
                vTBL[vTBL_length].level = currentlevel;
                if(currentlevel != 0){
                    vTBL[vTBL_length].raddr = ++rStack_length;
                    identNum++;
                }
                else{
                    vTBL[vTBL_length].raddr = ++global_length;
                }
                strcpy(_array,"");
            }

            getsym();
        }
        else if(!strcmp(token,","))            //������,��ʱ���������Ƕ���
        {
            ifFirst = 1;
            //@action /vTBL
            if(lookUp(tempChar,currentlevel) != -1)
            {

                error(32);
                nextlineRead();
                return -1;
            }
            if(currentlevel == 0 && lookUpGlobal(tempChar,0) != -1)
            {
                error(32);
                nextlineRead();
                return -1;
            }
            amount++;
            vTBL_length++;

            strcpy(vTBL[vTBL_length].name,tempChar);
            vTBL[vTBL_length].type = type;
            vTBL[vTBL_length].kind = 0;
            vTBL[vTBL_length].level = currentlevel;
            if(currentlevel != 0){
                identNum++;
                vTBL[vTBL_length].raddr = ++rStack_length;
            }
            else
                vTBL[vTBL_length].raddr = ++global_length;
        }
        else
        {
            error(28);
            nextlineRead();
            return -1;
        }
    }
    while(!strcmp(",",token));
    ungets(token);
    itoa(identNum * 4,tempChar,10);
    emit("+","$sp","$sp",tempChar);
    return 1;
}

int identityItemStatement(char var[],int factorNum)         //���Ӵ����ʱ��ı�ʶ��������///////////////////////////////////////////////////////////////////////////////////////////////////////////
{
    char temp_identity[TOKEN_SIZE];
    char ident[50][FELMSIZE];
    char tempNum[20];
    int identNum = 0;
    int i = 0;
    //�հ�sentence����ĺ�������

    int amount = 0;
    int check = 0;
    int raddr = 0;
    int temp_vTBL_addr = 0;
    int ident_addr;
    char addr[FELMSIZE];
    char rStackaddr[FELMSIZE];//Ϊ�˼�������ջ�е�����
    char _tempReg[FELMSIZE];
    char funcReg[FELMSIZE];
    strcpy(temp_identity,var);

    getsym();
    if(!strcmp(token,"("))
    {
        paranInStack();
        //@action
        temp_vTBL_addr = lookUpGlobal(temp_identity,0);

        if(temp_vTBL_addr == -1)
        {
            error(22);
            nextlineRead();
            return -1;
        }
        else if(vTBL[temp_vTBL_addr].type == 0)
        {
            error(30);
            nextlineRead();
            return -1;
        }

        addFunc();
        vTBL_length++;
        strcpy(vTBL[vTBL_length].name,tempFunc);
        vTBL[vTBL_length].type = vTBL[temp_vTBL_addr].type;
        vTBL[vTBL_length].kind = 0;
        vTBL[vTBL_length].level = currentlevel;
        vTBL[vTBL_length].raddr = ++rStack_length;//must in a function
        strcpy(funcReg,tempFunc);
        emit("+","$sp","$sp","4");
        //strcat(temp_identity,token);
        getsym();
        expressType = vTBL[temp_vTBL_addr].type;
        //@action
        //��sp����ջ�����������оֲ�����ռ�Ŀռ���
        li("$s1",0);
        strcpy(addr,"");
        if(!strcmp(token,")"))   //�м�û�в���
        {
            paranOutStack();
            jump("jal",temp_identity);
            //@action
            //strcpy(elm[factorNum],temp_identity);
        }
        else                    //�м��в���
        {
            emit("+","$sp","$sp","12");

            ungets(token);
            do
            {
                getsym();
                if(expression() == -1)
                {
                    return -1;
                }
                amount++;
                if(isNum(elm[1]) == 1)
                {
                    emit("+","$sp","$sp","4");
                    li("$t0",atoi(elm[1]));
                    storeData("0($sp)","$t0");
                    strcpy(ident[identNum],elm[1]);
                    identNum++;

                }
                else if(lookUp(elm[1],currentlevel) != -1)
                {
                    emit("+","$sp","$sp","4");
                    itoa(vTBL[lookUp(elm[1],currentlevel)].raddr * 4,rStackaddr,10);
                    strcat(rStackaddr,"($s3)");//��ǰ����ջ��
                    loadData(rStackaddr,"$t0");
                    storeData("0($sp)","$t0");
                    strcpy(ident[identNum],elm[1]);
                    identNum++;
                }
                else if(lookUpGlobal(elm[1],currentlevel) != -1)
                {
                    emit("+","$sp","$sp","4");
                    itoa(vTBL[lookUpGlobal(elm[1],0)].raddr * 4,rStackaddr,10);
                    strcat(rStackaddr,"($s2)");//����ջջ��
                    loadData(rStackaddr,"$t0");
                    storeData("0($sp)","$t0");
                    strcpy(ident[identNum],elm[1]);
                    identNum++;
                }
                else{
                    emit("+","$sp","$sp","4");
                    itoa(lookUpDTBL(elm[1]) * 4,rStackaddr,10);
                    strcat(rStackaddr,"($s5)");//���ݱ�ջ��
                    loadData(rStackaddr,"$t0");
                    storeData("0($sp)","$t0");
                    strcpy(ident[identNum],elm[1]);
                    identNum++;
                }
                //temp += 4;
                getsym();
            }
            while(!strcmp(token,","));
            if(amount != vTBL[temp_vTBL_addr].amount)
            {
                error(35);
                nextlineRead();
                return -1;
            }

            itoa(amount * 4 + 12,addr,10);
            emit("-","$sp","$sp",addr);

            //input the numbers in the rStack
            for(i = 0;i < identNum; i++){
                if(isNum(ident[i]) == 1){
                    itoa(i * 4 + 16,tempNum,10);
                    strcat(tempNum,"($sp)");
                    storeData(tempNum,ident[i]);
                }
                else if(ident[i][0] == '@'){
                    itoa(i * 4 + 16,tempNum,10);
                    strcat(tempNum,"($sp)");
                    itoa(lookUpDTBL(ident[i]) * 4,_tempReg,10);
                    strcat(_tempReg,"($s5)");
                    loadData(_tempReg,"$t0");
                    storeData(tempNum,"$t0");
                }
                else{
                    if(lookUp(ident[i],currentlevel) != -1)
                    {
                        itoa(i * 4 + 16,tempNum,10);
                        strcat(tempNum,"($sp)");
                        itoa(vTBL[lookUp(ident[i],currentlevel)].raddr * 4,_tempReg,10);
                        strcat(_tempReg,"($s3)");//��ǰ����ջ��
                        loadData(_tempReg,"$t0");
                        storeData(tempNum,"$t0");
                    }
                    else if(lookUpGlobal(ident[i],0) != -1)
                    {
                        itoa(i * 4 + 16,tempNum,10);
                        strcat(tempNum,"($sp)");
                        itoa(vTBL[lookUpGlobal(ident[i],currentlevel)].raddr * 4,_tempReg,10);
                        strcat(_tempReg,"($s2)");//����ջջ��
                        loadData(_tempReg,"$t0");
                        storeData(tempNum,"$t0");
                    }
                }
            }
            li("$s1",amount * 4);
            jump("jal",temp_identity);
            if(!strcmp(token,")"))
            {
                paranOutStack();
                //strcat(temp_identity,token);
            }
            else
            {
                error(6);
                paranOutStack();
            }
            //currentlevel = _currentlevel;
        }
        strcpy(elm[factorNum],funcReg);
        currentItem = 0;

    }
    else if(!strcmp("[",token))  //����
    {
        char _array[FELMSIZE];
        char _ident[FELMSIZE];
        strcpy(_array,"[");
        brackInStack();
        getsym();
        if(expression() == -1)
        {
            return -1;
        }

        check = 1;
        addReg();
        strcpy(dTBL[++dTBL_length].name,tempReg);
        strcpy(funcReg,tempReg);

        strcat(_array,elm[1]);
        getsym();
        if(!!strcmp("]",token))
        {
            error(8);
            brackOutStack();
        }
        brackOutStack();
        strcat(_array,"]");

        //@action
        temp_vaddr = lookUp(temp_identity,currentlevel);
        if(temp_vaddr == -1)
        {
            temp_vaddr = lookUpGlobal(temp_identity,0);
            if(temp_vaddr == -1){
                error(29);
                nextlineRead();
                return -1;
            }
        }
        expressType = vTBL[temp_vaddr].type;
        strcpy(_ident,temp_identity);
        strcat(_array,temp_identity);
        strcpy(temp_identity,_array);
        if(check == 1){
            if(elm[1][0] != '@'){//����м����������ʱ����
                raddr = lookUp(_ident,currentlevel);
                if(raddr == -1){//������ȫ�ֵ�
                    raddr = lookUpGlobal(_ident,0);
                    if(isNum(elm[1]) == 1){
                        itoa(4 * atoi(elm[1]) + 4 * vTBL[raddr].raddr,addr,10);
                        strcat(addr,"($s2)");
                        loadData(addr,"$t0");
                    }
                    else{//����Ǳ���������Ҫ�Ҿֲ�������ȫ�ֱ�������
                        ident_addr = lookUp(elm[1],currentlevel);
                        if(ident_addr == -1){
                            ident_addr = lookUpGlobal(elm[1],0);
                            itoa(vTBL[ident_addr].raddr * 4,addr,10);
                            strcat(addr,"($s2)");
                            loadData(addr,"$t0");
                            sll("sll","$t0","$t0","2");
                            emit("+","$t0","$t0","$s2");//����$s2���������ڵ�λ��
                            li("$t1",vTBL[raddr].raddr * 4);
                            emit("+","$t0","$t0","$t1");
                            loadData("0($t0)","$t0");
                        }
                        else{
                            itoa(vTBL[ident_addr].raddr * 4,addr,10);
                            strcat(addr,"($s3)");
                            loadData(addr,"$t0");
                            sll("sll","$t0","$t0","2");
                            emit("+","$t0","$t0","$s2");//����$s3���������ڵ�λ��
                            li("$t1",vTBL[raddr].raddr * 4);
                            emit("+","$t0","$t0","$t1");
                            loadData("0($t0)","$t0");
                        }
                    }
                    itoa(lookUpDTBL(funcReg) * 4,addr,10);
                    strcat(addr,"($s5)");
                    storeData(addr,"$t0");
                    strcpy(elm[factorNum],funcReg);
                    currentItem = 1;
                }
                else{
                    if(isNum(elm[1]) == 1){
                        itoa(4 * atoi(elm[1]) + 4 * vTBL[raddr].raddr,addr,10);
                        strcat(addr,"($s3)");
                        loadData(addr,"$t0");
                    }
                    else{//����Ǳ���������Ҫ�Ҿֲ�������ȫ�ֱ�������
                        ident_addr = lookUp(elm[1],currentlevel);
                        if(ident_addr == -1){
                            ident_addr = lookUpGlobal(elm[1],0);
                            itoa(vTBL[ident_addr].raddr * 4,addr,10);
                            strcat(addr,"($s2)");
                            loadData(addr,"$t0");
                            sll("sll","$t0","$t0","2");
                            emit("+","$t0","$t0","$s3");//����$s3���������ڵ�λ��
                            li("$t1",vTBL[raddr].raddr * 4);
                            emit("+","$t0","$t0","$t1");
                            loadData("0($t0)","$t0");
                        }
                        else{
                            itoa(vTBL[ident_addr].raddr * 4,addr,10);
                            strcat(addr,"($s3)");
                            loadData(addr,"$t0");
                            sll("sll","$t0","$t0","2");
                            emit("+","$t0","$t0","$s3");//����$s3���������ڵ�λ��
                            li("$t1",vTBL[raddr].raddr * 4);
                            emit("+","$t0","$t0","$t1");
                            loadData("0($t0)","$t0");
                        }
                    }

                    itoa(lookUpDTBL(funcReg) * 4,addr,10);
                    strcat(addr,"($s5)");
                    storeData(addr,"$t0");
                    strcpy(elm[factorNum],funcReg);
                    currentItem = 1;
                }
            }
            else{
                itoa(lookUpDTBL(elm[1]) * 4,addr,10);
                strcat(addr,"($s5)");
                loadData(addr,"$t0");
                sll("sll","$t0","$t0","2");

                raddr = lookUp(_ident,currentlevel);
                if(raddr == -1){
                    raddr = lookUpGlobal(_ident,0);
                    emit("+","$t0","$t0","$s2");//����$s3���������ڵ�λ��
                    li("$t1",vTBL[raddr].raddr * 4);
                    emit("+","$t0","$t0","$t1");
                    loadData("0($t0)","$t0");
                }
                else{

                    emit("+","$t0","$t0","$s3");//����$s3���������ڵ�λ��
                    li("$t1",vTBL[raddr].raddr * 4);
                    emit("+","$t0","$t0","$t1");
                    loadData("0($t0)","$t0");
                }
                itoa(lookUpDTBL(funcReg) * 4,addr,10);
                strcat(addr,"($s5)");
                storeData(addr,"$t0");
                strcpy(elm[factorNum],funcReg);
                currentItem = 1;
            }

        }

        else
            strcpy(elm[factorNum],temp_identity);

        currentItem = 1;
    }
    else
    {
        if(lookUp(var,currentlevel) == -1){
            if(lookUpGlobal(var,currentlevel) != -1){
                ungets(token);
                //@action
                strcpy(elm[factorNum],temp_identity);
            }
            else{
                error(29);
                nextlineRead();
                return -1;
            }
        }
        else{
            ungets(token);
                //@action
                strcpy(elm[factorNum],temp_identity);
        }

    }
    return 1;
}

int conststatement()                   //��ȥ֮����Լ���
{
    int identNum = 0;
    int startSignal = 0;
    char tempChar[30];
    char tempNum[5];
    char tempReg[20];
    int amount = 0;
    int num;
    do
    {
        getsym();
        if(!strcmp(symbol,"INTSYM"))  //�����int����
        {

            do
            {
                getsym(); //������һ������������ǲ���������ű����򱨴�
                if(!!strcmp(symbol,"IDENTSYM"))
                {
                    error(13);
                    nextlineRead();
                    return -1;
                }

                strcpy(tempChar,token);
                getsym();
                if(!!strcmp("=",token))
                {
                    error(14);
                    nextlineRead();
                    return -1;
                }
                getsym();
                if(!strcmp(token,"+") || !strcmp(token,"-"))  //+-2��+-0֮�������
                {
                    if(!strcmp(token,"+"))
                    {
                        startSignal = 1;
                    }
                    getsym();
                    if(!!strcmp("NUM",symbol))
                    {
                        error(15);
                        nextlineRead();
                        return -1;
                    }
                    else if(token[0] == '0')
                    {
                        error(17);
                        nextlineRead();
                        return -1;
                    }
                    if(startSignal == 0)
                    {
                        tempNum[0] = '-';
                        strcat(tempNum,token);
                    }
                    else
                    {
                        strcpy(tempNum,token);
                    }
                }
                else
                {
                    if(!!strcmp("NUM",symbol))
                    {
                        error(15);
                        nextlineRead();
                        return -1;
                    }
                    strcpy(tempNum,token);
                }

                //@action
                if(lookUp(tempChar,currentlevel) != -1)
                {

                    error(32);
                    nextlineRead();
                    return -1;

                }

                vTBL_length++;
                strcpy(vTBL[vTBL_length].name,tempChar);
                vTBL[vTBL_length].type = 1;
                vTBL[vTBL_length].kind = 1;
                vTBL[vTBL_length].level = currentlevel;
                if(currentlevel == 0)
                {
                    vTBL[vTBL_length].raddr = ++global_length;
                    itoa(global_length * 4,tempReg,10);
                    strcat(tempReg,"($s2)");
                    storeData(tempReg,tempNum);
                }

                else
                {
                    vTBL[vTBL_length].raddr = ++rStack_length;
                    itoa(rStack_length * 4,tempReg,10);
                    strcat(tempReg,"($s3)");
                    storeData(tempReg,tempNum);
                    identNum++;
                }



                getsym();
                amount++;
            }
            while(!strcmp(token,","));

            if(!!strcmp(token,";"))
            {
                error(11);
                ungets(token);
                nextlineRead();
                return -1;
            }
        }
        else if(!strcmp(symbol,"CHARSYM"))
        {
            do
            {
                getsym();
                if(!!strcmp(symbol,"IDENTSYM"))
                {
                    error(13);
                    nextlineRead();
                    return -1;
                }

                strcpy(tempChar,token);
                getsym();
                if(!!strcmp("=",token))
                {
                    error(14);
                    nextlineRead();
                    return -1;
                }
                getsym();
                if(!!strcmp(symbol,"SIG_QUATATION"))
                {
                    error(16);
                    nextlineRead();
                    return -1;
                }
                token[0] = token[1];
                token[1] = '\0';
                token[2] = '\0';
                num = token[0] - '\0';
                itoa(num,tempNum,10);

                //@action
                if(lookUp(tempChar,currentlevel) != -1)
                {
                    error(32);
                    nextlineRead();
                    return -1;
                }
                vTBL_length++;

                //strcpy(dTBL[dTBL_length].name,tempChar);

                strcpy(vTBL[vTBL_length].name,tempChar);
                vTBL[vTBL_length].type = 2;
                vTBL[vTBL_length].kind = 1;
                vTBL[vTBL_length].level = currentlevel;

                if(currentlevel == 0)
                {
                    vTBL[vTBL_length].raddr = ++global_length;
                    itoa(global_length * 4,tempReg,10);
                    strcat(tempReg,"($s2)");
                    storeData(tempReg,tempNum);
                }

                else
                {
                    vTBL[vTBL_length].raddr = ++rStack_length;
                    itoa(rStack_length * 4,tempReg,10);
                    strcat(tempReg,"($s3)");
                    storeData(tempReg,tempNum);
                    identNum++;
                }
                getsym();
                amount++;
            }
            while(!strcmp(token,","));
            if(!!strcmp(token,";"))
            {
                error(11);
                ungets(token);
                nextlineRead();
                return -1;
            }
        }
        else
        {
            error(12);
            nextlineRead();
            return -1;
        }
        getsym();
    }
    while(!strcmp(symbol,"CONSTSYM"));
    ungets(token);
    ///////////////////////////////////////////////////////////////////////////////////////
    itoa(identNum * 4,tempNum,10);
    emit("+","$sp","$sp",tempNum);
    return 1;
}

int complexSentence()
{
    int tempType = 1;
    if(!strcmp(symbol,"CONSTSYM"))
    {
        if(period_func > 1)
        {
            error(23);
            nextlineRead();
            return -1;
        }
        period_func = 1;
        if(conststatement() == -1)
        {
            return -1;
        }
        //printf("This is a const declaration in a function\n");
        getsym();
    }

    if(!strcmp(symbol,"INTSYM") || !strcmp(symbol,"CHARSYM"))
    {
        if(!strcmp(symbol,"CHARSYM"))
        {
            tempType = 2;
        }
        else{
            tempType = 1;
        }
        if(period_func > 2)
        {
            error(23);
            nextlineRead();
            return -1;
        }
        period_func = 2;

        do
        {
            if(!strcmp(token,"char"))
            {
                tempType = 2;
            }
            else{
                tempType = 1;
            }
            getsym();
            strcpy(varible,token);

            if(variblestatement(varible,tempType) == -2)
            {
                return -2;
            }

            getsym();
            if(strcmp(token,";"))
            {
                error(11);
                nextlineRead();
            }
            getsym();
        }
        while(!strcmp(token,"char") || !strcmp(token,"int"));

    }
    period_func = 3;

    while(strcmp(token,"}"))
    {
        if(sentence() == -1)
        {
            return -1;
        }
        getsym();
    }

    ungets(token);
    return 1;
}

int statement()
{
    char tempChar[IDENTSIZE];
    int tempType = 1;
    int amount = 0;     //���㺯�������м�������
    int elmAmount = 0;
    if(!strcmp(symbol,"CONSTSYM"))
    {
        if(period > 1)
        {
            error(22);
            nextlineRead();
            return -1;
        }
        period = 1;             //����������һ�׶�
        if(conststatement() == -1)
        {
            return -1;
        };
        //printf("This is a const declaration\n");
    }
    else if(!strcmp(symbol,"VOIDSYM")) // void����
    {
        if(ifGlobal == 0)
        {
            ifGlobal = 1;
            jump("j","main");      //mips����ʼ
        }

        getsym();
        if(!strcmp(symbol,"MAINSYM"))
        {
            strcpy(currentFunc,"main");
            ifHasReturn = 0;
            currentType = 0;
            returnNeed = 1;
            rStack_length = -1;
            strcpy(tempChar,token);
            period = 4;         //��������Ľ׶�
            getsym();
            if(!!strcmp(token,"("))
            {
                error(5);
                ungets(token);
                nextstatementRead();
                period = 3;             //�������������׶�
                return -1;
            }
            getsym();
            if(!!strcmp(token,")"))
            {
                error(6);
                ungets(token);
                nextstatementRead();
                paranOutStack();
                return -1;
            }
            getsym();
            if(!!strcmp(token,"{"))
            {
                error(9);
                ungets(token);
                nextstatementRead();
                return -1;
            }
            quaInStack();
            period_func = 0; //����������ʼ��
            currentlevel++;

            //@action
            genFuncLabel("main");

            vTBL_length++;
            amount++;
            strcpy(vTBL[vTBL_length].name,tempChar);
            vTBL[vTBL_length].type = 0;
            vTBL[vTBL_length].kind = 3;
            vTBL[vTBL_length].level = 0;
            vTBL[vTBL_length].raddr = ++global_length;
            vTBL[vTBL_length].flevel = currentlevel;
            getsym();
            period_func = 0;

            //@action
            emit("+","$sp","$sp","16");//sp��Ŀǰջ��
            _move("$s3","$sp");
            emit("-","$sp","$sp","4");

            if(complexSentence() == -2)
            {
                return -2;
            }
            getsym();
            if(!!strcmp(token,"}"))
            {
                error(10);
                ungets(token);
                nextstatementRead();
                quaOutStack();
                return -1;
            }

            //printf("This is a main declaration\n");
            return 1;
        }
        else if(strcmp(symbol,"IDENTSYM"))
        {
            error(13);
            nextlineRead();
            return -1;
        }

        //@action
        strcpy(tempChar,token);

        getsym(); //�ж�����Ǻ������Ǳ���
        if(!strcmp("(",token))  //void ���ͺ���
        {
            amount = 0;
            elmAmount = 0;
            currentType = 0;
            returnNeed = 1;
            rStack_length = -1;
            if(period > 3)
            {
                error(22);
                nextstatementRead();
                paranOutStack();
                return -1;
            }
            period = 3;             //�������������׶�
            currentlevel++;

            //@action
            genFuncLabel(tempChar);     //���ɺ���label
            ifHasReturn = 0;
            acceptCall();

            paranInStack();
            getsym();
            if(!strcmp(token,")"))
            {
                paranOutStack();
            }
            else                        //�����д��ݵĲ���
            {
                ungets(token);
                do
                {
                    getsym();
                    if(strcmp(symbol,"INTSYM") && strcmp(symbol,"CHARSYM"))
                    {
                        error(12);
                        nextstatementRead();
                        return -1;
                    }
                    if(!strcmp(symbol,"CHARSYM"))
                    {
                        tempType = 2;
                    }


                    getsym();
                    if(!!strcmp(symbol,"IDENTSYM"))
                    {
                        error(13);
                        nextstatementRead();
                        paranOutStack();
                        return -1;
                    }

                    //@action
                    if(lookUp(token,currentlevel) != -1)
                    {
                        error(32);
                        nextlineRead();
                        return -1;
                    }
                    vTBL_length++;
                    elmAmount++;
                    strcpy(vTBL[vTBL_length].name,token);
                    vTBL[vTBL_length].type = tempType;
                    vTBL[vTBL_length].kind = 0;
                    vTBL[vTBL_length].level = currentlevel;
                    vTBL[vTBL_length].raddr = ++rStack_length;
                    getsym();
                }
                while(!strcmp(token,","));

                if(!strcmp(token,"["))
                {
                    error(25);
                    brackOutStack();
                    nextstatementRead();
                    return -1;
                }
                if(!!strcmp(token,")"))
                {
                    error(6);
                }
                paranOutStack();
                getsym();
                if(strcmp(token,"{"))
                {
                    error(10);
                    nextstatementRead();
                    return -1;
                }
                else
                {
                    ungets(token);
                }
            }
            //printf("This is a void function declaration\n");
            period_func = 0;

            //@action
            if(lookUpGlobal(tempChar,0) != -1)
            {
                error(32);
                nextstatementRead();
                return -1;
            }
            vTBL_length++;

            strcpy(vTBL[vTBL_length].name,tempChar);
            vTBL[vTBL_length].type = tempType;
            vTBL[vTBL_length].kind = 3;
            vTBL[vTBL_length].level = 0;
            vTBL[vTBL_length].raddr = ++global_length;
            vTBL[vTBL_length].amount = elmAmount;
            vTBL[vTBL_length].flevel = currentlevel;

        }
        else{
            error(5);
            nextstatementRead();
            return -1;
        }
    }
    else if(!strcmp(symbol,"INTSYM"))  // int
    {
        getsym();
        if(!!strcmp(symbol,"IDENTSYM"))
        {
            error(13);
            nextlineRead();
            return -1;
        }
        else
        {
            strcpy(varible,token);
            strcpy(tempChar,token);
        }
        getsym();
        if(!strcmp("(",token))  //int ���ͺ���
        {
            if(ifGlobal == 0)
            {
                ifGlobal = 1;
                jump("j","main");      //mips����ʼ
            }
            currentType = 1;
            returnNeed = 1;
            rStack_length = -1;
            currentlevel++;
            ifHasReturn = 0;
            //@action
            genFuncLabel(tempChar);     //���ɺ���label
            acceptCall();

            if(period > 3)
            {
                error(22);
                paranOutStack();
                return -1;
            }
            period = 3;
            period_func = 0;
            paranInStack();
            getsym();
            if(!strcmp(token,")"))
            {
                paranOutStack();
                //printf("This is an int declaration\n");
            }
            else                        //�����д��ݵĲ���
            {
                ungets(token);
                do
                {
                    getsym();
                    if(!!strcmp(symbol,"INTSYM") && !!strcmp(symbol,"CHARSYM"))
                    {
                        error(12);
                        nextstatementRead();
                        paranOutStack();
                        return -1;
                    }
                    if(!strcmp(symbol,"CHARSYM"))
                    {
                        tempType = 2;
                    }
                    getsym();
                    if(!!strcmp(symbol,"IDENTSYM"))
                    {
                        error(13);
                        nextstatementRead();
                        paranInStack();
                        return -1;
                    }

                    //@action
                    if(lookUp(token,currentlevel) != -1)
                    {
                        error(32);
                        nextlineRead();
                        return -1;
                    }
                    vTBL_length++;
                    elmAmount++;
                    strcpy(vTBL[vTBL_length].name,token);
                    vTBL[vTBL_length].type = tempType;
                    vTBL[vTBL_length].kind = 0;
                    vTBL[vTBL_length].level = currentlevel;
                    vTBL[vTBL_length].raddr = ++rStack_length;

                    getsym();
                }
                while(!strcmp(token,","));
                ///////////////////////////����ط���Ҫô����///////////////////////////////////////////////////////////////////////
                if(!strcmp(token,"["))
                {
                    error(25);
                    brackOutStack();
                    nextstatementRead();
                    return -1;
                }
                //���������в����ܳ�������
                if(!!strcmp(token,")"))
                {
                    error(6);
                }
                paranOutStack();
            }
            period_func = 0;
            //printf("This is an int function declaration\n");

            //@action
            if(lookUpGlobal(tempChar,0) != -1)
            {
                error(32);
                nextlineRead();
                return -1;
            }

            vTBL_length++;
            strcpy(vTBL[vTBL_length].name,tempChar);
            vTBL[vTBL_length].type = 1;
            vTBL[vTBL_length].kind = 3;
            vTBL[vTBL_length].level = 0;
            vTBL[vTBL_length].raddr = ++global_length;
            vTBL[vTBL_length].amount = elmAmount;
            vTBL[vTBL_length].flevel = currentlevel;
            period_func = 0;


        }
        else  //int���ͱ�������
        {
            //if(!strcmp(token,"[")){
            //    ungets(token);  //�����������'['
            //}
            ungets(token);
            period = 2;
            if(variblestatement(varible,1) == -1)
            {
                return -1;
            }
            getsym();
            if(strcmp(token,";"))
            {
                error(11);
                nextlineRead();
                return -1;
            }
            //printf("This is an int varible declaration\n");

        }
    }
    else if(!strcmp(symbol,"CHARSYM"))  //char
    {
        getsym();
        if(!!strcmp(symbol,"IDENTSYM"))
        {
            error(13);
            nextstatementRead();
            return -1;
        }
        else
        {
            strcpy(varible,token);
            strcpy(tempChar,token);
        }
        getsym();
        if(!strcmp("(",token))  //char ���ͺ���
        {
            if(ifGlobal == 0)
            {
                ifGlobal = 1;
                jump("j","main");      //mips����ʼ
            }
            currentType = 2;
            returnNeed = 1;
            rStack_length = -1;
            currentlevel++;
            ifHasReturn = 0;

            //@action
            genFuncLabel(tempChar);     //���ɺ���label
            acceptCall();

            if(period > 3)
            {
                error(23);
                paranOutStack();
            }
            period = 3;
            paranInStack();
            getsym();
            if(!strcmp(token,")"))
            {
                paranOutStack();
                //printf("This is a char function declaration\n");
            }
            else                        //�����д��ݵĲ���
            {
                ungets(token);
                do
                {
                    getsym();
                    if(!!strcmp(symbol,"INTSYM") && !!strcmp(symbol,"CHARSYM"))
                    {
                        error(12);
                        nextstatementRead();
                        paranOutStack();
                        return -1;
                    }
                    getsym();
                    if(!!strcmp(symbol,"IDENTSYM"))
                    {
                        error(13);
                        nextstatementRead();
                        paranInStack();
                        return -1;
                    }

                    //@action
                    if(lookUp(token,currentlevel) != -1)
                    {
                        error(32);
                        nextlineRead();
                        return -1;
                    }
                    vTBL_length++;
                    elmAmount++;
                    strcpy(vTBL[vTBL_length].name,token);
                    vTBL[vTBL_length].type = tempType;
                    vTBL[vTBL_length].kind = 0;
                    vTBL[vTBL_length].level = currentlevel;
                    vTBL[vTBL_length].raddr = ++rStack_length;
                    getsym();
                }
                while(!strcmp(token,","));
                if(!strcmp(token,"["))
                {
                    error(25);
                    brackOutStack();
                    nextstatementRead();
                    return -1;
                }
                if(!!strcmp(token,")"))
                {
                    error(6);
                }
                paranOutStack();
                //printf("This is a char function declaration\n");
            }

            //@action
            if(lookUpGlobal(tempChar,0) != -1)
            {
                error(32);
                nextlineRead();
                return -1;
            }
            vTBL_length++;

            strcpy(vTBL[vTBL_length].name,tempChar);
            vTBL[vTBL_length].type = 2;
            vTBL[vTBL_length].kind = 3;
            vTBL[vTBL_length].level = 0;
            vTBL[vTBL_length].raddr = ++global_length;
            vTBL[vTBL_length].amount = elmAmount;
            vTBL[vTBL_length].flevel = currentlevel;
            period_func = 0;


        }
        else                //char ���ͱ�������
        {
            //if(!strcmp(token,"[")){
            //    ungets(token);
            //}
            ungets(token);
            if(variblestatement(varible,2) == -1)
            {
                return -1;
            }
            getsym();
            if(strcmp(token,";"))
            {
                error(11);
                nextlineRead();
                return -1;
            }
            //printf("This is a char varible declaration\n");
        }
    }
    else
    {
        if(period >= 3)
        {
            quaInStack();
            getsym();
            if(complexSentence() == -1)
            {
                return -1;
            }
            getsym();
        }
        if(strcmp(token,"}"))
        {
            if(period >= 3){
                error(9);
                quaOutStack();
            }
            else{
                error(26);
                nextstatementRead();
                return -1;
            }
        }
        else if(!strcmp(token,"}"))
        {
            quaOutStack();
            return 1;
        }
        else
        {
            error(26);
            nextlineRead();
            return -1;
        }
    }
    return 1;
}


#endif // STATEMENT_H_INCLUDED
