#define  _CRT_SECURE_NO_WARNINGS 1


#include "BigData.h"


//在_value 和 _str都保存一份数据
BigData:: BigData(const long long data )
:_value(data)
,_str("+0")
{
	if (data == 0)
	{
		return;
	}

	long long tmp = (long long)data;
	if (data < 0)
		tmp = -tmp;
	int Len = (int)log10(tmp) + 1;

	//将数据转化为带符号的字符串
	char * buffer = new char[Len + 2];
	buffer[Len + 1] = '\0';
	int i = 0;
	for (i = 1; i < Len + 1; i++)
	{
		buffer[Len + 2 - 1 - i] = tmp % 10 + '0';
		tmp /= 10;
	}

	if (data < 0)
		buffer[0] = '-';
	else
		buffer[0] = '+';

	_str = buffer;

	delete[] buffer;

}

///**************************************************************
//****** 函数名：BigData(const char*)
//****** 逻辑描述：
//******	(1)没有溢出在_value 和 _str 各保存一份数据
//******	(2)溢出时将 _value置0，_str 保存数据
//***************************************************************/
//BigData:: BigData(const char* pData)
//{
//	assert(pData != NULL);
//	char* tmp = (char*)pData;
//
//	//处理符号
//	char sign = ' ';
//	if (*tmp == '-' || *tmp == '+')
//	{
//		sign = *tmp++;
//	}
//	else if (*tmp >= '0' || *tmp <= '9')
//		sign = '+';
//	else   //非法数据
//	{
//		throw;
//	}
//
//	//去掉前置0
//	while (*tmp == '0')
//		tmp++;
//
//	if (*tmp == '\0')
//	{
//		_str = "+0";
//		_value = 0;
//		return;
//	}
//
//	//处理非法字符
//	int iCount = 0;
//	for (size_t i = 0; i < strlen(tmp); i++)
//	{
//		if (tmp[i] < '0' || tmp[i] > '9')
//			break;
//		iCount++;
//	}
//
//	//
//	char * buffer = new char[iCount +2];
//	buffer[1] = '\0';
//	buffer[0] = sign;
//	strncat(buffer, tmp, iCount);
//
//	_str = buffer;
//
//	if (_str.size() < INT64LEN)
//		_value = _atoi64(buffer);
//	else
//		_value = 0;
//
//	delete[] buffer;
//
//}

/**************************************************************
****** 函数名：BigData(const string&)
****** 逻辑描述：
******	(1)没有溢出在_value 和 _str 各保存一份数据
******	(2)溢出时将 _value置0，_str 保存数据
***************************************************************/
BigData::BigData(const string& sData)
{
	assert(sData.size());
	char* tmp = (char*)sData.c_str();

	//处理符号
	char sign = ' ';
	if (*tmp == '-' || *tmp == '+')
	{
		sign = *tmp++;
	}
	else if (*tmp >= '0' || *tmp <= '9')
		sign = '+';
	else   //非法数据
	{
		throw;
	}

	//去掉前置0
	while (*tmp == '0')
		tmp++;

	if (*tmp == '\0')
	{
		_str = "+0";
		_value = 0;
		return;
	}

	//处理非法字符
	int iCount = 0;
	for (size_t i = 0; i < strlen(tmp); i++)
	{
		if (tmp[i] < '0' || tmp[i] > '9')
			break;
		iCount++;
	}

	//
	char * buffer = new char[iCount + 2];
	buffer[1] = '\0';
	buffer[0] = sign;
	strncat(buffer, tmp, iCount);

	_str = buffer;

	if (_str.size() <= INT64LEN)
		_value = _atoi64(buffer);
	else
		_value = 0;

	delete[] buffer;

}




ostream& operator<< (ostream& _cout, const BigData& data)
{
	char* tmp = (char*)data._str.c_str();
	if (*tmp == '+')
		_cout << tmp + 1;
	else
		_cout << data._str;
	return _cout;
}

istream& operator>> (istream& _cin,  BigData& data)
{
	string tmp;
	_cin >> tmp;
	data = BigData(tmp);
	return _cin;
}


/************************************************************************
******** 函数名：operator +
******** 逻辑描述：
********	(1)没有溢出直接相加
********	(2)溢出调用Add()
**************************************************************************/
BigData BigData:: operator+ (const BigData& data)
{
	BigData left = *this;
	BigData right = data;


	//两个都没溢出
	if (left._str.size() <= INT64LEN && right._str.size() <= INT64LEN)
	{
		long long ret = left._value + right._value;
		return BigData(ret);
	}
	else//至少有一个溢出
	{
		string ret = Add(left._str ,right._str);
		return BigData(ret.c_str());
	}
}


