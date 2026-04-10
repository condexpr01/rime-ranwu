#include "../cppcore/core.impl.cpp"

using namespace table;

int main(int argc ,char *argv[]) try {
	if (argc != 3){
		cerr << format("Too many or too few argument.") << endl;
		cerr << format("Usage: {} 双拼码.utf8txt 辅助码.utf8txt",argv[0]) << endl;
		throw std::runtime_error("Error arguments.");
	}

	error_type error = nullptr;
	bool is_ok = false;

	table::table_t zrm(argv[1],table_category::key_word);
	if (!zrm.is_ok){throw std::runtime_error(zrm.error);}

	table::table_t aux(argv[2],table_category::key_word);
	if (!aux.is_ok){throw std::runtime_error(aux.error);}

	//0     1    first 2
	//codec,any=字词,any
	
	string ranwu{};
	for(auto &&i : zrm){
		auto find = aux.find(i.first);
		if (find == aux.end()){
			cerr << format("[aux table]missing {}",i.first) << endl;

		}else{
			i.second[0] = i.second[0] + (*find).second[0];
		}
	}


	//epcall(t.output_table(cout,table_t::encoder_filter),error,is_ok);
	epcall(zrm.output_table(cout),error,is_ok);
	if(is_ok)cerr << error << endl;

	return 0;

} catch (std::exception &e){
	cerr << e.what() << endl;
	return 1;

} catch (int v){
	return v;
}



