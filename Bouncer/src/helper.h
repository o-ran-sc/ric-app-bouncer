#include"header.h"
struct NodebListHandler : public BaseReaderHandler<UTF8<>, NodebListHandler> {
  vector<std::string> nodeb_list;
  string curr_key = "";

  bool Key(const Ch* str, SizeType length, bool copy) {
    curr_key = str;
    return true;
  }

  bool String(const Ch* str, SizeType length, bool copy) {
    if( curr_key.compare( "inventoryName" ) == 0 ) {
      std::cout<<str<<" "<<std::endl;
      nodeb_list.push_back( str );
    }
    return true;
  }
};
