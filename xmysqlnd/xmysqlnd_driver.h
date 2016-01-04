/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2006-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Andrey Hristov <andrey@mysql.com>                           |
  +----------------------------------------------------------------------+
*/

#ifndef XMYSQLND_DRIVER_H
#define XMYSQLND_DRIVER_H

#include "xmysqlnd_enum_n_def.h"

struct st_xmysqlnd_node_session;
struct st_xmysqlnd_node_session_data;
struct st_xmysqlnd_node_stmt;
struct st_xmysqlnd_result_field_meta;
struct st_xmysqlnd_protocol_frame_codec;

MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_object_factory);

typedef struct st_xmysqlnd_node_session *			(*func_xmysqlnd_object_factory__get_node_session)(MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_object_factory) * factory, const zend_bool persistent);
typedef struct st_xmysqlnd_node_stmt *				(*func_xmysqlnd_object_factory__get_node_stmt)(struct st_xmysqlnd_node_session_data * session, const MYSQLND_CSTRING query, const zend_bool persistent, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info);
typedef struct st_xmysqlnd_node_query_result_meta *	(*func_xmysqlnd_object_factory__get_node_query_result_meta)(const zend_bool persistent, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info);
typedef struct st_xmysqlnd_result_field_meta *		(*func_xmysqlnd_object_factory__get_result_field_meta)(const zend_bool persistent, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info);
typedef struct st_xmysqlnd_protocol_frame_codec *	(*func_xmysqlnd_object_factory__get_pfc)(const zend_bool persistent, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info);

MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_object_factory)
{
	func_xmysqlnd_object_factory__get_node_session get_node_session;
	func_xmysqlnd_object_factory__get_node_stmt get_node_stmt;
	func_xmysqlnd_object_factory__get_node_query_result_meta get_node_query_result_meta;
	func_xmysqlnd_object_factory__get_result_field_meta get_result_field_meta;
	func_xmysqlnd_object_factory__get_pfc get_protocol_frame_codec;
};

PHPAPI extern MYSQLND_CLASS_METHOD_TABLE_NAME_FORWARD(xmysqlnd_object_factory);

#endif	/* XMYSQLND_DRIVER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */