#include <QApplication>
#include <iostream>
#include <Qstring>
#include <QVector>
#include<QDebug>
#include <algorithm>
using namespace std;
class student
{
public:
   int num;
   QString name;
   int class1;
   int class2;

};

void st(QVector<student> &vec,int num,QString name,int class1,int class2)
{
    student stu;
    stu.num=num;
    stu.name=name;
    stu.class1=class1;
    stu.class2=class2;
    vec.push_back(stu);
}


bool less_name(const student &s1, const student &s2) //按姓名排序
    {return s1.name < s2.name;}
bool less_class1(const student &s1, const student &s2) //按课程一排序
    {return s1.class1 > s2.class1;}
bool less_class2(const student &s1, const student &s2) //按课程二排序
    {return s1.class2 > s2.class2;}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

        QVector<student> vec;
        st(vec,1403140103,"武松",88,80);
        st(vec,1403140101,"林冲",82,76);
        st(vec,1403130209,"鲁智深",80,72);
        st(vec,1403140102,"宋江",76,85);

 QVector<student>::iterator iter;

        qDebug()<< "排序前" << endl;
        for (iter = vec.begin(); iter != vec.end(); iter++)
            {
               qDebug()<<  (*iter).num <<  (*iter).name <<  (*iter).class1 <<  (*iter).class2 <<endl;
            }

         sort(vec.begin(),vec.end(),less_name);
         qDebug()<< "按姓名排序" << endl;
         for (iter = vec.begin(); iter != vec.end(); iter++)
             {
                qDebug()<< (*iter).num <<  (*iter).name <<  (*iter).class1 <<  (*iter).class2 <<endl;
             }

         sort(vec.begin(),vec.end(),less_class1);
         qDebug()<< "按课程1排序" << endl;
         for (iter = vec.begin(); iter != vec.end(); iter++)
             {
                qDebug()<<  (*iter).num <<  (*iter).name <<  (*iter).class1 <<  (*iter).class2 <<endl;
             }

         sort(vec.begin(),vec.end(),less_class2);
         qDebug()<< "按课程2排序" << endl;
         for (iter = vec.begin(); iter != vec.end(); iter++)
             {
                qDebug()<<  (*iter).num <<  (*iter).name <<  (*iter).class1 <<  (*iter).class2 <<endl;
             }



    return a.exec();
}
