#include <iostream>
#include <direct.h>
#include <vector>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;
#define MaxSize 100

//ÿһ����������ʽ�����ݽṹ
typedef struct {
    string element[MaxSize]; //ÿһ����|���ָ����ĵ���ʽ�ӵļ���
    int length;//������������ʽ�ӵ�����
} Productions;
typedef struct{
    char elem[MaxSize];
    int length;
    bool done;
}First,Follow;

typedef struct {
    char Non[MaxSize];//���ս��
    int NonNum;//���ս������
    int TerNum;//�ս������
    char Ter[MaxSize];//�ս��
    Productions Produ[MaxSize]; //����ʽ�����޼��� ˳������ս��˳��һһ��Ӧ
    int producNum;//����ʽ����
    char start; //�ķ���ʼ����
    First first[MaxSize]; //��ά���� ��Ӧÿ�����ս����first/follow
    Follow follow[MaxSize];
    string predAnal[MaxSize][MaxSize];
} CFG;

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}
//��char�������ж��ض�Ԫ���Ƿ����
int isExist(char c,char chars[MaxSize],int length){
    for (int i = 0; i < length; ++i) {
        if(chars[i] == c)
            return i;
    }
    return -1;
}
int isExist(char c,string s){
    for (int i = 0; i < s.size(); ++i) {
        if(s[i] == c)
            return i;
    }
    return -1;
}
//������֪���ĵ����Ĳ���ʽ���������еĵ������ӣ�
void saveNewProduction(CFG &cfg, string s,char start){
    //�Ƚ�ʽ�����³��ֵ���ĸ�����ս��
    for (int k = 0; k < s.size(); ++k) {
        if(s[k] == '@')
            continue;
        if( isExist(s[k],cfg.Ter,cfg.TerNum)<0 && isExist(s[k],cfg.Non,cfg.NonNum)<0 ){
            cfg.Ter[cfg.TerNum++] = s[k];
        }
    }
    //�Աȸò���ʽ�Ŀ�ʼ�����Ƿ����ڷ��ս�������д��ڣ����������ϲ����Ѵ��ڵĲ���ʽ��
    for (int i = 0; i < cfg.NonNum; ++i) {
        if(start == cfg.Non[i]){
            //���ڻ�Ҫ��֤��ʽ�Ӳ��ظ����ظ��Ļ�ֱ���˳���ʽ�ӣ����ظ���ϲ���
            for (int j = 0; j < cfg.Produ[i].length; ++j) {
                if(!s.compare(cfg.Produ[i].element[j]))
                    return;
            }
            cfg.Produ[i].element[cfg.Produ[i].length] += s;
            cfg.Produ[i].length++;
            return;
        }
    }
    //����ò���ʽ�Ŀ�ʼ���Ų������ڷ��ս������ Ҫ����cfg��ʽ���и����ս�������ս���Լ�����ʽ
    //ͬʱ���Ҹÿ�ʼ�����Ƿ����ս�������У������Ҫ�ó���
    if(isExist(start,cfg.Ter,cfg.TerNum) >= 0){
        for (int i = isExist(start,cfg.Ter,cfg.TerNum); i < cfg.TerNum-1; ++i) {
            cfg.Ter[i] = cfg.Ter[i+1];
        }
        cfg.TerNum--;
    }
    cfg.Non[cfg.NonNum++] = start;
    cfg.Produ[cfg.producNum++].element[0] += s;
    cfg.Produ[cfg.producNum-1].length = 1;

}

void initCFG(string s, CFG &cfg) {
    cfg.NonNum = 0;
    cfg.producNum = 0;
    cfg.TerNum = 0;

    vector<string> v = split(s,';');
    vector<string> m;
    char index;
    for(int i=0;i<=v.size()-1;i++){
        index = v[i][0];
        m = split(v[i].erase(0,2),'|');
        for (int j = 0; j < m.size(); ++j) {
            saveNewProduction(cfg,m[j],index);
        }
    }
}

