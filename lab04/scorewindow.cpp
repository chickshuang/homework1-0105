#include "scorewindow.h"
#include "ui_scorewindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QDebug>



ScoreWindow::ScoreWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScoreWindow)
{
    ui->setupUi(this);

    //加载数据库驱动
    db = QSqlDatabase::addDatabase("QMYSQL");
    //设置账号密码
    db.setHostName("localhost");//设置主机地址
    db.setPort(3306);           //设置端口
    db.setUserName("root");     //设置用户名
    db.setPassword("");         //设置密码
    db.setDatabaseName("test"); //设置数据库名称

    QPushButton *btnQuery=ui->btnQuery;
    connect(btnQuery,&QPushButton::clicked,this,&ScoreWindow::on_btnQuery_clicked);
}

ScoreWindow::~ScoreWindow()
{
    delete ui;
}

void ScoreWindow::on_btnQuery_clicked()
{
    if(db.isOpen())//打开数据库连接
        db.close();//释放数据库连接
    if(!db.open()){
        QMessageBox::critical(this,tr("错误"),
     tr("无法连接数据库！\n请查询数据库连接装置。"));
        return;
    }
    else{
        QString str;
        if(ui->radioStNumber->isChecked())
            str=QString("where sNumber='%1'").arg(ui->edtInput->text());
        else if(ui->radioStName->isChecked()){
            str=QString("where sName='%1'").arg(ui->edtInput->text());
         qDebug()<<str;}
        //else if(ui->radioStAll->isChecked())
          //  str="";
        QString queryStr=QString
                ("SELECT * FROM t_stud_info %1;").arg(str);
        queryStudentInfo(queryStr);   //查询学生的输入
        queryStr = QString("select t_stud_info.sName as '姓名',t_courses.courseName as '课程名称',"
                                  "t_stud_course_info.scScores as '成绩',t_courses.courseCredit as '学分' "
                                  "from "
                                  "(t_stud_course_info inner join t_courses on t_stud_course_info.scCourseID = t_courses.courseID) "
                                  "inner join t_stud_info on t_stud_info.sNumber = t_stud_course_info.scNumber "
                                  "%1 "
                                  "order by t_stud_course_info.scNumber "
                                  ";").arg(str);
        queryStudentScore(queryStr);  //查询学生的成绩
        calculatePoints(queryResult);    //计算绩点
        updateInfoWindow(queryResult);   //更新输入窗口
        OutputTheInfo("t_courses","t_courses.csv");
        OutputTheInfo("t_stud_course_info","t_stud_course_info.csv");
        OutputTheInfo("t_stud_info","t_stud_info.csv");
        db.close();


    }

}

//学生详细信息
void ScoreWindow::queryStudentInfo(QString query1)
{
    QSqlQuery query(query1);
    while(query.next())
    {
        ui->number->setText(query.value(0).toString());//第一行，学号（查询t-stud-info）
        ui->name->setText(query.value(1).toString());//第二行，姓名
        ui->major->setText(query.value(2).toString());//第三行，课程
        ui->grade->setText(query.value(3).toString());//第四行，年级
    }

}

void ScoreWindow::queryStudentScore(QString str2)
{
    sum = 0;
    QSqlQuery query(str2);
    QStringList Result;
    Result.append("");
    while(query.next())
    {
        course[sum] = query.value(1).toString();//'姓名','课程名称', '成绩','学分'
        Result.append(course[sum]);
        grade[sum] = query.value(2).toString();
        Result.append(grade[sum]);
        credit[sum] = query.value(3).toString();
        Result.append(credit[sum]);
        sum++;
    }
    if(sum==0){
        QMessageBox::information(this,tr("Error Message!"),tr("No select Data！"));
    }
    queryResult=Result;
}


