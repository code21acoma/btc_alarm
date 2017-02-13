<?php
$json = file_get_contents('https://www.bitstamp.net/api/v2/ticker/btcusd/');
$jsonParse = json_decode($json);
$btcAlarm = file_get_contents('btcAlarm.txt');
echo $jsonParse->last."#".$btcAlarm;
?>
