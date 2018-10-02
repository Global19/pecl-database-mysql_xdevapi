--TEST--
mysqlx client queue timeout while getting session from client
--SKIPIF--
--INI--
error_reporting=E_ALL
default_socket_timeout=4
--FILE--
<?php
require_once(__DIR__."/../connect.inc");
require_once(__DIR__."/client_utils.inc");

$pooling_options = '{
	"enabled": true,
  	"maxSize": 3,
  	"queueTimeOut": 200
}';

$client = mysql_xdevapi\getClient($connection_uri, $pooling_options);
$session0 = $client->getSession();
create_test_db($session0);
assert_session_valid($session0);

$session1 = $client->getSession();
assert_session_valid($session1);

$session2 = $client->getSession();
assert_session_valid($session2);

try {
	$session3 = $client->getSession();
	test_step_failed("connection pool should be full");
} catch (Exception $e) {
	echo $e->getMessage(), PHP_EOL;
	test_step_ok();
}

verify_expectations();
print "done!\n";
?>
--CLEAN--
<?php
	require_once(__DIR__."/../connect.inc");
	clean_test_db();
?>
--EXPECTF--
[10055][HY000] couldn't get connection from pool - queue timeout elapsed%s
done!%A