void ScoreWindow::calculatePoints(QStringList str3)//计算绩点
{
    int i=0;
    int j=0;
    int k=0;

    double addCredit1 = 0;
    for (i = 0;i<sum;i++)
    {
        double GPA1;
        if(grade[i]=="优秀"||grade[i]=="A")  GPA[j++] = "4";
        else if(grade[i]=="良好"||grade[i]=="B")  GPA[j++] = "3.5";
        else if(grade[i]=="中等"||grade[i]=="C")  GPA[j++] = "2.5";
        else if(grade[i]=="及格"||grade[i]=="D")  GPA[j++] = "1.5";
        else if(grade[i]=="旷考"||grade[i]=="作弊"||grade[i]=="E")  GPA[j++] = "0";
        else if(grade[i]=="缓考"){
            GPA[j++] = "0";
            addCredit[k]=credit[i];
            addCredit1 = addCredit1 + addCredit[k].toDouble();
            k++;
        }
        else {
            double a=grade[i].toDouble();
            if(a<60) GPA[j++] = "0";
            else if(90<=a&&a<=100)
            {
               GPA1 =4.00;
               GPA[j++] = QString("%1").arg(GPA1);
            }
            else if(86<=a&&a<=89)
            {
               GPA1 =3.70;
               GPA[j++] = QString("%1").arg(GPA1);
            }
            else if(82<=a&&a<=85)
            {
               GPA1 =3.30;
               GPA[j++] = QString("%1").arg(GPA1);
            }
            else if(79<=a&&a<=81)
            {
               GPA1 =3.00;
               GPA[j++] = QString("%1").arg(GPA1);
            }
            else if(75<=a&&a<=78)
            {
               GPA1 =2.70;
               GPA[j++] = QString("%1").arg(GPA1);
            }
            else if(71<=a&&a<=74)
            {
               GPA1 =2.30;
               GPA[j++] = QString("%1").arg(GPA1);
            }
            else if(68<=a&&a<=70)
            {
               GPA1 =2.00;
               GPA[j++] = QString("%1").arg(GPA1);
            }
            else if(64<=a&&a<=67)
            {
               GPA1 =1.70;
               GPA[j++] = QString("%1").arg(GPA1);
            }
            else if(60<=a&&a<=63)
            {
               GPA1 =1.30;
               GPA[j++] = QString("%1").arg(GPA1);
            }
        }
    }


    double GPA2= 0;//平均绩点
    for(i = 0;i < sum;i++)
    {
        GPA2 = GPA2 + GPA[i].toDouble()/sum;
    }
    finalGPA = QString("绩点：%1").arg(GPA2);
    ui->GPA->setText(finalGPA);
    for(i=0;i<sum;i++)
    {
       str3.append(GPA[i]);
    }
    str3.append(finalGPA);
    queryResult2 = str3;
}