void displayCFG(CFG cfg){
    cout<<"�ս������"<<cfg.TerNum<<endl;
    cout<<"�ս��Ϊ��";
    for (int i = 0; i < cfg.TerNum; ++i) {
        cout<<cfg.Ter[i]<<" ";
    }
    cout<<endl;
    cout<<"���ս������"<<cfg.NonNum<<endl;
    cout<<"���ս��Ϊ��";
    for (int i = 0; i < cfg.NonNum; ++i) {
        cout<<cfg.Non[i]<<" ";
    }
    cout<<endl;
    cout<<"����ʽ��"<<endl;
    for (int j = 0; j < cfg.producNum; ++j) {
        cout<<cfg.Non[j]<<"->";
        for (int i = 0; i < cfg.Produ[j].length; ++i) {
            cout<<cfg.Produ[j].element[i];
            if(i < cfg.Produ[j].length-1){
                cout<<" | ";
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

int newletter = 0;//��A��ʼ

//������ݹ�ʱ����������ĸ�ĺ���
int askNewLetter(CFG cfg){
    char letter = 'A' + newletter;
    if(isExist(letter,cfg.Non,cfg.NonNum)>=0 || isExist(letter,cfg.Ter,cfg.TerNum)>=0){
        newletter++;
        askNewLetter(cfg);
    } else{
        newletter++;
        return letter;
    }
}


//����ĳһ�����ս������ݹ�
void elminOneLeftRecur(CFG &cfg,int i){
    int index = 0;//��¼���������ĵڼ�������ݹ������
    int at;
    for (int j = 0; j < cfg.Produ[i].length; ++j) {
        if(cfg.Produ[i].element[j][0] == cfg.Non[i]){
            cout<<cfg.Non[i]<<"->"<<cfg.Produ[i].element[j]<<"����ݹ�"<<endl;
            if(index == 0){
                at = askNewLetter(cfg);
                if(!at){
                    cfg.Non[cfg.NonNum] = 'A' +(newletter-1);
                } else{
                    cfg.Non[cfg.NonNum] = at;
                }
//                cfg.Non[cfg.NonNum] = askNewLetter(cfg);
                cfg.NonNum++;
                cfg.Produ[cfg.producNum++].length = 0;
            }
            cfg.Produ[cfg.producNum-1].element[cfg.Produ[cfg.producNum-1].length++]
                    = cfg.Produ[i].element[j].erase(0,1) + cfg.Non[cfg.NonNum-1];
            for (int k = j; k < cfg.Produ[i].length-1; ++k) {
                cfg.Produ[i].element[k] = cfg.Produ[i].element[k+1];
            }
            cfg.Produ[i].length--;
            j--;
            index++;
        } else if (index!=0){
            cout<<cfg.Non[i]<<"->"<<cfg.Produ[i].element[j]<<"������ݹ�"<<endl;
            if(cfg.Produ[i].element[j][0] == '@')
                cfg.Produ[i].element[j] = cfg.Non[cfg.NonNum-1];
            else
                cfg.Produ[i].element[j] += cfg.Non[cfg.NonNum-1];
        } else {
            cout << cfg.Non[i] << "->" << cfg.Produ[i].element[j] << "������ݹ�" << endl;
        }
        if (j == cfg.Produ[i].length-1 && index!=0){
            cfg.Produ[cfg.producNum-1].element[cfg.Produ[cfg.producNum-1].length++] = '@';
        }
    }
}

//���������ʽ��ݹ飬Ҫ���л�����ʽ��ݹ�����������ݹ�
void elminAllLeftRecur(CFG &cfg){
    string tail;
    for (int i = 0; i < cfg.producNum; ++i) {
        for (int j = 0; j < i; ++j) {
            for (int k = 0; k < cfg.Produ[i].length; ++k) {
                if(cfg.Produ[i].element[k][0] == cfg.Non[j]){
                    tail.append(cfg.Produ[i].element[k],1,cfg.Produ[i].element[k].size()-1);
                    if(tail.empty())
                        continue;
                    cfg.Produ[i].element[k] = cfg.Produ[j].element[0] + tail;
                    for (int l = 1; l < cfg.Produ[j].length; ++l) {
                        if(cfg.Produ[j].element[l][0] == '@')
                            cfg.Produ[j].element[l] = tail;
                        else
                            cfg.Produ[i].element[cfg.Produ[i].length] = cfg.Produ[j].element[l] + tail;
                        cfg.Produ[i].length++;
                    }
                }
            }
        }
        elminOneLeftRecur(cfg,i);

    }
}
//who.compare(start,to,withwho)
//��֪�����ַ����ó������������ǰ׺
string getLongestPremix(string str1,string str2){
    if(str1.size()*str2.size() == 0)
        return "";
//    cout<<"error1"<<endl;
    string prestr;
    for (int i = 0; i < str1.size(); ++i) {
       if(str1[i] == str2[i]){
//           cout<<"match"<<i<<endl;
           prestr += str1[i];
       }else{
//           cout<<"not match"<<i<<endl;
           return prestr;
       }
    }
    return prestr;
}
//���i������ʽ���ǰ׺
string getLeftFactor(CFG &cfg,int i){
    string longest,index;
    for (int j = 0; j < cfg.Produ[i].length; ++j) {
        for (int k = j+1; k < cfg.Produ[i].length; ++k) {
            index = getLongestPremix(cfg.Produ[i].element[j],cfg.Produ[i].element[k]);
            if(longest.size() < index.size())
                longest = index;
        }
    }
    return longest;
}
//�������в���ʽ��������
void elimnAllLeftFactor(CFG &cfg){
    string lf,index;
    char at;
    for (int i = 0; i < cfg.producNum; ++i) {
        lf = getLeftFactor(cfg,i);
        if(lf.empty())
            continue;
        at = askNewLetter(cfg);
        if(!at){
            cfg.Non[cfg.NonNum] = 'A' +(newletter-1);
        } else{
            cfg.Non[cfg.NonNum] = at;
        }
        cfg.NonNum++;
        cfg.Produ[cfg.producNum++].length = 0;
        for (int j = 0; j < cfg.Produ[i].length; ++j) {
            if(lf == index.assign(cfg.Produ[i].element[j],0,lf.size())){
                //���if else�������������ɵĲ���ʽ
                if(cfg.Produ[i].element[j].size()-lf.size() == 0)
                    cfg.Produ[cfg.producNum-1].element[cfg.Produ[cfg.producNum-1].length] = "@";
                else{
                    cfg.Produ[cfg.producNum-1].element[cfg.Produ[cfg.producNum-1].length].assign(cfg.Produ[i].element[j],lf.size(),cfg.Produ[i].element[j].size()-lf.size());
                }
                    cfg.Produ[cfg.producNum-1].length++;
                //�����������ԭ���Ĳ���ʽ
                if(j == cfg.Produ[i].length-1){
                    cfg.Produ[i].element[j] = lf + cfg.Non[cfg.NonNum-1];
                    return;
//                    cfg.Produ[i].element[j].erase();
//                    cfg.Produ[i].length--;
                }else{
                    for (int k = j; k < cfg.Produ[i].length-1; ++k) {
                        cfg.Produ[i].element[k].swap(cfg.Produ[i].element[k+1]);
                    }
                    cfg.Produ[i].length--;
                    j--;
//                    cfg.Produ[i].element[j] = cfg.Produ[i].element[cfg.Produ[i].length-1];
//                    j--;
                }

            }
            index = "";
            if(j == cfg.Produ[i].length-1){
                cfg.Produ[i].element[j+1] = lf + cfg.Non[cfg.NonNum-1];
                cfg.Produ[i].length++;
                return;
//                    cfg.Produ[i].element[j].erase();
//                    cfg.Produ[i].length--;
            }
        }
    }
}
//�ж��ַ��Ƿ��Ѵ��ڣ���������ڽ��ַ������ַ�����
void insertChar(char ch,char (&chars)[MaxSize],int &length){
    if(isExist(ch,chars,length)>=0)
        return;
    chars[length++] = ch;
}
//��ȡ���ս���������±�Ϊi�ķ��ս����first��
void getOneFirstSet(CFG &cfg,int i){
    if(i >= cfg.NonNum)
        return;
    cfg.first[i].done = false;
    if(!cfg.first[i].elem[0])
        cfg.first[i].length = 0;
    char temp;
    int temp2,NonNum = 0,temp3;
    bool isContinue;
    //�������������ʽ������ĸ���ս�����Ƿ��ս����
    for (int j = 0; j < cfg.Produ[i].length; ++j) {
        temp = cfg.Produ[i].element[j][0];
        //������ս����գ���ֱ�Ӵ���first����
        if(isExist(temp,cfg.Ter,cfg.TerNum) >=0 ||temp == '@'){
//            cout<<"����"<<temp<<endl;
            insertChar(temp,cfg.first[i].elem,cfg.first[i].length);
        }else {
            temp2 = isExist(temp,cfg.Non,cfg.NonNum);
            //����Ƿ��ս�� ��������� �ֱ���һ�����ս��+�ս�� ��һ������Ƕ���ս��+���ս��
            if(temp2>=0){
                for (int k = 0; k < cfg.Produ[i].element[j].size(); ++k) {
                    if(isExist(cfg.Produ[i].element[j][k],cfg.Non,cfg.NonNum)<0)
                        break;
                    NonNum++;
                }
                for (int k = 0; k < NonNum; ++k) {
                    temp3 = isExist(cfg.Produ[i].element[j][k],cfg.Non,cfg.NonNum);
                    isContinue = false;
                    getOneFirstSet(cfg,temp3);
                    if(cfg.first[temp3].done){
//                        cout<<"temp:"<<temp3<<endl;
                        for (int l = 0; l < cfg.first[temp3].length; ++l) {
                            if(cfg.first[temp3].elem[l] == '@'){
                                isContinue = true;
                                continue;
                            }
//                            cout<<"���ڲ��룺"<<cfg.first[temp3].elem[l]<<endl;
                            insertChar(cfg.first[temp3].elem[l],cfg.first[i].elem,cfg.first[i].length);
                        }
                    }else{
                        cout<<"error: "<<cfg.Non[temp3]<<"����first�����̷�������"<<endl;
                        return;
                    }
                    if(!isContinue) {
                        break;
                    }
                    if(k == NonNum-1){
                        if(cfg.Produ[i].element[j].size() > NonNum){
                            insertChar(cfg.Produ[i].element[j][NonNum],cfg.first[i].elem,cfg.first[i].length);
                        } else if (cfg.Produ[i].element[j].size() == NonNum){
                            insertChar('@',cfg.first[i].elem,cfg.first[i].length);
                        }else{
                            cout<<"error: �������ս������ > ��������ʽ����"<<endl;
                        }
                    }
                }


            }

        }
    }
    cfg.first[i].done = true;
}
void getAllFirstSet(CFG &cfg){
    for (int i = 0; i < cfg.NonNum; ++i) {
        cfg.first[i].done = false;
    }
    for (int j = 0; j < cfg.NonNum; ++j) {
        if(!cfg.first[j].done){
            getOneFirstSet(cfg,j);
        }
    }
//    getOneFirstSet(cfg,2);
}
void displayFirst(CFG cfg){
    cout<<" First      Follow"<<endl;
    for (int i = 0; i < cfg.NonNum; ++i) {
        cout<<cfg.Non[i]<<":";
        for (int j = 0; j < cfg.first[i].length; ++j) {
            cout<<cfg.first[i].elem[j];
            if(j < cfg.first[i].length-1) cout<<" ";
        }
        cout<<"       ";
        for (int j = 0; j < cfg.follow[i].length; ++j) {
            cout<<cfg.follow[i].elem[j];
            if(j < cfg.follow[i].length-1) cout<<" ";
        }
        cout<<endl;
    }
}
void displayFollow(CFG cfg){
    cout<<"     Follow������"<<endl;
    for (int i = 0; i < cfg.NonNum; ++i) {
        cout<<cfg.Non[i]<<":";
        for (int j = 0; j < cfg.follow[i].length; ++j) {
            cout<<cfg.follow[i].elem[j];
            if(j < cfg.follow[i].length-1) cout<<" ";
        }
        cout<<endl;
    }
}
//����һ�����ս����Follow��
void getOneFollow(CFG &cfg,int i){
    cfg.follow[i].done = true;
//    cout<<"���ڽ���"<<cfg.Non[i]<<"��follow"<<endl;
    char temp;
    int temp2,temp3;
    for (int j = 0; j < cfg.producNum; ++j) {
        for (int k = 0; k < cfg.Produ[j].length; ++k) {
            temp2 = isExist(cfg.Non[i],cfg.Produ[j].element[k]);
            if(temp2 < 0) {
//                cout<<cfg.Produ[j].element[k]<<"�в���"<<cfg.Non[i]<<endl;
                continue;
            }
            else{
                //�������һ��
                if(temp2 < cfg.Produ[j].element[k].size()-1){
//                    cout<<cfg.Produ[j].element[k]<<"1�к���"<<cfg.Non[i]<<endl;
                    temp = cfg.Produ[j].element[k][temp2+1];
                    temp3 = isExist(temp,cfg.Non,cfg.NonNum);
                    //�ж�������ڷ��ս������Ƿ��ս��
                    if(temp3 >= 0){
                        for (int l = 0; l < cfg.first[temp3].length; ++l) {
                            if(cfg.first[temp3].elem[l] == '@') continue;
                            insertChar(cfg.first[temp3].elem[l],cfg.follow[i].elem,cfg.follow[i].length);
                        }
                        if(isExist('@',cfg.first[temp3].elem,cfg.first[temp3].length) >= 0){
                            if(!cfg.follow[j].done){
                                if(cfg.Non[j] == cfg.Non[i]){
                                    continue;
                                }
                                getOneFollow(cfg,j);
                            }
                            if(!cfg.follow[j].done){
                                return;
                            }
                            for (int l = 0; l < cfg.follow[j].length; ++l) {
                                insertChar(cfg.follow[j].elem[l],cfg.follow[i].elem,cfg.follow[i].length);
                            }
                        }

                    }else{//������ڷ��ս��������ս��
                        insertChar(temp,cfg.follow[i].elem,cfg.follow[i].length);
                    }
                }else if(temp2 == cfg.Produ[j].element[k].size()-1){
                    if(!cfg.follow[j].done){
                        if(cfg.Non[j] == cfg.Non[i]){
                            continue;
                        }
                        getOneFollow(cfg,j);
                    }
                    if(!cfg.follow[j].done){
                        return;
                    }
                    for (int l = 0; l < cfg.follow[j].length; ++l) {
                        insertChar(cfg.follow[j].elem[l],cfg.follow[i].elem,cfg.follow[i].length);
                    }
                }
            }
        }
    }

}
//����FOLLOW��
void getAllFollow(CFG &cfg){
    //����#��FOLLOW(S)�����У�S�ǿ�ʼ���ţ�#�����������ǣ�
    cfg.follow[0].elem[0] = '#';
    cfg.follow[0].length++;
    for (int i = 0; i < cfg.NonNum; ++i) {
        cfg.follow[i].done = false;
    }
    for (int i = 0; i < cfg.NonNum; ++i) {
        getOneFollow(cfg,i);
    }
}
void displayPreAna(CFG &cfg){
    cout<<"Ԥ����������£�"<<endl;
    cout<<"     ";
    for (int j = 0; j < cfg.TerNum; ++j) {
        cout<<cfg.Ter[j]<<"      ";
    }
    cout<<"#";
    for (int i = 0; i < cfg.NonNum; ++i) {
        cout<<endl;
        cout<<cfg.Non[i]<<":  " ;
        for (int j = 0; j < cfg.TerNum+1; ++j) {
            cout<<cfg.predAnal[i][j]<<"   ";
//            if(cfg.predAnal[i][j]!="error"){
//                cout<<cfg.predAnal[i][j]<<"   ";
//            }else{
//                cout<<"    ";
//            }
        }
        cout<<endl;
    }
}
//����Ԥ�������
void predictAnalysis(CFG &cfg){
    for (int i = 0; i < cfg.NonNum; ++i) {
        for (int j = 0; j < cfg.TerNum+1; ++j) {
            cfg.predAnal[i][j] = "error";
        }
    }
    int nonIndex;
    //�ȴ���First��
    for (int k = 0; k < cfg.NonNum; ++k) {
        for (int i = 0; i < cfg.first[k].length; ++i) {
            if(cfg.first[k].elem[i] == '@')
                continue;
            nonIndex = isExist(cfg.first[k].elem[i],cfg.Ter);
            //�����������
            //1.ĳ����ʽֻ��һ���ǿշ�֧��Ϊ���ս����ͷ����ô���÷�֧����ÿ��first��Ӧ�ĸ�����
            //��һ���ǿշ�֧�����������ֻ��һ���ǿշ�֧��������һ���ǿշ�֧��һ���շ�֧
            //2.������ÿ������ʽ��������һ���ս����ͷ�ķ�֧
            if((cfg.Produ[k].length == 1&&isExist(cfg.Produ[k].element[0][0],cfg.Non)>=0)
            ||(cfg.Produ[k].length == 2&&isExist(cfg.Produ[k].element[0][0],cfg.Non)>=0&&cfg.Produ[k].element[1][0]=='@')){
                //�������û������� ��ô������error������������
                if(cfg.predAnal[k][nonIndex] == "error")
                    cfg.predAnal[k][nonIndex] = cfg.Produ[k].element[0];
                else{//������ݲ���error ��ô˵�����������Ҫ�ֹһ��Ԫ�أ�ֱ�ӱ����˳�
                    displayPreAna(cfg);
                    cout<<"����"<<cfg.Produ[k].element[0]<<"��"<<cfg.predAnal[k][nonIndex]<<"�ڶ�ά�����±�("<<k<<","<<nonIndex<<")ײ����ֱ���˳�"<<endl;
                    cout<<"Ԥ�������Ԥ����ʾ���ķ�����LL(1)�ķ�"<<endl;
                    return;
                }
            }else{
                for (int j = 0; j < cfg.Produ[k].length; ++j) {
                    if(cfg.first[k].elem[i] == cfg.Produ[k].element[j][0]){
                        //�������û������� ��ô������error������������
                        if(cfg.predAnal[k][nonIndex] == "error")
                            cfg.predAnal[k][nonIndex] = cfg.Produ[k].element[j];
                        else{//������ݲ���error ��ô˵�����������Ҫ�ֹһ��Ԫ�أ�ֱ�ӱ����˳�
                            displayPreAna(cfg);
                            cout<<"����"<<cfg.Produ[k].element[j]<<"��"<<cfg.predAnal[k][nonIndex]<<"�ڶ�ά�����±�("<<k<<","<<nonIndex<<")ײ����ֱ���˳�"<<endl;
                            cout<<"Ԥ�������Ԥ����ʾ���ķ�����LL(1)�ķ�"<<endl;
                            return;
                        }
                    }
                }
            }
        }
    }
    //����FOLLOW��
//    cout<<"����FOLLOW��"<<endl;
    bool haveNull = false;
    for (int l = 0; l < cfg.NonNum; ++l) {
        for (int i = 0; i < cfg.first[l].length; ++i) {
            if(cfg.first[l].elem[i] == '@')
                haveNull = true;
        }
        if(!haveNull) continue;
        for (int i = 0; i < cfg.follow[l].length; ++i) {
            if(cfg.follow[l].elem[i] == '#'){
                nonIndex = cfg.TerNum;
            } else{
                nonIndex = isExist(cfg.follow[l].elem[i],cfg.Ter);
//                cout<<cfg.follow[l].elem[i]<<"��"<<nonIndex<<endl;
            }
            if(cfg.predAnal[l][nonIndex] == "error")
                cfg.predAnal[l][nonIndex] = '@';
            else{//������ݲ���error ��ô˵�����������Ҫ�ֹһ��Ԫ�أ�ֱ�ӱ����˳�
                displayPreAna(cfg);
                cout<<"����"<<"@"<<"��"<<cfg.predAnal[l][nonIndex]<<"�ڶ�ά�����±�("<<l<<","<<nonIndex<<")ײ����ֱ���˳�"<<endl;
                cout<<"Ԥ�������Ԥ����ʾ���ķ�����LL(1)�ķ�"<<endl;
                return;
            }
        }
        haveNull = false;
//        displayPreAna(cfg);
    }
    cout<<"Ԥ�������Ԥ����ʾ���ķ���LL(1)�ķ�"<<endl;
    displayPreAna(cfg);
}
//���ļ����뵽string��
string readFileIntoString(char * filename)
{
    ifstream ifile(filename);
//���ļ����뵽ostringstream����buf��
    ostringstream buf;
    char ch;
    while(buf&&ifile.get(ch))
        buf.put(ch);
//������������buf�������ַ���
    return buf.str();
}


int main() {
    string s = "";
    CFG cfg;
//    cin>>s;
    s += readFileIntoString("C:\\Users\\Houble\\CLionProjects\\Parser\\test.txt");
//    s = "S:(T)|a+S|a;T:T,S|S";//"E:E+T|T;T:T*F|F;F:(E)|i"; //S:(T)|a+S|a;T:T,S|S
//    s = "S:Aa|b;A:Ac|Sd|@";
//    s = "A:aABj|a;B:Bb|d ";
//    s = "M:MaH|H;H:b(M)|(M)|b";
//    s = "S:AdD|@;A:aAd|@;D:DdA|b|@";
// S:ABCD;A:a|@;B:b|@;C:c|@;D:d
//S:(L)|aS|a;L:L,S|S;
    for(int k = 0; k < s.size(); ++k) {
        if((int)s[k] == 32){
            cout<<"������пո���ɾ���ո���ٽ��в��ԡ�"<<endl;
            return 0;
        }else if((int)s[k] < 32||(int)s[k] > 126){
            cout<<"������зǷ��ַ������滻ΪASCII����33-126�����ڷ��ź��ٽ��в��ԡ�"<<endl;
            return 0;
        }
    }
    initCFG(s,cfg);
    cout<<"��ʼ��ΪCFG��"<<endl;
    displayCFG(cfg);
    cout<<"���ڴ�����ݹ飺"<<endl;
    elminAllLeftRecur(cfg);
    cout<<endl;
    cout<<"������ݹ��Ϊ��"<<endl;
    displayCFG(cfg);
    cout<<"��ȡ�����Ӻ�Ϊ��"<<endl;
    elimnAllLeftFactor(cfg);
    displayCFG(cfg);
//    cout<<getLeftFactor(cfg,0);
    getAllFirstSet(cfg);
//    displayFirst(cfg);
    getAllFollow(cfg);
    displayFirst(cfg);
    predictAnalysis(cfg);
    system("pause");
    return 0;
}
