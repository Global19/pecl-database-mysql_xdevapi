/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2006-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Andrey Hristov <andrey@php.net>                             |
  +----------------------------------------------------------------------+
*/
#ifndef MYSQLX_EXCEPTION_H
#define MYSQLX_EXCEPTION_H

#ifdef  __cplusplus
extern "C" {
#endif

void mysqlx_new_exception(const unsigned int code, const MYSQLND_CSTRING sql_state, const MYSQLND_CSTRING msg);
void mysqlx_new_exception_ex(const unsigned int code, const MYSQLND_CSTRING sql_state, const char * const format, ...);

void mysqlx_register_exception_class(INIT_FUNC_ARGS, zend_object_handlers * mysqlx_std_object_handlers);
void mysqlx_unregister_exception_class(SHUTDOWN_FUNC_ARGS);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* MYSQLX_EXCEPTION_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
