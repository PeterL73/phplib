<?php
// Based on https://refactoring.guru/design-patterns/singleton/php/example#example-1
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

require_once('singleton.inc');
require_once('config.inc');

/* config options
 * [logger]
 * level     = DEBUG
 * label     = false
 * toscreen  = true (false when not set and tofile set)
 * tofile    = false
 * filename  = <scriptname>.log
 * markfirst = true
 * utc       = true
 */
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
    'ERROR' => ['prio' => 1, 'label' => 'ERROR',   'color' => self::COLOR['RED']],
    'WARNING' => ['prio' => 2, 'label' => 'WARNING', 'color' => self::COLOR['YELLOW']],
    'NOTICE' => ['prio' => 3, 'label' => 'NOTICE',  'color' => self::COLOR['CYAN']],
    'INFO' => ['prio' => 4, 'label' => 'INFO',    'color' => self::COLOR['DEFAULT']],
    'DEBUG' => ['prio' => 5, 'label' => 'DEBUG',   'color' => self::COLOR['GREY']]
  ];

  private $level='DEBUG';
  private $label;
  private $toscreen;
  private $tofile;
  private $filename;

    /**
     * A file pointer resource of the log file.
     */
     private $fileHandle = null;
     private $screenHandle = null;


    /**
     * Since the Singleton's constructor is called only once, just a single file
     * resource is opened at all times.
     *
     * Note, for the sake of simplicity, we open the console stream instead of
     * the actual file here.
     */
    protected function __construct()
    { 
      $this->label = Config::get('logger:label', false);
      $this->tofile = Config::get('logger:tofile', false);
      if($this->tofile){
        Config::softSet('logger:toscreen', false);
        $script=pathinfo(get_included_files()[0]);
        $filename=$script['filename'].'.log';
        Config::softSet('logger:filename', $script['dirname'].'/'.$filename);
      }
      $this->toscreen = Config::get('logger:toscreen', true);
      $this->filename = Config::get('logger:filename', null);
      if($this->tofile){
        $this->fileHandle = fopen($this->filename, 'a');
      }
      if($this->toscreen){
        $this->screenHandle = fopen('php://stdout', 'a');
      }
      $this->_setLevel(Config::get('logger:level', 'DEBUG'));
    }

    public function write($string){
      if($this->screenHandle){
        fwrite($this->screenHandle, $string);
      }
      if($this->fileHandle){
        fwrite($this->fileHandle, $string);
      }
    }
    /**
     * Write a log entry to the opened file resource.
     */
    public function _writeLog(string $level, string $format, $replacements, $prefix=true): void
    {
      static $first=null;
      if(is_null($first)){
        $first=Config::get('logger:markfirst',true);
      }

      if(!isset(self::LEVEL[$level]['prio'])){
        $this->_writeLog('ERROR', 'Invalid level [%s]', $level);
        return;
      }
      if(self::LEVEL[$level]['prio'] > self::LEVEL[$this->level]['prio']) {
        return;
      }
      if($first){
        $this->write("\x1B[53m");
      }
      if($prefix){
        if(Config::get('logger:utc', true)){
          $this->write(gmdate("[Y-m-d H:i:s]"));
        }
        else {
          $this->write(date("[Y-m-d H:i:s]"));
        }
      }
      else {
        $this->write(str_pad('',21));
      }  
      if($first){
        $this->write("\x1B[0m");
      }
      $this->write(' ');
      $first=false;
      $this->write(self::LEVEL[$level]['color']);
      if($prefix && $this->label){
        $this->write(self::LEVEL[$level]['label']. ': ' );
      }

      if(isset($replacements[0]) && (is_array($replacements[0]) || is_object($replacements[0]))){  //TODO parse als var_export wanneer %v
        $this->write($format.": ".PHP_EOL);
        $this->write(var_export($replacements[0], true));
      }
      else {
        $this->write(vsprintf($format, $replacements));
      }

      $this->write(self::COLOR['DEFAULT']);

      $this->write( PHP_EOL );
    }

    public function _setLevel(string $value)
    {
      $value=strtoupper($value);
      if(!isset($this::LEVEL[$value]['prio'])){
        $this->_writeLog('ERROR', 'Invalid level [%s] set', $value);
        return;
      }
      $this->level = $value;
    }

    public static function setLevel(string $value){
      $logger = static::getInstance();
      $logger->_setLevel($value);
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
      static::staticWriteLog('ERROR', $format, $replacements, true);
    }
    public static function errorf2(string $format, ...$replacements): void
    {
      static::staticWriteLog('ERROR', $format, $replacements, false);
    }
    public static function warningf(string $format, ...$replacements): void
    {
      static::staticWriteLog('WARNING', $format, $replacements, true);
    }
    public static function warningf2(string $format, ...$replacements): void
    {
      static::staticWriteLog('WARNING', $format, $replacements, false);
    }
    public static function noticef(string $format, ...$replacements): void
    {
      static::staticWriteLog('NOTICE', $format, $replacements, true);
    }
    public static function noticef2(string $format, ...$replacements): void
    {
      static::staticWriteLog('NOTICE', $format, $replacements, false);
    }
    public static function infof(string $format, ...$replacements): void
    {
      static::staticWriteLog('INFO', $format, $replacements, true);
    }
    public static function infof2(string $format, ...$replacements): void
    {
      static::staticWriteLog('INFO', $format, $replacements, false);
    }
    public static function debugf(string $format, ...$replacements): void
    {
      static::staticWriteLog('DEBUG', $format, $replacements, true);
    }
    public static function debugf2(string $format, ...$replacements): void
    {
      static::staticWriteLog('DEBUG', $format, $replacements, false);
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
