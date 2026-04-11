
//确保只有一个音，并且这个音的频率最大
#include "../cppcore/core.impl.cpp"
using namespace table;

struct unique_voice_table{
	using table_type = unordered_multimap<string,vector<string>>;
	bool is_ok = false;

	table_type table;

	ep<void> make_table(ifstream& ifs){
		ep<void> e{};

		//cat为key_codec,期望键存在编码,对应位置:[编码],序号=字词,频数
		//cat为key_word ,期望键存在字词,对应位置:编码,序号=[字词],频数
		(*this).is_ok = false;
		
		string line;
		string a,b,c,d;

		const size_t startpos = 0;

		size_t a_endpos;
		size_t b_endpos;
		size_t c_endpos;
		[[maybe_unused]] size_t d_endpos;

		bool a_exist;
		bool b_exist;
		bool c_exist;
		[[maybe_unused]]bool d_exist;
		
		while(true){

			if(getline(ifs,line,'\n')){

				//降噪去0x0d
				if(line.empty()) continue;
				if(line.back() == '\r')line.pop_back();

				//降噪去空白
				line.erase(std::remove_if(line.begin(),line.end(),
						[](char c){return std::isspace(c);})
					,line.end());

				//opt pattern: "编码或字词,"
				a_endpos = line.find(',',startpos);
				a_exist = (a_endpos != string::npos);

				//opt pattern: "序号="
				b_endpos = line.find('=',a_endpos + a_exist);
				b_exist = (b_endpos != string::npos);

				//opt pattern: "字词或编码,"
				c_endpos = line.find(',',b_endpos + b_exist);
				c_exist = (c_endpos != string::npos);

				//opt pattern: "频数"
				//c_endpos到string::npos

				a = string_slice(line,startpos          ,a_endpos);
				b = string_slice(line,a_endpos + a_exist,b_endpos);
				c = string_slice(line,b_endpos + b_exist,c_endpos);
				d = string_slice(line,c_endpos + c_exist,string::npos);

			}else if(!ifs.eof()){
				e = unep{"Error: Didn't get to EOF."};
				break;

			}else{break;}

			//key_word 下c为key 具体位置: "编码,序号=[字词],频数"
			do{
				if(c.empty()){
					cerr << "line: " << string_visiable(line) 
						<< " [fmt need]: " << "编码,序号=[字词],频数" << endl;
					break;}
				
				auto find = (*this).table.find(c);
				if (find == (*this).table.end()){
					(*this).table.emplace(c,vector<string>{a,b,d});
				}else{
					if ((*find).second[2] < d){
						(*find).second[0] = a;
						(*find).second[1] = b;
						(*find).second[2] = d;
					}
				}

			}while(false);

		}
		
		(*this).is_ok = true;
		return e;
	}

};



int main(int argc ,char *argv[]) try {

	ifstream ifs{};

	if (argc == 2){
		ifs.open(argv[1],std::ios_base::in);

		if (!ifs.is_open()){
			throw std::runtime_error{"Failed to open the file."};
		}

	}else{
		throw std::runtime_error{"Too many or too few arguments."};
	}

	error_type error = nullptr;
	bool is_ok = false;

	unique_voice_table result{};

	epcall(result.make_table(ifs),error,is_ok);
	if (!is_ok)throw std::runtime_error{"[Fatal] make table"};

	for (auto &&v : result.table){

		cout << format("{},{}={},{}\n",
			 string_visiable(v.second[0]),
			 string_visiable(v.second[1]),
			 string_visiable(v.first),
			 string_visiable(v.second[2])
			 );

		cout.flush();
	}

	return 0;

} catch (std::exception &e){
	cerr << e.what() << endl;
	return 1;

} catch (int v){
	return v;
}


