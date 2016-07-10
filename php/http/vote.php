<?php

require_once "Http.php";

$postData = array(
    'appid' => 'runtimeappid',
    'service' => 'generalApi',
    'method' => 'accumulate',
    'data' => '{"instanceId":"576121f13a7f7ec258bd96aa","type":"GLOBAL","counterName":"每人每天投票次数上限","option":"media_57613e346762d67f94ed4130","number":1,"openId":"o44n5s_Q8axsU1xNegYl4g813iB8","where":1}',
);
var_dump($postData);
$http = new Http();
$url = "http://api.mtq.tvm.cn/templateapp";
$res = $http->postRequest($url, $postData);
echo $res;
