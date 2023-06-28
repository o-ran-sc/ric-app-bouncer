#!/bin/sh
#docker run --rm -u 0 -it -d -p 8090:8080 -e DEBUG=1 -e STORAGE=local -e STORAGE_LOCAL_ROOTDIR=/charts -v $(pwd)/charts:/charts chartmuseum/chartmuseum:latest
export CHART_REPO_URL=http://0.0.0.0:8090
dms_cli onboard config-file.json schema.json
curl -X GET http://localhost:8090/api/charts | jq .
dms_cli install bouncer-xapp 2.0.0 ricxapp
curl -v -X POST "http://$(kubectl get service -n ricplt | grep appmgr-http| awk '{print $3}'):8080/ric/v1/register" -H "accept: application/json" -H "Content-Type: application/json" -d "@bouncer-register.json"
