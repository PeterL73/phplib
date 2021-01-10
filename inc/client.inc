<?php
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

require_once('debug.inc');
function client($config,$command,$params=null){
#  $input=null;
#  foreach($params as $value){
#    $input.=' '.$value;
#  }
#  if(isset($params)){
#    $params=escapeshellarg($params);
#  }


#  echo PHP_EOL;
#  echo $config['client'].' '.escapeshellarg($command).' '.$params.PHP_EOL;
  debug('DEBUG','Client: %s %s',$command, $params);
  exec($config['client'].' '.escapeshellarg($command).' '.$params.' 2>&1',$output,$status);
//  print_r($output);
//  print_r($status);
  $output=implode(' ',$output);
  if($status!=0){
    debug('ERROR','Client: %s',$output);
    return null;
  }
  $result=json_decode($output,true);
  if(is_array($result)){
    return $result;
  }
  return $output;
}

