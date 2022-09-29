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
echo Config::get('x:s1','###').PHP_EOL;
echo Config::get('ini',null).PHP_EOL;

echo Config::get('S1','###').PHP_EOL;
echo Config::get('x:S1','###').PHP_EOL;
echo Config::get('ini',null).PHP_EOL;

echo 'getParams'.PHP_EOL;
print_r(Config::getParams());
echo 'param[0]: "'.Config::param(0).'"'.PHP_EOL;
echo 'param[1]: "'.Config::param(1).'"'.PHP_EOL;
echo 'param[2]: "'.Config::param(2).'"'.PHP_EOL;
