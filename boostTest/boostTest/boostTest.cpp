// boostTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include<boost/thread.hpp>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;
using namespace std;
#include <map>
#include <vector>

void mythread()
{
	int i = 0;
	while (i<1000)
	{
		i++;
		cout << i << endl;
		cout << "\n" << endl;
	}	
}
void test_thread()
{
	boost::thread t(mythread);
	t.join();
	cout << " thread is over! " << endl;
}
void test_json()
{
	string stra, strc;
	vector<string> vecStr;
	vector<string> vecStr1;
	ptree pt, p1, p2;
	read_json<ptree>("config.json", pt);
	p1 = pt.get_child("people");
	for (ptree::iterator it = p1.begin(); it != p1.end(); ++it)
	{
		p2 = it->second; 
		stra = p2.get<string>("email");//firstName,lastName,email
		vecStr.push_back(stra);
	}
}
//数据的插入，第一种方式：用insert函数插入pair数据
void test_map1()
{
	map<int, string> mapStudent;
	mapStudent.insert(pair<int, string>(1, "student1"));
	mapStudent.insert(pair<int, string>(2, "student2"));
	mapStudent.insert(pair<int, string>(3, "student3"));
	mapStudent.insert(pair<int, string>(4, "student4"));
	mapStudent.insert(pair<int, string>(5, "student5"));
	int nSize = mapStudent.size();

	map<int, string>::iterator iter;
	for (iter = mapStudent.begin();iter != mapStudent.end();iter++)
	{
		cout << iter->first << " " << iter->second << endl;

	}
}
//第二种方式，用insert函数插入value_type数据，
void test_map2()
{
	map<int, string> mapStudent;
	mapStudent.insert(map<int, string>::value_type(1, "student1"));
	mapStudent.insert(map<int, string>::value_type(2, "student2"));
	mapStudent.insert(map<int, string>::value_type(3, "student3"));
	mapStudent.insert(map<int, string>::value_type(4, "student4"));
	mapStudent.insert(map<int, string>::value_type(5, "student5"));
	int nSize = mapStudent.size();

	map<int, string>::iterator iter;
	for (iter = mapStudent.begin();iter != mapStudent.end();iter++)
	{
		cout << iter->first << " " << iter->second << endl;

	}

}
// map删除数据
void test_map3()
{
	map<int, string>mapStudent;
	mapStudent[1] = "student1";
	mapStudent[2] = "student2";
	mapStudent[3] = "student3";
	mapStudent[4] = "student4";
	mapStudent[5] = "student5";
	int nSize = mapStudent.size();

	map<int, string>::iterator iter;
	for (iter = mapStudent.begin();iter != mapStudent.end();iter++)
	{
		cout << iter->first << " " << iter->second << endl;
	}
	//数组删除
	iter = mapStudent.find(3);
	mapStudent.erase(iter);
	cout << "\ndelete somethings" << endl;
	for (iter = mapStudent.begin();iter != mapStudent.end();iter++)
	{
		cout << iter->first << " " << iter->second << endl;
	}
}
typedef struct tagStudentinfo
{
	int      niD;
	string   strName;
	bool operator < (tagStudentinfo const& _A) const
	{     //这个函数指定排序策略，按niD排序，如果niD相等的话，按strName排序  

		if (niD < _A.niD) return true;
		if (niD == _A.niD)
			return strName.compare(_A.strName) < 0;
		return false;
	}
}Studentinfo, *PStudentinfo; //学生信息 
void test_map4()
{
	
	map<Studentinfo, int>mapStudent;
	map<Studentinfo, int>::iterator iter;
	Studentinfo studentinfo;
	studentinfo.niD = 1;
	studentinfo.strName = "student1";
	mapStudent.insert(pair<Studentinfo, int>(studentinfo, 90));
	studentinfo.niD = 2;
	studentinfo.strName = "student_two";
	mapStudent.insert(pair<Studentinfo, int>(studentinfo, 80));
	for (iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

		cout << iter->first.niD << ' ' << iter->first.strName << ' ' << iter->second << endl;

}
//map的基本函数
//begin(),clear(),count(),empty,end,equal_range,erase,find,insert,
void test_map5()
{
	map<int, string>mapStudent;
	mapStudent[1] = "student1";
	mapStudent[2] = "student2";
	mapStudent[3] = "student3";
	mapStudent[4] = "student4";
	mapStudent[5] = "student5";
	//元素的个数
	int nSize = mapStudent.size();

	map<int, string>::iterator iter;
	
	//查找&删除
	iter = mapStudent.find(3);
	mapStudent.erase(iter);
	//插入
	mapStudent.insert(pair<int, string>(9, "student9"));
	mapStudent.insert(pair<int, string>(9, "student9"));
	//获取某个元素的个数
	int num = mapStudent.count(9);
	cout << "num 9个数为"<<num<<endl;
	//清理掉所有的数据
	mapStudent.clear();
}

void printMsgs(void)

{
	 

}

class student {
public:
	student() {};
		int age;
};
void test_vector()
{
	vector<int> str;
	for (int  i = 0; i < 10; i++)
	{
		str.push_back(i);
		cout << i << endl;
	}

}


int main()
{
	//test_thread();
	//test_json();
	//test_map1();
	//test_map2();
	//test_map3();
	//test_map4();
	//test_map5();
	test_vector();
	return 0;
}