void ScoreWindow::updateInfoWindow(QStringList)
{
    ui->textEdit->clear();
    QString st[7];
    st[0]=QString("1");
    st[1]=QString("2");
    st[2]=QString("3");
    st[3]=QString("4");
    st[4]=QString("5");
    st[5]=QString("6");
    st[6]=QString("7");

    QString s = QString("%1的成绩单：").arg(ui->name->text());
    QString text = "<table border='1'  cellpadding='10' >"
                  "<caption><td>"+s+"</td></caption>"
                  " <tr>   <td>编号</td>   <td>课程名称</td>   <td>成绩</td>    <td>学分</td>   <td>绩点</td> </tr> ";
                  //" <tr>   <td> 1 </td>   <td>"+course[0]+"</td>   <td>"+grade[0]+"</td>    <td>"+credit[0]+"</td>   <td>"+GPA[0]+"</td> </tr> "
                  //" <tr>   <td> 2 </td>   <td>"+course[1]+"</td>   <td>"+grade[1]+"</td>    <td>"+credit[1]+"</td>   <td>"+GPA[1]+"</td> </tr> "
                  //" <tr>   <td> 3 </td>   <td>"+course[2]+"</td>   <td>"+grade[2]+"</td>    <td>"+credit[2]+"</td>   <td>"+GPA[2]+"</td> </tr> "
                  //" <tr>   <td> 4 </td>   <td>"+course[3]+"</td>   <td>"+grade[3]+"</td>    <td>"+credit[3]+"</td>   <td>"+GPA[3]+"</td> </tr> "
                  //" <tr>   <td> 5 </td>   <td>"+course[4]+"</td>   <td>"+grade[4]+"</td>    <td>"+credit[4]+"</td>   <td>"+GPA[4]+"</td> </tr> "
                  //" <tr>   <td> 6 </td>   <td>"+course[5]+"</td>   <td>"+grade[5]+"</td>    <td>"+credit[5]+"</td>   <td>"+GPA[5]+"</td> </tr> "
                  //" <tr>   <td> 7 </td>   <td>"+course[6]+"</td>   <td>"+grade[6]+"</td>    <td>"+credit[6]+"</td>   <td>"+GPA[6]+"</td> </tr> "
                  //"</table> ";
    for(int i=0;i<sum;i++)
    {
        text.append(" <tr>   <td>"+st[i]+"</td>   <td>"+course[i]+"</td>   <td>"+grade[i]+"</td>    <td>"+credit[i]+"</td>   <td>"+GPA[i]+"</td> </tr> ");
    }

    for(int i=0;i<7;i++)
    {
        course[i]=QString("");
        grade[i]=QString("");
        credit[i]=QString("");
        GPA[i]=QString("");

    }
    ui->textEdit->insertHtml(text);
    ui->textEdit->setTextColor(Qt::red);
    ui->textEdit->setCurrentFont(QFont("楷体",15));
    ui->textEdit->insertPlainText(finalGPA);
}

void ScoreWindow::on_writeButton_clicked()
{
    if(db.isOpen())
        db.close();

    if(!db.open())
    {
        QMessageBox::critical((this),tr("错误"),tr("无法连接到数据库！\n请检查数据库连接配置"));
        return;
    }else {
    QString  stb = QString("select courseName from t_courses");//加载所有课程


        QSqlQuery query(db);
        query.exec(stb);

        while(query.next()){
           // qDebug()<<query.value(0).toString();
            allcourselist<<query.value(0).toString();
        }

    }
    btndialog = new Dialog(this);
    btndialog->setquery(db,allcourselist,allNamelist);
    btndialog->setWindowTitle("信息录入");
    btndialog->show();
    OutputTheInfo("t_courses","t_courses.csv");
    OutputTheInfo("t_stud_course_info","t_stud_course_info.csv");
    OutputTheInfo("t_stud_info","t_stud_info.csv");
}

//将表导出为csv格式表格
void ScoreWindow::OutputTheInfo(const QString &tableName,const QString &csvFileName)
    {
        QSqlTableModel *exportModel = new QSqlTableModel();
    //    exportModel->setTable("student");
        exportModel->setTable(tableName);
        exportModel->select();
        QStringList strList;//记录数据库中的一行报警数据
        QString strString;
        const QString FILE_PATH(csvFileName);
    //    const QString FILE_PATH("student.csv");
        QFile csvFile(FILE_PATH);
        if (csvFile.open(QIODevice::ReadWrite))
        {
            for (int i=0;i<exportModel->rowCount();i++)
            {
                for(int j=0;j<exportModel->columnCount();j++)
                {
                    strList.insert(j,exportModel->data(exportModel->index(i,j)).toString());//把每一行的每一列数据读取到strList中
                }
                strString = strList.join(", ")+"\n";//给两个列数据之前加“,”号，一行数据末尾加回车
                strList.clear();//记录一行数据后清空，再记下一行数据
                csvFile.write(strString.toUtf8());//使用方法：转换为Utf8格式后在windows下的excel打开是乱码,可先用notepad++打开并转码为unicode，再次用excel打开即可。
                qDebug()<<strString.toUtf8();
            }
            csvFile.close();
        }
    }

