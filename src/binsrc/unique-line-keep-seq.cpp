
//确保行内容唯一
//同时保证不改变第一次出现的顺序

#include <iostream>
#include <string>
#include <vector>
#include <fstream>


using std::cout,std::cerr,std::endl;

using std::string;
using std::vector;
using std::ifstream;

int main(int argc ,char *argv[]) try {
	std::vector<string> result{};

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

			//very slow
			//in order to keeping sequences
			auto is_find = std::find_if(result.begin(),result.end(),[&](auto &e){
				return e == line;
			});

			if(is_find == result.end()){
				result.emplace_back(line);
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


