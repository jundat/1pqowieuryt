#ifndef __BASE64_H__
#define __BASE64_H__

#include <string>
using namespace std;

class Base64
{
public:
	static std::string encode(unsigned char const* , unsigned int len);
	static std::string decode(std::string const& s);
};


#endif //__BASE64_H__