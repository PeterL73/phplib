<?php
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

require_once('../inc/logger.inc');


// test nieuw
//Logger::setLevel('NOTICE');

Logger::errorf('Logger::error');
Logger::warningf('Logger::warning');
Logger::noticef('Logger::notice');
Logger::infof('Logger::info');
Logger::debugf('Logger::debug');
Logger::writef('ERROR', 'Error regel 1');
Logger::writef2('ERROR', 'Error regel 2');
Logger::writef('WARNING', 'Warning');
Logger::writef('NOTICE', 'object','ERROR');
Logger::writef('LEVEL', 'invalida logger level');
Logger::setLevel('INVALID');

// test oud
logger('ERROR', 'oud Error regel %d', 1);
logger2('ERROR', 'oud Error regel %d', 2);
logger('WARNING', 'oud Waring regel 1');

