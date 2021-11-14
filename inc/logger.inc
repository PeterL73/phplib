<?php
// Based on https://refactoring.guru/design-patterns/singleton/php/example#example-1
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

require_once('singleton.inc');

class LoggerLevel 
{
  const ERROR='ERROR';
  const WARNING='WARNING';
  const NOTICE='NOTICE';
  const INFO='INFO';
  const DEBUG='DEBUG';

  private function __construct()
  {
  }
}

/**
 * The logging class is the most known and praised use of the Singleton pattern.
 * In most cases, you need a single logging object that writes to a single log
 * file (control over shared resource). You also need a convenient way to access
 * that instance from any context of your app (global access point).
 */
class Logger extends Singleton
{
  private const COLOR = [
    'DEFAULT' => "\x1B[0m",
    'RED' => "\x1B[31m",
    'GREEN' => "\x1B[32m",
    'YELLOW' => "\x1B[33m",
    'BLUE' => "\x1B[34m",
    'MAGENTA' => "\x1B[35m",
    'CYAN' => "\x1B[36m",
    'GREY' => "\x1B[90m"   /* dark gray selectable in putty */
  ];

  private const LEVEL = [
    LoggerLevel::ERROR => ['prio' => 1, 'label' => 'ERROR',   'color' => self::COLOR['RED']],
    LoggerLevel::WARNING => ['prio' => 2, 'label' => 'WARNING', 'color' => self::COLOR['YELLOW']],
    LoggerLevel::NOTICE => ['prio' => 3, 'label' => 'NOTICE',  'color' => self::COLOR['CYAN']],
    LoggerLevel::INFO => ['prio' => 4, 'label' => 'INFO',    'color' => self::COLOR['DEFAULT']],
    LoggerLevel::DEBUG => ['prio' => 5, 'label' => 'DEBUG',   'color' => self::COLOR['GREY']]
  ];

  private $level = LoggerLevel::DEBUG;
  private $label = true;

    /**
     * A file pointer resource of the log file.
     */
    private $fileHandle;

    /**
     * Since the Singleton's constructor is called only once, just a single file
     * resource is opened at all times.
     *
     * Note, for the sake of simplicity, we open the console stream instead of
     * the actual file here.
     */
    protected function __construct()
    { 
        $this->fileHandle = fopen('php://stdout', 'w');
    }

    /**
     * Write a log entry to the opened file resource.
     */
    public function _writeLog(string $level, string $format, $replacements, $prefix=true): void
    {
      if(self::LEVEL[$level]['prio'] > self::LEVEL[$this->level]['prio']) {
        return;
      }
      if($prefix){
        fwrite($this->fileHandle, gmdate("[Y-m-d H:i:s] "));
      }
      else {
        printf(str_pad('',22));
      }  
      fwrite($this->fileHandle, self::LEVEL[$level]['color']);
      if($prefix && $this->label){
        fwrite($this->fileHandle, self::LEVEL[$level]['label']. ': ' );
      }

      if(isset($replacements[0]) && (is_array($replacements[0]) || is_object($replacements[0]))){  //TODO parse als var_export wanneer %v
        fwrite($this->fileHandle, $format.": ".PHP_EOL);
        fwrite($this->fileHandle, var_export($replacements[0], true));
      }
      else {
        fwrite($this->fileHandle, vsprintf($format, $replacements));
      }

      fwrite($this->fileHandle, self::COLOR['DEFAULT']);

      fwrite($this->fileHandle, PHP_EOL );
    }

    public static function setLevel(string $value){
      $logger = static::getInstance();
      $logger->level = $value;
    }
    public static function setLabel(bool $value){
      $logger = static::getInstance();
      $logger->label = $value;
    }

    /**
     * Just a handy shortcut to reduce the amount of code needed to log messages
     * from the client code.
     * parameters: level, format, replacements
     */
    private static function staticWriteLog(string $level, string $format, array $replacements, bool $prefix=true): void
    {
        $logger = static::getInstance();
        $logger->_writeLog($level, $format, $replacements, $prefix);
    }
    public static function errorf(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::ERROR, $format, $replacements, true);
    }
    public static function errorf2(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::ERROR, $format, $replacements, false);
    }
    public static function warningf(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::WARNING, $format, $replacements, true);
    }
    public static function warningf2(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::WARNING, $format, $replacements, false);
    }
    public static function noticef(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::NOTICE, $format, $replacements, true);
    }
    public static function noticef2(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::NOTICE, $format, $replacements, false);
    }
    public static function infof(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::INFO, $format, $replacements, true);
    }
    public static function infof2(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::INFO, $format, $replacements, false);
    }
    public static function debugf(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::DEBUG, $format, $replacements, true);
    }
    public static function debugf2(string $format, ...$replacements): void
    {
      static::staticWriteLog(LoggerLevel::DEBUG, $format, $replacements, false);
    }
    public static function writef(string $level, string $format, ...$replacements): void
    {
      static::staticWriteLog($level, $format, $replacements, true);
    }
    public static function writef2(string $level, string $format, ...$replacements): void
    {
      static::staticWriteLog($level, $format, $replacements, false);
    }

}

//old implementation DEPRECATED
define('LOGGER_ERROR',   'ERROR');
define('LOGGER_WARNING', 'WARNING');
define('LOGGER_NOTICE',  'NOTICE');
define('LOGGER_INFO',    'INFO');
define('LOGGER_DEBUG',   'DEBUG');
$logger_level=0;
$logger_label=true;

function logger(){
  logger_set();
  call_user_func_array("Logger::writef", func_get_args());
}
function logger2(){
  logger_set();
  call_user_func_array("Logger::writef2", func_get_args());
}
function logger_set(){
  static $first_run=true;
  global $logger_level;
  global $logger_label;

  if($logger_level && !is_numeric($logger_level)){
    Logger::setLevel($logger_level);
  }
  Logger::setLabel($logger_label);
  if($first_run){
    Logger::warningf('Using DEPRECATED Logger implementation');
    $first_run=false;
  }
}
