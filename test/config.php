<?php
// Copyright (c) 2021 PeterL73
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://choosealicense.com/licenses/mit/

require_once('../inc/config.inc');

Config::forceSet('f1','force');
Config::forceSet('f2','force');
Config::softSet('s1','soft');
Config::softSet('s2','soft');

Config::forceSet('s1','force');
Config::softSet('f1','soft');

Config::forceSet('X1', 'upper x1');
Config::forceSet('s1','upper Force');
Config::softSet('F1','upper Soft');
Config::dump();

echo Config::get('s1','###').PHP_EOL;
echo Config::get('s1','###', 'x').PHP_EOL;
echo Config::get('ini',null).PHP_EOL;

echo Config::get('S1','###').PHP_EOL;
echo Config::get('S1','###', 'x').PHP_EOL;
echo Config::get('ini',null).PHP_EOL;


