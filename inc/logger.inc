<?php
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

define('LOGGER_CL_N', "\x1B[0m");
define('LOGGER_CL_RED', "\x1B[31m");
define('LOGGER_CL_GRN', "\x1B[32m");
define('LOGGER_CL_YLW', "\x1B[33m");
define('LOGGER_CL_BLU', "\x1B[34m");
define('LOGGER_CL_MAG', "\x1B[35m");
define('LOGGER_CL_CYN', "\x1B[36m");

define('LOGGER_CL_GRY', "\x1B[90m");    /* dark gray selectable in putty */

define('LOGGER_ERROR',   ['prio' => 1, 'label' => 'ERROR',   'color' => LOGGER_CL_RED]);
define('LOGGER_WARNING', ['prio' => 2, 'label' => 'WARNING', 'color' => LOGGER_CL_YLW]);
define('LOGGER_NOTICE',  ['prio' => 3, 'label' => 'NOTICE',  'color' => LOGGER_CL_CYN]);
define('LOGGER_INFO',    ['prio' => 4, 'label' => 'INFO',    'color' => ""]);
define('LOGGER_DEBUG',   ['prio' => 5, 'label' => 'DEBUG',   'color' => LOGGER_CL_GRY]);

$logger_level=0;
$logger_label=true;

//TODO standaard PHP meldingen afvangen (of niet)
//error_reporting(0);
//set_error_handler(_logger_error_handler);

//_logger_test();

function logger(){
  $argv = func_get_args();
  $level = array_shift( $argv );
  $format = array_shift( $argv );
  _logger($level, $format, $argv, true);
}
function logger2(){
  $argv = func_get_args();
  $level = array_shift( $argv );
  $format = array_shift( $argv );
  if(strlen($format)==0) {
    printf( PHP_EOL );
  }
  else {
    _logger($level, $format, $argv, false);
  }
}
function _logger($level, $format, $argv, $prefix){
  global $logger_level;
  global $logger_label;

  $test_level=$logger_level;
  if(isset($logger_level['prio'])){
    $test_level=$logger_level['prio'];
  }
  if($test_level && isset($level['prio']) && $level['prio'] > $test_level){
    return;
  }
  if($prefix){
    printf( gmdate("[Y-m-d H:i:s] "));
  }
  else {
    printf(str_pad('',22));
  }
  if(isset($level['color'])){
    printf($level['color']);
  }
  if($prefix && $logger_label){
    if(isset($level['label'])){
      printf($level['label']. ': ' );
    }
  }
  _logger_line($format, $argv);
  if(isset($level['color'])){
    printf(LOGGER_CL_N);
  }
  printf( PHP_EOL );
}
function _logger_line($format, $argv){
  if(isset($argv[0]) && (is_array($argv[0]) || is_object($argv[0]))){  //TODO parse als var_export wanneer %v
    print($format.": ");
    printf( PHP_EOL );
    #print_r($argv[0]);
    var_export($argv[0]);
  }
  else {
    vprintf( $format, $argv );
  }
}
function _logger_error_handler($errno, $errstr, $errfile, $errline){
  $errortype = array (
    E_WARNING            => LOGGER_WARNING,
    E_NOTICE             => LOGGER_NOTICE,
    E_USER_ERROR         => LOGGER_ERROR,
    E_USER_WARNING       => LOGGER_WARNING,
    E_USER_NOTICE        => LOGGER_WARNING
  );

  logger($errortype[$errno], '(PHP) '.$errstr.' in '.$errfile.' on line '.$errline);
}

function _logger_test(){
  global $logger_level;
  $test_object=new mysqli;
  logger(LOGGER_ERROR,'Dit is een error');
  logger(LOGGER_ERROR,'Dit is een error met variabele logger_level: %d', $logger_level);
  logger(LOGGER_ERROR,'Dit is een error met array', LOGGER_INFO);
  logger(LOGGER_ERROR,'Dit is een error met object', $test_object);

  logger(LOGGER_WARNING,'Dit is een warning');
  logger(LOGGER_NOTICE,'Dit is een notice');
  logger2(LOGGER_NOTICE,'Dit is een notice');
  logger(LOGGER_INFO,'Dit is een info');
  logger(LOGGER_DEBUG,'Dit is een debug %r',$test_object);
}
