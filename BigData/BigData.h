#pragma once

#include<iostream>
#include <string>

#include <math.h>
#include <string.h> //strlen / strncat
#include <assert.h> //assert



#define INT64LEN 19



using namespace std;

class BigData
{
public:
	BigData(const string& sData);
	BigData(const long long data = 0);
	//BigData(const char* pData);

	friend ostream& operator<< (ostream& _cout, const BigData& data);
	friend istream& operator>> (istream& _cin,  BigData& data);

	BigData operator+(const BigData& data);
	BigData operator- (const BigData& data);
	BigData operator* (const BigData& data);
	BigData operator/ (const BigData& data);


	int BigData::UnsignedStringCmp(const string& s1, const string& s2);




private:
	string Add(const string& left, const string& right);
	string AddForSameSign(const string& data1, const string& data2);

	string Sub(const string &data1, const string & data2);
	string SubForSameSign(const string&data1, const string& data2);

	string _StrMulStr(const string& str1, const string& str2);
	string _StrMulChar(const string& str, char ch);

	string _StrDivStr(const string& data1, const string& data2);
	string GetString(string str, size_t start, size_t end);
	void PutString(string& dest, const string& src,size_t start,size_t end);
	char LoopSubN(string &s1, const string& s2);





	//改变字符串的符号
	string AlterSign(string& data);
	BigData AlterSign(BigData& data);



private:
	long long _value;
	string  _str;     //保存带符号的数据

};





