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
Logger::infof('set level VERBOSE#40');
Logger::setLevel('VERBOSE#40');
Logger::verbosef('1', 'verbose level 1');
Logger::verbosef('2', 'verbose level 2');
Logger::verbosef('3', 'verbose level 3');
Logger::infof('set level VERBOSE#2');
Logger::setLevel('VERBOSE#2');
Logger::verbosef('1', 'verbose level 1');
Logger::verbosef('2', 'verbose level 2');
Logger::verbosef('3', 'verbose level 3');
Logger::infof('level 3 skipped');
// test oud
logger('ERROR', 'oud Error regel %d', 1);
logger2('ERROR', 'oud Error regel %d', 2);
logger('WARNING', 'oud Waring regel 1');

