<?php
// Based on https://refactoring.guru/design-patterns/singleton/php/example#example-1
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

require_once('singleton.inc');
require_once('logger.inc');
require_once('config.inc');

class Client extends Singleton
{
  protected function __construct()
  {
  }

  protected function _callNamed(string $name, string $command, string $params){
    Logger::debugf('%s: %s %s', $name, $command, $params);
    $cmd=Config::get('cmd', null, $name);
    if(empty($cmd)){
      Logger::errorf('client command not set');
      return null;
    }
    exec($cmd.' '.escapeshellarg($command).' '.$params.' 2>&1',$output,$status);
    $output=implode(' ',$output);
    if($status!=0){
      Logger::errorf('%s',$output);
      return null;
    }
    $result=json_decode($output,true);
    if(is_array($result)){
      return $result;
    }
    return $output;    
  }

  public static function callNamed(string $name, string $command, string $params=''){
    $client = static::getInstance();
    return $client->_callNamed($name, $command, $params);
  }

  public static function call(string $command, string $params=''){
    $client = static::getInstance();
    return $client->_callNamed('client', $command, $params);
  }
}

//old implementation DEPRECATED
function client($config,$command,$params=null){
  static $first_run=true;
  if($first_run){
    Logger::warningf('Using DEPRECATED Client implementation');
    $first_run=false;
  }
  if(is_null($params)){
    $params='';
  }
  Config::forceSet('cmd', $config['client'], $config['client']);
  return Client::callNamed($config['client'], $command, $params);
}
