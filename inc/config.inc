<?php
// Based on https://refactoring.guru/design-patterns/singleton/php/example#example-1
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

require_once('singleton.inc');
require_once('logger.inc');

/**
 * Applying the Singleton pattern to the configuration storage is also a common
 * practice. Often you need to access application configurations from a lot of
 * different places of the program. Singleton gives you that comfort.
 */
class Config extends Singleton
{
  private $map = [];
  private $params = [];
  private $empty_section_as_default=false;
  private $now = 0;

  protected function __construct() 
  {
    $now = time();
    $arg_map=[];
    $this->processArgv($arg_map);

    $ini=null;
    if(isset($arg_map['']['ini'])){
      $ini=realpath($arg_map['']['ini']);
    }
    else {
      $script=pathinfo(get_included_files()[0]);
      $ini_filename=$script['filename'].'.ini';
      $search=[];
      $search[]=getcwd();
      $search[]=$script['dirname'];
//      print_r($search);
      for($i=0; $i<count($search); $i++){
        $test=realpath($search[$i].'/'.$ini_filename);
        if(is_file($test)){
          $ini=$test;
          break;
        }
      }
    }

    $this->map = [];
    if(is_file($ini)){
      $ini_data=file_get_contents($ini);
      $ini_data='[]'.PHP_EOL.$ini_data;
      $this->map = @parse_ini_string($ini_data,true/*, INI_SCANNER_RAW*/);
      $this->map['']['ini']=$ini;
    }

    foreach($arg_map as $section_key => $section){
      foreach($section as $key => $value){
        $this->map[$section_key][$key]=$value;
      }
    }
  }

  public function processArgv(&$arg_map): void
  {
    // TODO: implement all ini capabilities
    global $argv;
    $arg_map=[];

    $key=null;
    $section=null;
    for($i=1;$i<count($argv);$i++){
      if(!empty($key)){
        $this->processValue($arg_map, $section, $key, $argv[$i]);
      } elseif(substr($argv[$i],0,1)=='-'){
        if(substr($argv[$i],-1,1)==':'){
          $section=strtolower(substr(substr($argv[$i],0,-1),1));
        }
        else {
          $key=strtolower(substr($argv[$i],1));
        }
      }
      else {
        $this->params[]=$argv[$i];
      }
    }
    if(!empty($key)){
     $this-> processValue($arg_map, $section, $key, 1);
    }
  }

  public function processValue(&$arg_map, $section, &$key, $value){
    if(strpos($key, ':')){
      list($section,$key) = explode(':', $key, 2);
    }
    if(substr($value,0,1)=='-'){
      $arg_map[$section][$key]=1;
      $key=strtolower(substr($value,1));
    } else {
      $arg_map[$section][$key]=$value;
      $key=null;
    }
  }

  public function _splitSectionKey(string &$section, string &$key){
    $section='';
    $key=strtolower($key);
    if(strpos($key, ':') !== false){
      list($section,$key) = explode(':', $key);
    }
  }

  public function _get(string $key, $default) 
  {
    $section='';
    $this->_splitSectionKey($section, $key);

    $value=$default;

    if(isset($this->map[$section][$key])){
      $value=$this->map[$section][$key];
    }
    elseif($this->empty_section_as_default && isset($this->map[''][$key])){
      $value=$this->map[''][$key];
    }
    return $value;
  }

  public function _set(string $key, $value, bool $force): void
  {
    $section='';
    $this->_splitSectionKey($section, $key);

    $key=strtolower($key);

    if($force || !isset($this->map[$section][$key])){
      $this->map[$section][$key]=$value;
    }
  }

  public function _isSet(string $key): bool
  {
    return (bool)$this->_get($key,0);
  }

  public function _getParams(): array
  {
    return $this->params;
  }

  public function _time(): int
  {
    return $this->now;
  }

  public function _dump(): void
  {
    print_r($this->map);
    print_r($this->params);
  }

  public static function testArgCount($allow, $num){
    if($num>$allow){
      echo 'DEPRECATED use section:key'.PHP_EOL;
      Logger::errorf('Using DEPRECATED Config implementation. Use "section:key" instead.');
      exit;
    }
  }
  public static function get(string $key, $default){
    static::testArgCount(2, func_num_args());
    $config = static::getInstance();
    return $config->_get($key, $default);
  }
  public static function softSet(string $key, $value){
    static::testArgCount(2, func_num_args());
    $config = static::getInstance();
    $config->_set($key, $value, false);
  }
  public static function forceSet(string $key, $value){
    static::testArgCount(2, func_num_args());
    $config = static::getInstance();
    $config->_set($key, $value, true);
  }
  public static function isSet(string $key){
    static::testArgCount(1, func_num_args());
    $config = static::getInstance();
    return $config->_isSet($key);
  }
  public static function param($index)
  {
    $params = static::getParams();
    if(isset($params[$index])){
      return $params[$index];
    }
    return null;
  }
  public static function getParams(): array
  {
    $config = static::getInstance();
    return $config->_getParams();
  }
  public static function time(){
    $config = static::getInstance();
    return $config->_time();
  }
  public static function dump(){
    $config = static::getInstance();
    $config->_dump();
  }
}
