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


int main()
{
	//test_thread();
	test_json();
	
	return 0;
}

