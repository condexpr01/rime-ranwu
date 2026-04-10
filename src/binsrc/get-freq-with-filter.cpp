#include "../cppcore/core.impl.cpp"

using namespace table;

int main(int argc ,char *argv[]) try {
	if (argc != 3){
		cerr << format("Too many or too few argument.") << endl;
		cerr << format("Usage: {} 频表.utf8txt 待获频表.utf8txt",argv[0]) << endl;
		throw std::runtime_error("Error arguments.");
	}

	error_type error = nullptr;
	bool is_ok = false;

	table::table_t from(argv[1],table_category::key_word);
	if (!from.is_ok){throw std::runtime_error(from.error);}

	
	ifstream ifs{argv[2]};
	if (!ifs.is_open()){
		throw std::runtime_error{"Failed to open the file."};
	}

	vector<pair<string,vector<string>>> to;
	epcall(make_vector_table(ifs,to,table_category::key_word),error,is_ok);
	if(!is_ok){throw std::runtime_error{error};}

	epcall(get_freq(from,to),error,is_ok);
	if(!is_ok)cerr << error << endl;

	for (auto &&v : to){

		if (v.second[2].empty())continue;

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



