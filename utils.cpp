#include "utils.h"


using namespace std;

string char_to_str(char *id)
{
	string new_string(id);

	return new_string;
}

bool is_primitive(string id)
{
    if(!id.compare("integer") || !id.compare("boolean"))
    {
        return true;
    }
    else
    {
        return false;
    }
}