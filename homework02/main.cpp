#include <QApplication>
#include <QDebug>
#include <QVector>
#include <QTextStream>
#include <QFile>
#define N 4
namespace SK {
enum SortKind{
    col01    =   0x00000001<<0,         //!< 第1列
    col02    =   0x00000001<<1,         //!< 第2列
    col03    =   0x00000001<<2,         //!< 第3列
    col04    =   0x00000001<<3,         //!< 第4列
    col05    =   0x00000001<<4,         //!< 第5列
    col06    =   0x00000001<<5,         //!< 第6列
    col07    =   0x00000001<<6,         //!< 第7列
    col08    =   0x00000001<<7,         //!< 第8列
    col09    =   0x00000001<<8,         //!< 第9列
    col10    =   0x00000001<<9,         //!< 第10列
    col11    =   0x00000001<<10,        //!< 第11列
    col12    =   0x00000001<<11,        //!< 第12列
    col13    =   0x00000001<<12,        //!< 第13列
    col14    =   0x00000001<<13,        //!< 第14列
    col15    =   0x00000001<<14,        //!< 第15列
    col16    =   0x00000001<<15,        //!< 第16列
    col17    =   0x00000001<<16,        //!< 第17列
    col18    =   0x00000001<<17,        //!< 第18列
    col19    =   0x00000001<<18,        //!< 第19列
    col20    =   0x00000001<<19,        //!< 第20列
    col21    =   0x00000001<<20,        //!< 第21列
    col22    =   0x00000001<<21,        //!< 第22列
    col23    =   0x00000001<<22,        //!< 第23列
    col24    =   0x00000001<<23,        //!< 第24列
    col25    =   0x00000001<<24,        //!< 第25列
    col26    =   0x00000001<<25,        //!< 第26列
    col27    =   0x00000001<<26,        //!< 第27列
    col28    =   0x00000001<<27,        //!< 第28列
    col29    =   0x00000001<<28,        //!< 第29列
    col30    =   0x00000001<<29,        //!< 第30列
    col31    =   0x00000001<<30,        //!< 第31列
    col32    =   0x00000001<<31,        //!< 第32列
};
}

typedef struct{
    QStringList stu;
} studData;

QDebug operator<< (QDebug d, const studData &data) {
   for(int i=0;i<data.stu.size();i++)
   {
       d.noquote()<<data.stu.at(i);
   }
    return d;
}
class myCmp {
public:
    myCmp(int selectedColumn) { this->currentColumn = selectedColumn; }
    bool operator() (const studData& d1,const studData& d2);
private:
    int currentColumn;
};
bool myCmp::operator()(const studData &d1, const studData &d2)
{
    bool result = false;
    quint32 sortedColumn = 0x00000001<<currentColumn;
    switch (sortedColumn) {
    default:result=(d1.stu.at(currentColumn+1)>=d2.stu.at(currentColumn+1));
        break;
    }
    return result;
}
class ScoreSorter
{
public:
    ScoreSorter(QString dataFile);
    void readfile();
    void dosort();
private:
QString tempfile;
QList<studData> student;
QStringList quantity;
};
ScoreSorter::ScoreSorter(QString dataFile)
{
    tempfile=dataFile;
}
void ScoreSorter::dosort()
{
    myCmp cmp_temp(N-2);
    std::sort(student.begin(),student.end(),cmp_temp);
    qDebug()<<"排序后输出"<<N<<"列："<<"\n";
    quantity.removeLast();
    qDebug().nospace().noquote()<<quantity;
    for(int i=0;i<student.size();i++)
    {
        qDebug()<<student.at(i);
    }
}
void ScoreSorter::readfile()
{
    QFile mfile(tempfile);
         if (!mfile.open(QIODevice::ReadOnly | QIODevice::Text))
           {
           qDebug()<<"无法打开文件";
   }

         studData tempstu;
         QString quant(mfile.readLine());
                quantity= quant.split(" ", QString::SkipEmptyParts);
         while (!mfile.atEnd()) {
             QString str(mfile.readLine());
                     tempstu.stu= str.split(" ", QString::SkipEmptyParts);
                     if((tempstu.stu).last() == "\n") tempstu.stu.removeLast();
                     if(tempstu.stu.size()==0) continue;
                     student.append(tempstu);
         }
         mfile.close();
}

int main()
{
    QString datafile = "data.txt";
    QFile f("sorted_"+datafile);
    if (f.exists()){
        f.remove();
    }

    ScoreSorter s(datafile);
    s.readFile();
    s.doSort();
    return 0;
}