//导入文件
void ScoreWindow::on_importantButton_clicked()
{
    path = QFileDialog::getOpenFileName(this, tr("Open csv"), ".", tr("csv Files(*.csv)"));
    if(path.length() == 0) {
        return;
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }
    QStringList  tempText;
    QFile file(path);
    qDebug()<<path;
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(NULL,tr("错误"),tr("打开文件失败！"));
        return;
    }
    QTextStream out(&file);
    while(!out.atEnd()){
        tempText.append(out.readLine().trimmed());
    }

    file.close();
    if(tempText.isEmpty())
        return;
    if(db.isOpen())
    {
        db.close(); //重启db
    }
    if(!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
        return;
    }
    else{
        QString tempdata;
        QStringList singledata;
        QString  select_sql;
        QString  select_sql2;
        QStringList signalcourseID;
        QSqlQuery sql_query;       //数据库操作类

        tempdata=tempText.at(1);
        singledata=tempdata.split(',');
        qDebug()<<singledata;

        qDebug()<<singledata.size();

        for(int i=0;i<4;i++){
            select_sql = QString("select courseID from t_courses where courseName='%1';").arg(singledata.at(5+i));

            if(!sql_query.exec(select_sql))
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                while(sql_query.next())
                {
                    signalcourseID<<sql_query.value(0).toString();

                }

            }
        }
        qDebug()<<signalcourseID;

        for(int i=1;i<tempText.size();i++)
        {
            tempdata=tempText.at(i);
            singledata=tempdata.split(',');
            qDebug()<<singledata;
            select_sql2 = QString("insert into t_stud_info value ('%1','%2','%3','%4','','');").arg(singledata.at(1),singledata.at(2),singledata.at(3),singledata.at(4));
            sql_query.exec(select_sql2);
            qDebug()<<singledata.at(1)<<singledata.at(2)<<singledata.at(3)<<singledata.at(4);

            for(int j=0;j<4;j++){

                select_sql = QString("insert into t_stud_course_info value ('','%1','%2','%3','','');").arg(singledata.at(1),signalcourseID.at(j),singledata.at(5+j));
                qDebug()<<singledata.at(1)<<signalcourseID.at(j)<<singledata.at(5+j);
                if(!sql_query.exec(select_sql))
                {
                    qDebug()<<sql_query.lastError();
                }
                else
                {
                    qDebug()<<"插入成功";
                }
            }
        }
        QMessageBox::information(NULL, tr("Info"),tr("导入成功"));
    }
    OutputTheInfo("t_courses","t_courses.csv");
    OutputTheInfo("t_stud_course_info","t_stud_course_info.csv");
    OutputTheInfo("t_stud_info","t_stud_info.csv");
}

