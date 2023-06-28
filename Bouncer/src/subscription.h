#include"header.h"
struct subscriptionJson{
                 nlohmann::json jsonObject =
    {



        {"SubscriptionId",""},
        {"ClientEndpoint",{{"Host","service-ricxapp-bouncer-xapp-http.ricxapp"},{"HTTPPort",8080},{"RMRPort",4560}}},
        {"Meid","meid"},
        {"RANFunctionID",1},
        {"SubscriptionDetails",
                {
                        {
                            {"XappEventInstanceId",12345},{"EventTriggers",{8,39,15}},
                            {"ActionToBeSetupList",
                                    {
                                        {
                                                {"ActionID",1},{"ActionType","report"},{"ActionDefinition",{0}},{"SubsequentAction",{{"SubsequentActionType","continue"},{"TimeToWait","zero"}}}
                                        }
                                    }
                            }
                        }
                }
        }

    };

};
