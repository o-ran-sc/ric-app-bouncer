#include"bouncer.h"
//using namespace xapp;

Bouncer::Bouncer(){
        set_gnb();
}

void Bouncer::set_gnb(){
        cpprestclient obj("http://service-ricplt-e2mgr-http.ricplt:3800");
        response_t res;
        res=obj.do_get("/v1/nodeb/states");
        NodebListHandler handler;

        if( res.status_code == 200 )
        {
                //Reader reader;
                //StringStream ss( res.body.dump().c_str() );
                //reader.Parse( ss, handler );
                //std::cout << "[INFO] nodeb list is " << res.body.dump().c_str() << std::endl;
                std::cout << "[INFO] nodeb list is " << res.body<< std::endl;
                std::vector<std::string> inventoryNames;
                std::cout <<"pol"<<std::endl;
                for (const auto& item : res.body)
                {

                        std::string inventoryName = item["inventoryName"];
                        this->gnb_list.push_back(inventoryName);
                }

        }

        else
        {
                if( res.body.empty() )
                {
                        std::cout << "[ERROR] Unexpected HTTP code " << res.status_code <<  std::endl;
                }

                else
                {
                        std::cout << "[ERROR] Unexpected HTTP code " << res.status_code <<" HTTP payload is " << res.body.dump().c_str() << std::endl;
                }
        }

        //this->gnb_list= handler.nodeb_list;

}

