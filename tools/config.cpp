#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "config.h"

using namespace std;
using namespace lux;

//#define CONFIG_FILE "config.json"
#define CONFIG_FILE "new_config.json"

//从文件中读取JSON
int SysConfig::LoadUserConfig(IConfigUser &user)
{
	Json::Reader reader;
	Json::Value root;
	

	ifstream in(CONFIG_FILE, ios::binary);
	
	if( !in.is_open() ) { 
		cout << "Error opening file\n"; 
		return -__LINE__; 
	}

	if(reader.parse(in,root)) {printf("%s +++++++ start %d ++++++  \n",__FILE__,__LINE__);
		user.Update(root);
	} else {
		cout << "parse error\n" << endl;
		return -__LINE__;
	}

	return 0;
}



int SysConfig::LoadConfig()
{
	Json::Reader reader;
	Json::Value root;
	list <IConfigUser *>::iterator iter;
	IConfigUser *user;

	//从文件中读取，保证当前文件有test.json文件
	ifstream in(CONFIG_FILE, ios::binary);
	//in.open("test.json", ios::binary);

	if( !in.is_open() ) { 
		cout << "Error opening file\n"; 
		return -__LINE__; 
	}


	if(reader.parse(in,root)) {
		
		for (iter = m_users.begin();iter != m_users.end();iter ++) { 
			user = *iter;
			if (user != NULL)
				user->Update(root);
		}

		uperComputerIp = root["UperComputerIp"].asString();
		uperComputerPort = root["UperComputerPort"].asInt();
		
#if 0
	//读取根节点信息
		string name = root["name"].asString();
		int age = root["age"].asInt();
		bool sex_is_male = root["sex_is_male"].asBool();

		cout << "My name is " << name << endl;
		cout << "I'm " << age << " years old" << endl;
		cout << "I'm a " << (sex_is_male ? "man" : "woman") << endl;

	//读取子节点信息
		string partner_name = root["partner"]["partner_name"].asString();
		int partner_age = root["partner"]["partner_age"].asInt();
		bool partner_sex_is_male = root["partner"]["partner_sex_is_male"].asBool();

		cout << "My partner's name is " << partner_name << endl;
		cout << (partner_sex_is_male ? "he" : "she") << " is "
		<< partner_age << " years old" << endl;

	//读取数组信息
		cout << "Here's my achievements:" << endl;
		for(unsigned int i = 0; i < root["achievement"].size(); i++) {
			string ach = root["achievement"][i].asString();
			cout << ach << '\t';
		}
		cout << endl;

		cout << "Reading Complete!" << endl;
#endif
	} else {
		cout << "parse error\n" << endl;	
	}

	in.close();
	return 0;
}

SysConfig& SysConfig::Instance()
{
	static SysConfig _instance;
	return _instance;
}

void SysConfig::AddUser(IConfigUser *u)
{
	if (u != NULL)
		m_users.push_back(u);
}


void SysConfig::DelUser(IConfigUser *u)
{
	m_users.erase(find(m_users.begin(), m_users.end(), u));
}



