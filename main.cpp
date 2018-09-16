#include <QCoreApplication>
#include <QDebug>
#include <QList>

typedef quint32 word;
typedef quint8 byte;                                     //定义word为32位 byte为8位
#define WORD_LLO(xxx) ((byte) ((word)(xxx) & 255))       //取最低8位
#define WORD_HLO(xxx) ((byte) ((word)(xxx) >> 8))        //左移8位取次低8位
#define WORD_LHI(xxx) ((byte) ((word)(xxx) >> 16))       //再次左移8位取次高8位
#define WORD_HHI(xxx) ((byte) ((word)(xxx) >> 24))       //再次左移8位取最高8位
#define MAX(x,y) (((x)>(y)) ? (x):(y))                   //取最大值
#define MIN(x,y) (((x)<(y)) ? (x):(y))                   //取最小值

int main(int argc, char *argv[])
{
    qint32 m=0x12345678;                                 //定义一个32位无符号整型数
    qDebug("原始值：0x%x==%d",m,m) ;
    qDebug("0x%x==%d  0x%x==%d  0x%x==%d  0x%x==%d" ,
           WORD_HHI(m),WORD_HHI(m),                      //输出最高8位
           WORD_LHI(m),WORD_LHI(m),                      //输出次高8位
           WORD_HLO(m),WORD_HLO(m),                      //输出次低8位
           WORD_LLO(m),WORD_LLO(m));                     //输出最低8位
    QList<qint8> values;
    values << WORD_HHI(m)<< WORD_LHI(m)
           << WORD_HLO(m)<< WORD_LLO(m);                //将取出结果存入values中
    qDebug("最高8位和次高8位最大值：0x%x(%d)",
           (MAX(WORD_HHI(m),WORD_LHI(m))),
           (MAX(WORD_HHI(m),WORD_LHI(m))));              //输出最高8位和次高8位最大值
    qDebug("次低8位和最低8位最小值：0x%x(%d)",
           (MIN(WORD_HLO(m),WORD_LLO(m))),
           (MIN(WORD_HLO(m),WORD_LLO(m))));              //输出次低8位和最低8位最小值
    qint32 n=(WORD_HLO(m)<<24)+(WORD_HHI(m)<<16)
                +(WORD_LLO(m)<<8)+(WORD_LHI(m));         //重新排列组合成一个新的32位无符号整型数
    qDebug("重新组合后数值：0x%x(%d)",n,n);                 //输出

    qDebug() << "排序前：" << values;
    std::sort(values.begin(),values.end(),
    std::greater<quint8>());                             //使用std::sort函数，将数字从大到小排列
    qDebug() << "排序后：" << values;

    QCoreApplication a(argc, argv);

    return a.exec();
}