/**************************************************************
****** 函数名：Add
****** 逻辑描述：
******	(1)异号相加调用Sub()  如：3+(-5)  -->3-5
******	(2)同号依次从最低位开始相加
***************************************************************/
string BigData::Add(const string& data1, const string& data2)
{
	string left = (string)data1;
	string right = (string)data2;
	

	//异号相加
	if (left[0] != right[0])
	{
		if (left[0] == '+')  //正加负  3 +(-5)  -->3 - 5
		{
			AlterSign(right);
			return Sub(left, right);
		}
		else  //  负加正  -5 +3   -->3 -5
		{
			AlterSign(left);
			return Sub(right, left);
		}
	}
	else  //同号相加
	{
		return AddForSameSign(left, right);
	}
}


string BigData:: AddForSameSign(const string& data1, const string& data2)
{
	string left = data1, right = data2;
	if (data1.size() < data2.size())
	{
		swap(left, right);
	}
	
	string ret;
	ret.resize(left.size() + 1);
	ret[0] = data1[0];

	size_t i = 1;
	int cFlag = 0;
	size_t LSize = left.size(), RSize = right.size();
	size_t size = ret.size();
	for (i = 1; i < LSize; i++)
	{
		ret[size - i] = left[LSize - i] + cFlag;   //加左操作数 加进位
		if (i < right.size())
		{
			ret[size - i] += right[RSize - i] - '0';  //加右操作数	
		}

		if (ret[size - i] > '9')  //处理进位
		{
			cFlag = 1;
			ret[size - i] -= 10;
		}
		else
			cFlag = 0;
	}
	ret[1] = cFlag + '0';  //处理和的最高位


	return ret;
}


/************************************************************************
******** 函数名：operator -
******** 逻辑描述：
********	(1)没有溢出直接相减
********	(2)溢出调用Sub()
**************************************************************************/
BigData BigData:: operator- (const BigData& data)
{
	BigData minu = *this;
	BigData subs = data;

	//两个都没溢出
	if (minu._str.size() <= INT64LEN && subs._str.size() <= INT64LEN)
	{
		long long ret = minu._value - subs._value;
		return BigData(ret);
	}
	else
	{
		string ret = Sub(minu._str, subs._str);
		return BigData(ret.c_str());
	}
}


/*****************************************************************
****** 函数名：Sub
****** 逻辑描述：
******	(1)异号相减调用Add  如：-3-5  —>-3 +(-5)
******	(2)同号正数相减长减短直接减，短减长如3 - 15  —> -(15-3)
******	(3)同号负数相减，绝对值相减再加负号 -3 -(-15) —>-(3-15)
*****************************************************************/
string BigData:: Sub(const string&data1, const string& data2)
{
	string minu = data1;
	string subs = data2;

	if (minu[0] != subs[0])   //异号相减
	{
		AlterSign(subs);//-5 - 3   -->-5 + (-3)
		return Add(minu, subs);
	}
	else if(minu[0] == '+')//两正数相减
	{
		if (minu.size() >=subs.size())  //长减短
			return SubForSameSign(minu, subs);
		
		//短减长
		return AlterSign( Sub(subs, minu));
		
	}
	else //两负数相减
	{
		AlterSign(subs);  //-3 - (-5) -->-(3-5)
		AlterSign(minu);
		return AlterSign(Sub(minu, subs));
	}
	
}
//同号相减且是长减短
string BigData:: SubForSameSign(const string&data1, const string& data2)
{
	if (data1.size() < data2.size())
		throw;
	string minu = data1;
	string subs = data2;
	string ret = minu;
	int  cFlag = 0;   //借位标志
	size_t i = 1;
	for (i = 1; i < ret.size(); i++)
	{
		ret[ret.size() - i] -= cFlag;  //减进位
		if (i < subs.size())
			ret[ret.size() - i] -= subs[subs.size() - i]- '0'; // 减 减数

		//处理借位标志
		if (ret[ret.size() - i] < '0')
		{
			ret[ret.size() - i] += 10;
			cFlag = 1;
		}
		else
			cFlag = 0;

	}
	if (cFlag == 1)
		AlterSign(ret);
	return ret;
}




//符号位取反  "+12345" --> "-12345"
string BigData::AlterSign(string& data)
{
	if (data[0] == '+')
		data[0] = '-';
	else
		data[0] = '+';

	return data;
}
BigData BigData::AlterSign(BigData& data)
{
	string str = data._str;
	AlterSign(str);

	return BigData(str);
}



BigData BigData:: operator* (const BigData& data)
{
	size_t size =_str.size()+data._str.size()-1;
	
	BigData  ret = 0 ;

	if (_str[1] == '0' || data._str[1] == '0')
		return ret;

	//没有溢出
	if (size < INT64LEN)
		return BigData(_value * data._value);

	else
		return BigData(_StrMulStr(_str, data._str));

}

