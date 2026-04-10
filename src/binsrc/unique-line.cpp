
//确保行内容唯一
//和shell里sort -u类似，但sort -u常常出问题，行为和预期不符且破坏性


#include <iostream>
#include <string>
#include <unordered_set>
#include <fstream>


using std::cout,std::cerr,std::endl;

using std::string;
using std::unordered_set;
using std::ifstream;

int main(int argc ,char *argv[]) try {
	unordered_set<string> result{};
	ifstream ifs{};

	if (argc == 2){
		ifs.open(argv[1],std::ios_base::in);

		if (!ifs.is_open()){
			throw std::runtime_error{"Failed to open the file."};
		}

	}else{
		throw std::runtime_error{"Too many or too few arguments."};
	}

	for(string line;true;){

		if(getline(ifs,line,'\n')){

			auto is_find = result.find(line);
			if(is_find == result.end()){
				result.emplace(line);
			}

		}else if(!ifs.eof()){
			throw std::runtime_error{"Error: Didn't get to EOF."};
			break;

		}else{break;}

	}

	for(auto line : result){
		cout << line << endl;
	}

	return 0;

} catch (std::exception &e){
	cerr << e.what() << endl;
	return 1;

} catch (int v){
	return v;
}


