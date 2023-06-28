#include <ricxfcpp/xapp.hpp>
#include <ricxfcpp/config.hpp>
#include<ricxfcpp/RestClient.h>
#include <rmr/RIC_message_types.h>
#include<vector>
#include<string>
#include<iostream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/schema.h>
#include <rapidjson/reader.h>
#include <rapidjson/prettywriter.h>
#include<sstream>
#include "../e2ap/headers/E2AP-PDU.h"
using namespace rapidjson;
using namespace std;
using namespace xapp;
