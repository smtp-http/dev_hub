#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <fstream>

#include <sstream>
#include <unordered_map>
#include <vector>
#include <iostream>



//using namespace std;

namespace lux {

typedef std::string String;
class Device
{
public:
	//I: ...
	struct I_data
	{
		//Bus=<Bus>
		String Bus;
		//Vendor=<Vendor>
		String Vendor;
		//Product=<Product>
		String Product;
		//Version=<Version>
		String Version;
	} I;
	//N: Name=<N>
	String N;
	//P: Phys=<P>
	String P;
	//S: Sysfs=<S>
	String S;
	//U: Uniq=<U>
	String U;
	//H: Handlers=<H>
	std::vector<String> H;
	//B: <first>=<second>
	std::unordered_map<String,String> B;
	
	// parse the s**t out of it. Not the most reliable parser, not even close,
	// but it works for now!
	// returns true if there is more to read!
	bool read(std::ifstream& f)
	{
		String line;
		while(std::getline(f,line)){
			if(line[0] == 'I'){
				line = line.substr(line.find_first_of('=')+1);
				I.Bus = line.substr(0,line.find_first_of(' '));
				line = line.substr(line.find_first_of('=')+1);
				I.Vendor = line.substr(0,line.find_first_of(' '));
				line = line.substr(line.find_first_of('=')+1);
				I.Product = line.substr(0,line.find_first_of(' '));
				line = line.substr(line.find_first_of('=')+1);
				I.Version = line.substr(0,line.find_first_of(' '));
			} else if(line[0] == 'N') {
				line = line.substr(line.find_first_of('=')+1);
				N = line;
			} else if(line[0] == 'P') {
				line = line.substr(line.find_first_of('=')+1);
				P = line;
			} else if(line[0] == 'S') {
				line = line.substr(line.find_first_of('=')+1);
				S = line;
			} else if(line[0] == 'U') {
				line = line.substr(line.find_first_of('=')+1);
				U = line;
			} else if(line[0] == 'H') {
				line = line.substr(line.find_first_of('=')+1);
				std::stringstream ss(line);
				String h;
				while(getline(ss,h,' ')) {
					H.push_back(h);
				}
			} else if(line[0] == 'B') {
				line = line.substr(line.find_first_of(' ')+1);
				String name = line.substr(0,line.find_first_of('='));
				String valu = line.substr(line.find_first_of('=')+1);
				B[name] = valu;
			} else
				break;
		}
		return !f.eof();
	}
	
	// converts this Device back to the string it was parsed from.
	String to_string() const
	{
		String res = "I: ";
		res+="Bus="+I.Bus+" Vendor="+I.Vendor+" Product="+
				I.Product+" Version="+I.Version+"\n";
		res+="N: Name="+N+"\n";
		res+="P: Phys="+P+"\n";
		res+="S: Sysfs="+S+"\n";
		res+="U: Uniq="+U+"\n";
		res+="H: Handlers=";
		for(const auto& h : H) {
			res+=h+" ";
		}
		res+="\n";
		for(const auto& b : B) {
			res+="B: "+b.first+"="+b.second+"\n";
		}
		res+="\n";
		return res;
	}
	
	// searches for an handler starting with <starts_with> and returns it.
	// if none is found an empty string is returned.
	String get_handler_starting_with(const String& starts_with) const
	{
		for(const auto& h : H) {
			if(h.find_first_of(starts_with) == 0)
				return h;
		}
		return "";
	}
};

};

//std::vector<Device> get_devices(const String& p="/proc/bus/input/devices");




#endif
