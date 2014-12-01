/* 
 * File:   main.cpp
 * Author: gmorgan
 *
 * Created on November 26, 2014, 6:22 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <vector>
#include <sstream>

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
	std::string fname;
	std::string lname;      
    
	/*
	 * 
	 */
	Name (const std::string &name) {
        std::vector<std::string> f = split(name, ' ');

        if (f.size() > 0) {
            fname = f[0];
            if (f.size() > 1) {
                lname = f[1];
            } else {
               lname = "";
            }
        } else {
            fname = "";
            lname = "";
        }
    }
    
     std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
       std::stringstream ss(s);
       std::string item;
       while (std::getline(ss, item, delim)) {
           elems.push_back(item);
       }
       return elems;
    }   
    
    /**
     * 
     */
    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    } 
    
};

/**
 * 
 */
class NameList {
public:
	
	std::list<Name> names;	

	/**
	 * Filename constructor 
	 */
	NameList (const char * fname) {
		Name * name;
        std::string ln;
		std::string line;
		std::ifstream inStream;
		
		cout << "Opening " << *fname << " for reading." << endl;
		
		inStream.open(fname, std::ifstream::in);	
		
		if (inStream.is_open()) {
            while (getline(inStream, line)) {
                if (line.length() == 0) {
                    continue;
                }

                // trim 
                std::size_t first = line.find_first_not_of(' ');
                std::size_t last  = line.find_last_not_of(' ');              
                ln = line.substr(first, last - first + 1);

                name = new Name(ln);
                names.push_back(*name);
            }
            inStream.close();
		} else {
			cout << "Unable to open file"; 
		}
	};
    
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
std::ostream& operator<<(std::ostream &ostream, const Name &name) {
    std::string delim = " ";
    if (name.lname.compare("") == 0) {
        delim = "";   
    }
	return ostream << "{" <<  name.fname << delim << name.lname << "}";
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
std::ostream& operator<<(std::ostream &ostream, const NameList &nameList) {
	std::list<Name>::const_iterator i;
	for (i = nameList.names.begin(); i != nameList.names.end(); i++) {
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

/** 
 * trim helper
 */
std::string trim(std::string const& str) {
    std::size_t first = str.find_first_not_of(' ');
    std::size_t last  = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);
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

	NameList nameList = NameList("infile.txt");
	
    cout << nameList << endl;
    
	cout << "Done." << endl;

	return 0;
}

// end file