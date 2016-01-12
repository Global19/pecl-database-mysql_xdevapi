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

#ifndef XMYSQLND_NODE_STMT_RESULT_FWD_H
#define XMYSQLND_NODE_STMT_RESULT_FWD_H

#include "xmysqlnd_driver.h"

struct st_xmysqlnd_node_stmt;
struct st_xmysqlnd_node_stmt_result_meta;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_xmysqlnd_node_stmt_result_fwd	XMYSQLND_NODE_STMT_RESULT_FWD;


typedef enum_func_status	(*func_xmysqlnd_node_stmt_result_fwd__init)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_object_factory) *factory, const size_t prefetch_rows, struct st_xmysqlnd_node_stmt * const stmt, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef enum_func_status	(*func_xmysqlnd_node_stmt_result_fwd__next)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef enum_func_status	(*func_xmysqlnd_node_stmt_result_fwd__fetch_current)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, zval * row, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef enum_func_status	(*func_xmysqlnd_node_stmt_result_fwd__fetch_one)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, const size_t row_cursor, zval * row, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef enum_func_status	(*func_xmysqlnd_node_stmt_result_fwd__fetch_all)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, zval * set, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef enum_func_status	(*func_xmysqlnd_node_stmt_result_fwd__rewind)(XMYSQLND_NODE_STMT_RESULT_FWD * const result);
typedef zend_bool			(*func_xmysqlnd_node_stmt_result_fwd__eof)(const XMYSQLND_NODE_STMT_RESULT_FWD * const result);

typedef zval *				(*func_xmysqlnd_node_stmt_result_fwd__create_row)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, const struct st_xmysqlnd_node_stmt_result_meta * const meta, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef void				(*func_xmysqlnd_node_stmt_result_fwd__destroy_row)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, zval * row, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef enum_func_status	(*func_xmysqlnd_node_stmt_result_fwd__add_row)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, zval * row, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef size_t				(*func_xmysqlnd_node_stmt_result_fwd__get_row_count)(const XMYSQLND_NODE_STMT_RESULT_FWD * const result);
typedef void				(*func_xmysqlnd_node_stmt_result_fwd__free_rows_contents)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef void				(*func_xmysqlnd_node_stmt_result_fwd__free_rows)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);

typedef enum_func_status	(*func_xmysqlnd_node_stmt_result_fwd__attach_meta)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, struct st_xmysqlnd_node_stmt_result_meta * const meta, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);

typedef void				(*func_xmysqlnd_node_stmt_result_fwd__free_contents)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info);
typedef void				(*func_xmysqlnd_node_stmt_result_fwd__dtor)(XMYSQLND_NODE_STMT_RESULT_FWD * const result, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info);

MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_node_stmt_result_fwd)
{
	func_xmysqlnd_node_stmt_result_fwd__init init;

	func_xmysqlnd_node_stmt_result_fwd__next next;
	func_xmysqlnd_node_stmt_result_fwd__fetch_current fetch_current;
	func_xmysqlnd_node_stmt_result_fwd__fetch_one fetch_one;
	func_xmysqlnd_node_stmt_result_fwd__fetch_all fetch_all;
	func_xmysqlnd_node_stmt_result_fwd__rewind rewind;
	func_xmysqlnd_node_stmt_result_fwd__eof eof;

	func_xmysqlnd_node_stmt_result_fwd__create_row create_row;
	func_xmysqlnd_node_stmt_result_fwd__destroy_row destroy_row;
	func_xmysqlnd_node_stmt_result_fwd__add_row add_row;

	func_xmysqlnd_node_stmt_result_fwd__get_row_count get_row_count;

	func_xmysqlnd_node_stmt_result_fwd__free_rows_contents free_rows_contents;
	func_xmysqlnd_node_stmt_result_fwd__free_rows free_rows;

	func_xmysqlnd_node_stmt_result_fwd__attach_meta attach_meta;

	func_xmysqlnd_node_stmt_result_fwd__free_contents free_contents;
	func_xmysqlnd_node_stmt_result_fwd__dtor dtor;
};


struct st_xmysqlnd_node_stmt_result_fwd
{
	struct st_xmysqlnd_node_stmt * stmt;
	struct st_xmysqlnd_node_stmt_result_meta * meta;

	zval ** rows; /* every row is a memory segment of field_count * sizeof(zval) */
	size_t row_count;
	size_t rows_allocated;
	size_t row_cursor;

	size_t total_row_count;
	size_t total_fetched;

	size_t prefetch_rows;
	MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_node_stmt_result_fwd) m;
	zend_bool		persistent;
};

PHPAPI extern MYSQLND_CLASS_METHOD_TABLE_NAME_FORWARD(xmysqlnd_node_stmt_result_fwd);

PHPAPI XMYSQLND_NODE_STMT_RESULT_FWD * xmysqlnd_node_stmt_result_fwd_init(const size_t prefetch_rows, struct st_xmysqlnd_node_stmt * stmt, const zend_bool persistent, MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_object_factory) *object_factory,  MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info);
PHPAPI void xmysqlnd_node_stmt_result_fwd_free(XMYSQLND_NODE_STMT_RESULT_FWD * const result, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XMYSQLND_NODE_STMT_RESULT_FWD_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