//创建新表或打开
void ScoreWindow::on_exportButton_clicked()
{

   QSqlDatabase ndb = QSqlDatabase::addDatabase("QSQLITE","db2");
   ndb.setDatabaseName("sysdb.sqlite3");
   if(!ndb.open())
   {
       QMessageBox::critical(NULL,tr("错误"),tr("数据库连接失败或超时！"),QMessageBox::Cancel);

       return;
   }
     QFile file1("t_courses.csv"); //打开csv文件
         if(!file1.open(QIODevice::ReadOnly | QIODevice::Text))
          {
          QMessageBox::critical(NULL,tr("错误"),tr("文件打开失败！"),QMessageBox::Cancel);
               return;
          }



         QSqlQuery query1(ndb);  //默认打开
         query1.exec("create table t_courses (courseID int primary key, courseName varchar(25),"
                    "courseCredit int,courseType int)");

         QStringList list1;
         list1.clear();

         QTextStream in1(&file1);  //QTextStream读取数据

         QTextCodec *codec1=QTextCodec::codecForName("utf-8");
         in1.setCodec(codec1);
         while(!in1.atEnd())
         {
            QString fileLine = in1.readLine();  //从第一行读取至下一行
            list1 = fileLine.split(",", QString::SkipEmptyParts);


            query1.prepare("INSERT INTO t_courses (courseID, courseName,courseCredit,courseType) VALUES (:1,:2,:3,:4)"); //准备执行SQL查询
            query1.bindValue(":1", list1.at(0));   //绑定要插入的值
            query1.bindValue(":2", list1.at(1));
            query1.bindValue(":3", list1.at(2));
            query1.bindValue(":4", list1.at(3));

            query1.exec();
         }
         qDebug()<<"成功嵌入";
         query1.clear();


         QFile file2("t_stud_course_info.csv"); //打开csv文件
             if(!file2.open(QIODevice::ReadOnly | QIODevice::Text))
              {
              QMessageBox::critical(NULL,tr("错误"),tr("文件打开失败！"),QMessageBox::Cancel);
                   return;
              }

             QSqlQuery query2(ndb);  //默认打开
             query2.exec("create  table t_stud_course_info (ID int primary key, scNumber int,"
                        "scCourseID int,scScores int,scYear varchar(25),scTerm int)");

             QStringList list2;
             list2.clear();

             QTextStream in2(&file2);  //QTextStream读取数据

             QTextCodec *codec2=QTextCodec::codecForName("utf-8");
             in2.setCodec(codec2);
             while(!in2.atEnd())
             {
                QString fileLine = in2.readLine();  //从第一行读取至下一行
                list2 = fileLine.split(",", QString::SkipEmptyParts);


                query2.prepare("INSERT INTO t_stud_course_info (ID, scNumber,scCourseID,scScores,scYear,scTerm) VALUES (:1,:2,:3,:4,:5,:6)"); //准备执行SQL查询
                query2.bindValue(":1", list2.at(0));   //绑定要插入的值
                query2.bindValue(":2", list2.at(1));
                query2.bindValue(":3", list2.at(2));
                query2.bindValue(":4", list2.at(3));
                query2.bindValue(":5", list2.at(4));
                query2.bindValue(":6", list2.at(5));

                query2.exec();
             }
             qDebug()<<"成功嵌入";
             query2.clear();



             QFile file3("t_stud_info.csv"); //打开csv文件
                 if(!file3.open(QIODevice::ReadOnly | QIODevice::Text))
                  {
                  QMessageBox::critical(NULL,tr("错误"),tr("文件打开失败！"),QMessageBox::Cancel);
                       return;
                  }



                 QSqlQuery query3(ndb);  //默认打开
                 query3.exec("create table t_stud_info (sNumber int primary key, sName varchar(25),"
                            "sMajor varchar(25),sClass varchar(25),sEnrolledYear int,sPhone int)");

                 QStringList list3;
                 list3.clear();

                 QTextStream in3(&file3);  //QTextStream读取数据

                 QTextCodec *codec3=QTextCodec::codecForName("utf-8");
                 in3.setCodec(codec3);
                 while(!in3.atEnd())
                 {
                    QString fileLine = in3.readLine();  //从第一行读取至下一行
                    list3 = fileLine.split(",", QString::SkipEmptyParts);


                    query3.prepare("INSERT INTO t_stud_info (sNumber, sName,sMajor,sClass,sEnrolledYear,sPhone) VALUES (:1,:2,:3,:4,:5,:6)"); //准备执行SQL查询
                    query3.bindValue(":1", list3.at(0));   //绑定要插入的值
                    query3.bindValue(":2", list3.at(1));
                    query3.bindValue(":3", list3.at(2));
                    query3.bindValue(":4", list3.at(3));
                    query3.bindValue(":5", list3.at(4));
                    query3.bindValue(":6", list3.at(5));

                    query3.exec();
                 }
                 qDebug()<<"成功嵌入";
                 query3.clear();

         ndb.close();
          QMessageBox::information(NULL, tr("Info"),tr("导出成功"));
         return;
}
