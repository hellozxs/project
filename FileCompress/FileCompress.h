#pragma once

#include "Huffman.hpp"
#include <string>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef unsigned long LongType;
struct CharInfo
{
	unsigned char _ch;  //字符
	LongType _count;   //出现次数
	string _code;   //编码

	CharInfo(LongType count = 0)
		:_count(count)
	{}

	bool operator != (const CharInfo& ci)const
	{
		return ci._count != _count;
	}

	CharInfo operator+(const CharInfo& ci)
	{
		return CharInfo(_count + ci._count);
	}

	bool operator<(const CharInfo& ci)
	{
		return _ch < ci._ch;
	}
};


class FileCompress
{
public:
	FileCompress()
	{
		//初始化_infos
		int i = 0;
		for (i = 0; i < 256; i++)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}

	}



	
	void Compress(string fileName)
	{
		//统计字符
		FILE* pFile = fopen(fileName.c_str(), "rb");
		assert(pFile);
		int ch = fgetc(pFile);
		while (ch != EOF)
		{
			_infos[ch]._count++;
			ch = fgetc(pFile);
		}

		fclose(pFile);

		

		//构建哈夫曼树
		HuffmanTree<CharInfo> ht;
		Node* root = ht.CreatTree(_infos, 256, 0);

		//生成哈夫曼编码
		string code;
		GenerateHuffmanCode(root, code);


		//写配置文件
		string configName = fileName + ".config";
		FILE* pConfig = fopen(configName.c_str(), "wb");
		assert(pConfig);

		LongType count = root->_value._count;
		while (count)
		{
			fputc(count%10, pConfig);
			count /= 10;
		}
		fputc('\n', pConfig);

		int i = 0;
		for (i = 0; i < 256; i++)
		{
			if (_infos[i]._count != NULL)
			{
				fputc(_infos[i]._ch, pConfig);
				LongType tmp = _infos[i]._count;
				while (tmp)
				{
					cout << tmp % 10 << "	";
					fputc(tmp%10 , pConfig);
					tmp /= 10;
				}
				fputc('\n', pConfig);
			}
		}
		fclose(pConfig);


		 pFile = fopen(fileName.c_str(), "rb");
		assert(pFile);
		string compName = fileName + ".compress";
		FILE* pCompress = fopen(compName.c_str(),"wb");
		assert(pCompress);

		unsigned char value = 0;
		int len =8;
		int tmp = fgetc(pFile);
		while (tmp != EOF)
		{
			int CodeSize = _infos[tmp]._code.size();
			string& code = _infos[tmp]._code;
			int i = 0;
			while (--CodeSize>=0)
			{
				value <<= 1;
				value |=code[i++]-'0';
				len--;
				if (len <1)
				{
					fputc(value, pCompress);
					value = 0;
					len = 8;
				}	
			}
			i = 0;
			tmp = fgetc(pFile);	
		}

		if (len >= 0)
		{
			value <<= len;
			fputc(value, pCompress);
		}

		fclose(pCompress);
		fclose(pFile);
	}

	void UnCompress(string compName)
	{
		FILE* pCompress = fopen(compName.c_str(), "rb");
		assert(pCompress);

		string configName = compName;
		configName.resize(configName.size() - 9);
		configName += ".config";
		string UnCompName = compName;
		UnCompName.resize(UnCompName.size() - 9);
		UnCompName += ".uncompress";

		FILE* pUnCompress = fopen(UnCompName.c_str(), "wb");
		assert(pUnCompress);
		FILE* pConfig = fopen(configName.c_str(), "rb");
		assert(pConfig);
		
		//读配置文件
		int i = 0;
		int ch = 0;
		LongType all  = fgetc(pConfig);
		while ((ch = fgetc(pConfig)) != '\n')
		{
			all =  all + ch *pow(10,++i);

		}
		i = 0;

		while ( (ch = fgetc(pConfig) )!= EOF)
		{
			LongType &count = _infos[ch]._count;
			char tmp = fgetc(pConfig);
			count = tmp;
			while ((tmp = fgetc(pConfig)) != '\n')
			{
				count = count + tmp*pow(10,++i);
			}
			i = 0;
		}


		//构建哈夫曼树
		HuffmanTree<CharInfo> ht;
		Node* root = ht.CreatTree(_infos, 256, 0);

		//生成哈夫曼编码
		string code;
		GenerateHuffmanCode(root, code);

		 ch = fgetc(pCompress);
		 all = root->_value._count;
		Node* tmp = root;
		while ( all > 0 )
		{
			int i = 0;
			for (i = 7; i >=0; i--)
			{
				if (root->_left == NULL && root->_right == NULL)
				{
					fputc(root->_value._ch, pUnCompress);
					all--;
					if (all <= 0)
						break;
					root = tmp;
				}

				if ((ch & (1 << i)) == 0)   //00001010 & 00001000 == 8
					root = root->_left;
				else
					root = root->_right;
			}
			ch = fgetc(pCompress);
			
		}

		fclose(pUnCompress);
		fclose(pCompress);
	}
	

private:
	//生成哈夫曼编码
	void GenerateHuffmanCode(const HuffmanNode<CharInfo>* root, string code)
	{
		if (root == NULL)
			return;

		if (root->_right == NULL && root->_right == NULL)
		{
			_infos[root->_value._ch]._code = code;
		}

		GenerateHuffmanCode(root->_left, code + '0');
		GenerateHuffmanCode(root->_right, code + '1');

	}

	
	
	void _ClearTree(HuffmanNode<CharInfo>* &root)
	{
		if (root == NULL)
			return;
		_ClearTree(root->_left);
		_ClearTree(root->_right);
		if (root != NULL)
		{
			delete root;
			root = NULL;
		}
	}

	typedef HuffmanNode<CharInfo> Node;
private:
	CharInfo _infos[256];
};