string BigData:: _StrMulStr( const string& str1,  const string& str2)
{
	string left = str1;
	string right = str2;
	if (left.size() < right.size())
		swap(left, right);

	string ret ;
	ret.resize(1);
	ret[0] = right[0];
	//memset((char*)ret.c_str() + 1, '0', ret.size() - 1);

	if (left[0] != right[0])
		AlterSign(left);

	int flag = 0;
	for (int i = right.size()-1; i >0; --i)
	{
		string tmp;
		tmp = _StrMulChar(left, right[i]);
		tmp.resize(tmp.size() + flag);
		for (int j = flag; j > 0;j--)
		{
			tmp[tmp.size() - j] = '0';	
		}

		tmp[0] = ret[0];
		ret = AddForSameSign(ret, tmp);
		flag++;
	}

	//处理符号
	if (str1[0] != str2[0])
		ret[0] = '-';
	else
		ret[0] = '+';

	return ret;
}


int BigData:: UnsignedStringCmp(const string& s1, const string& s2)
{
	string str1 = BigData(s1)._str;
	string str2 = BigData(s2)._str;

	if (str1.size() > str2.size())
		return 1;
	else if (str1.size() < str2.size())
		return -1;
	else
		return strcmp(str1.c_str() + 1, str2.c_str() + 1);

}




string BigData:: _StrMulChar(const string& str, char ch)
{
	if (ch > '9' || ch < '0')
		throw;

	ch -= '0';
	string ret;
	ret.resize(str.size() + 1);
	ret[0] = str[0];

	size_t cFlag = 0;
	for (int i = str.size() - 1; i > 0; i--)
	{
		ret[i + 1] = cFlag + (str[i] - '0') *ch;
		cFlag = 0;
		while (ret[i+1] > 9)
		{
			ret[i + 1] -= 10;
			cFlag++;
		}
		ret[i + 1] += '0';
	}
	ret[1] = cFlag + '0';

	return ret;
}




BigData BigData:: operator/ (const BigData& data)
{
	if (data._str[1] == 0)
		throw;

	//没溢出
	if (_str.size() < INT64LEN && data._str.size() < INT64LEN)
		return BigData(_value / data._value);

	//溢出
	if (_str.size() == data._str.size())
	{

		if (strcmp(_str.c_str() + 1, data._str.c_str() + 1) < 0)
			return BigData("+0");
		else
			return _StrDivStr(_str, data._str);

	}
	else if (_str.size() < data._str.size())
		return BigData("+0");
	else
		return _StrDivStr(_str, data._str);
		
}


//两字符串相除 被除数 > 除数
string BigData:: _StrDivStr(const string& data1, const string& data2)
{
	string div = data1;        //被除数
	string data = data2;     //除数
	if (div[0] != '+')
		div[0] = '+';
	if (data[0] != '+')
		data[0] = '+';

	string ret;		//商
	ret.resize(div.size());  //两个参数
	memset((char*)ret.c_str() + 1, '0', ret.size() - 1);
	
	int len = data.size() - 1;
	int start = 1, end = start +len-1;  
	string TmpDiv ;
	while (end < div.size())
	{
		TmpDiv = GetString(div, start, end);
		//取出的被除数小于除数
		if (UnsignedStringCmp(TmpDiv, data) < 0)
		{
			if (end == div.size() - 1)
				break;

			end++;
		    TmpDiv = GetString(div, start, end);
		}

		ret[end] = LoopSubN(TmpDiv, data);
		PutString(div, TmpDiv,start,end);

		//跳过前置0
		while (start <div.size()-1)
		{
			if (div[start] == '0')
				start++;
			else
				break;
		}

		while (end<div.size()-1 && end - start < len-1)
		{
			end++;
		}
	}

	
	if (data1[0] != data2[0])
		ret[0] = '-';
	else
		ret[0] = '+';

	return  BigData(ret)._str;
}




char BigData:: LoopSubN(string &s1, const string& s2)
{
	char ret = '0';

	while (UnsignedStringCmp(s1,s2)>=0)
	{
		s1 = SubForSameSign(s1, s2);
		ret++;
	}
	
	return ret;
}




string BigData::GetString(string str, size_t start, size_t end)
{
	if (start > end || start >= str.size() || end > str.size())
		throw __LINE__;

	string ret;
	ret.resize(end - start + 2);
	ret[0] = str[0];
	size_t count = 1;
	while (start <= end)
	{
		ret[count++] = str[start++];
	}

	return ret;
}


void BigData:: PutString(string& dest, const string& src,size_t start,size_t end)
{
	int count = 1;
	while (count < src.size())
	{
		dest[start++] = src[count++];
	}
}




