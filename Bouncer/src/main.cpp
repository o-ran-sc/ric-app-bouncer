#include "bouncer.h"
//using namespace xapp;
std::unique_ptr<Xapp> xfw;

void display_json(web::json::value const & jvalue){
	std::cout<<"\ndisplaying REST Notification\n";
	std::wcout << jvalue.serialize().c_str() << std::endl;
}


void indication_callback( Message& mbuf, int mtype, int subid, int len, Msg_component payload, void* data ) {

	std::cout << "[INFO] indication Callback got a message, type=" << mtype << ", length=" << len << std::endl;
	//std::string json ((char *)payload.get(), len); // RMR payload might not have a nil terminanted char
	
	E2AP_PDU_t* e2pdu = (E2AP_PDU_t*)calloc(1, sizeof(E2AP_PDU));
	asn_transfer_syntax syntax;
        syntax = ATS_ALIGNED_BASIC_PER;
	auto rval =  asn_decode(nullptr, syntax, &asn_DEF_E2AP_PDU, (void**)&e2pdu, (char *)payload.get(), len);
	if(rval.code == RC_OK)
	{
		std::cout << "[INFO] E2AP indication decode successfull rval.code = "<<rval.code<<std::endl;
		asn_fprint(stdout, &asn_DEF_E2AP_PDU, e2pdu);
	}
        else
	{
		 std::cout << "[INFO] E2AP indication decode failed rval.code = "<<rval.code<<std::endl;
        }

       	asn_fprint(stdout, &asn_DEF_E2AP_PDU, e2pdu);
	ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, e2pdu);
	//std::cout << "[INFO] Payload is " << json << std::endl;

}

void handle_request( web::http::http_request request)
{
auto answer = web::json::value::object();
std::cout<<"\nPrinting POST request content\n";
std::cout<<request.to_string()<<"\n";
   request
      
	   .extract_json()
      .then([&answer](pplx::task<web::json::value> task) {
         try
         {
            answer = task.get();
            display_json(answer);
            }
         catch (web::http::http_exception const & e)
         {
	 	std::wcout << e.what() << std::endl;
         }

      })
      .wait();

   request.reply(web::http::status_codes::OK, answer);
}

extern int main( int argc, char** argv ) {
  
	int nthreads = 1;
  	char*	port = (char *) "4560";
  	xfw = std::unique_ptr<Xapp>( new Xapp( port, true ) );
 	xfw->Add_msg_cb( RIC_INDICATION , indication_callback, NULL );
  
 	Bouncer B;
  
  	
	while(B.gnb_list.size() ==0){
		B.set_gnb();
  	}
  	std::cout << "[INFO] size of gnb list is "<<B.gnb_list.size()<<std::endl;
	cpprestclient obj("http://service-ricplt-submgr-http.ricplt.svc.cluster.local:8088/ric/v1/subscriptions",handle_request);
	
	for(int i=0;i<B.gnb_list.size();i++){

  	        subscriptionJson j;
		j.jsonObject["Meid"]=B.gnb_list[i];
		oresponse_t o;
		std::cout << "[INFO] sending subscription for "<<B.gnb_list[i]<<std::endl;
        	o=obj.post_subscriptions(j.jsonObject,"");
        	std::cout << "[INFO] status code is " << o.status_code<< std::endl;
        	std::cout << "[INFO] subscription id is " << o.SubscriptionId<< std::endl;

	}

	xfw->Run( nthreads );

}
