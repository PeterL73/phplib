<?php
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

$GLOBALS['DEBUG']=false;
function debug(){
  $argv = func_get_args();
  $level = array_shift( $argv );
  $format = array_shift( $argv );
  if(function_exists('_logger')){
    switch ($level) {
      case 'ERROR':
        $level=LOGGER_ERROR;
        break;
      case 'WARNING':
        $level=LOGGER_WARNING;
        break;
      case 'NOTICE':
        $level=LOGGER_NOTICE;
        break;
      case 'INFO':
        $level=LOGGER_INFO;
        break;
      case 'DEBUG':
        $level=LOGGER_DEBUG;
        break;
      default:
        $format=$level.':'.$format;
        $level=LOGGER_INFO;
        break;
    }
    _logger($level, $format, $argv, true);
    return;
  }
  if(!$GLOBALS['DEBUG'] && $level=='DEBUG'){
    return;
  }
  printf( gmdate("Y-m-d\TH:i:s\Z ").$level. ': ' );
  if(isset($argv[0]) && (is_array($argv[0]) || is_object($argv[0]))){
    print($format.": ");
    print_r($argv[0]);
  }
  else {
    vprintf( $format, $argv );
    printf( PHP_EOL );
  }
}
