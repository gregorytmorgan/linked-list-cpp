/* 
 * File:   main.cpp
 * Author: gmorgan
 *
 * Created on November 26, 2014, 6:22 PM
 */

#include <cstdlib>
#include <iostream>
#include <list>
#include <string>

using namespace std;

/**
 *
 */
class CmdLineArg {
public:
	std::string *flag;
	std::string *value;
};

/**
 *
 */
class Name {
public:
	std::string *fname;
	std::string *lname;
};

/**
 * 
 */
class NameList {
public:

	NameList (FILE *fp);

	NameList (std::list *l);
	
	NameList (char **string_array);

	std::list<Name> nlist;
};


/**
 *
 */
std::ostream& operator<<(std::ostream &ostream, const CmdLineArg &cmdLineArg) {
	string flag;
	string value;

	if (cmdLineArg.flag == NULL) {
		flag = string("-na-");
	} else {
		flag = *cmdLineArg.flag;
	}

	if (cmdLineArg.value == NULL) {
		value = string("-na-");
	} else {
		value = *cmdLineArg.value;
	}

	return ostream << "{flag:" <<  flag << ", Value:" << value << "}";
}

/**
 *
 */
std::ostream& operator<<(std::ostream &ostream, const list<CmdLineArg> &argList) {
	std::list<CmdLineArg>::const_iterator i;
	for (i = argList.begin(); i != argList.end(); i++) {
		ostream << *i << endl;
	}
	return ostream;
}

/**
 *
 */
bool parseArgs(int argc, char **argv, std::list<CmdLineArg> *argList) {
	int i;

	cout << "parseArgs - entry" << endl;

	CmdLineArg *arg = NULL;

	for (i = 1; i < argc; i++) {

		arg = new CmdLineArg;

		//
		// this could be a lot cleaner
		//
		if (argv[i][0] == '-') {

			if (((std::string)argv[i]).length() > 1){

				// just an flag, no value
				if (i + 1 < argc && argv[i + 1][0] == '-') {
					arg->flag = new string(&argv[i][1]);
					arg->value = NULL;
					argList->push_back(*arg);
					continue;
				}				
				
				arg->flag = new string(&argv[i][1]);

				if (i++ < argc) {
					arg->value = new string(&argv[i][0]);
				} else {
					arg->value = NULL;
				}

			} else {
				arg->flag = NULL;
				arg->value = new string(&argv[i][0]);
			}

		} else {
			arg->flag = NULL;
			arg->value = new string(&argv[i][0]);
		}

		argList->push_back(*arg);
	}

	cout << "parseArgs - exit" << endl;

	return true;
}




/*
 * 
 */
int main(int argc, char** argv) {

	list<CmdLineArg> argList;

	if (!parseArgs(argc, argv, &argList)) {
		exit(1);
	}

	cout << "Arg List:" << endl;
	cout << argList << endl;

	cout << "Done." << endl;

	return 0;
}

// end file