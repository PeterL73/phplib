<?php
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

require_once('../inc/logger.inc');


// test nieuw
//Logger::setLevel(LoggerLevel::NOTICE);

Logger::errorf('Logger::error');
Logger::warningf('Logger::warning');
Logger::noticef('Logger::notice');
Logger::infof('Logger::info');
Logger::debugf('Logger::debug');
Logger::writef(LoggerLevel::ERROR, 'Error regel 1');
Logger::writef2(LoggerLevel::ERROR, 'Error regel 2');
Logger::writef(LoggerLevel::WARNING, 'Warning');
Logger::writef(LoggerLevel::NOTICE, 'object',LoggerLevel::ERROR);

// test oud
logger('ERROR', 'oud Error regel %d', 1);
logger2('ERROR', 'oud Error regel %d', 2);
logger('WARNING', 'oud Waring regel 1');

