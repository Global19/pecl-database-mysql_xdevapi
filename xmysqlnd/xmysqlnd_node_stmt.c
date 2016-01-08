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
#include "php.h"
#include "ext/mysqlnd/mysqlnd.h"
#include "ext/mysqlnd/mysqlnd_debug.h"
#include "xmysqlnd.h"
#include "xmysqlnd_driver.h"
#include "xmysqlnd_node_session.h"
#include "xmysqlnd_node_stmt.h"
#include "xmysqlnd_node_stmt_result.h"
#include "xmysqlnd_node_stmt_result_meta.h"

/* {{{ xmysqlnd_node_stmt::init */
static enum_func_status
XMYSQLND_METHOD(xmysqlnd_node_stmt, init)(XMYSQLND_NODE_STMT * const stmt,
										  XMYSQLND_NODE_SESSION_DATA * const session,
										  const MYSQLND_CSTRING query,
										  MYSQLND_STATS * const stats,
										  MYSQLND_ERROR_INFO * const error_info)
{
	DBG_ENTER("xmysqlnd_node_stmt::init");
	if (!(stmt->data->session = session->m->get_reference(session))) {
		return FAIL;
	}
	stmt->data->m.get_reference(stmt);
	stmt->data->query = mnd_dup_cstring(query, stmt->data->persistent);
	DBG_INF_FMT("query=[%d]%*s", stmt->data->query.l, stmt->data->query.l, stmt->data->query.s);
	DBG_RETURN(PASS);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::send_query */
static enum_func_status
XMYSQLND_METHOD(xmysqlnd_node_stmt, send_query)(XMYSQLND_NODE_STMT * const stmt, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info)
{
	const MYSQLND_CSTRING namespace_par = {"sql", sizeof("sql") - 1};
	MYSQLND_VIO * vio = stmt->data->session->io.vio;
	XMYSQLND_PFC * pfc = stmt->data->session->io.pfc;
	const XMYSQLND_L3_IO io = {vio, pfc};
	const struct st_xmysqlnd_message_factory msg_factory = xmysqlnd_get_message_factory(&io, stats, error_info);
	enum_func_status ret;
	DBG_ENTER("xmysqlnd_node_stmt::send_query");

	stmt->data->partial_read_started = FALSE;
	stmt->data->msg_stmt_exec = msg_factory.get__sql_stmt_execute(&msg_factory);
	ret = stmt->data->msg_stmt_exec.send_request(&stmt->data->msg_stmt_exec, namespace_par, mnd_str2c(stmt->data->query), FALSE);
	DBG_INF_FMT("send_request returned %s", PASS == ret? "PASS":"FAIL");

	DBG_RETURN(PASS);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::create_result_fwd */
static XMYSQLND_NODE_STMT_RESULT *
XMYSQLND_METHOD(xmysqlnd_node_stmt, create_result_fwd)(void * context)
{
	struct st_xmysqlnd_create_result_or_meta_ctx * ctx = (struct st_xmysqlnd_create_result_or_meta_ctx *) context;
	XMYSQLND_NODE_STMT_RESULT * result;
	DBG_ENTER("xmysqlnd_node_stmt::create_result_fwd");
	result = xmysqlnd_node_stmt_result_init(XMYSQLND_RESULT_FWD_ONLY, ctx->fwd_prefetch_count, ctx->stmt, ctx->stmt->persistent, &ctx->stmt->data->session->object_factory, ctx->stats, ctx->error_info);
	DBG_RETURN(result);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::create_result_buffered */
static XMYSQLND_NODE_STMT_RESULT *
XMYSQLND_METHOD(xmysqlnd_node_stmt, create_result_buffered)(void * context)
{
	struct st_xmysqlnd_create_result_or_meta_ctx * ctx = (struct st_xmysqlnd_create_result_or_meta_ctx *) context;
	XMYSQLND_NODE_STMT_RESULT * result;
	DBG_ENTER("xmysqlnd_node_stmt::create_result_buffered");
	result = xmysqlnd_node_stmt_result_init(XMYSQLND_RESULT_BUFFERED, (size_t)~0, ctx->stmt, ctx->stmt->persistent, &ctx->stmt->data->session->object_factory, ctx->stats, ctx->error_info);
	DBG_RETURN(result);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::create_meta */
static XMYSQLND_NODE_STMT_RESULT_META *
XMYSQLND_METHOD(xmysqlnd_node_stmt, create_meta)(void * context)
{
	struct st_xmysqlnd_create_result_or_meta_ctx * ctx = (struct st_xmysqlnd_create_result_or_meta_ctx *) context;
	XMYSQLND_NODE_STMT_RESULT_META * meta;
	DBG_ENTER("xmysqlnd_node_stmt::create_meta");
	meta = xmysqlnd_node_stmt_result_meta_init(ctx->stmt->persistent, &ctx->stmt->data->session->object_factory, ctx->stats, ctx->error_info);
	DBG_RETURN(meta);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::create_meta_field */
static XMYSQLND_RESULT_FIELD_META *
XMYSQLND_METHOD(xmysqlnd_node_stmt, create_meta_field)(void * context)
{
	struct st_xmysqlnd_create_result_or_meta_ctx * ctx = (struct st_xmysqlnd_create_result_or_meta_ctx *) context;
	XMYSQLND_RESULT_FIELD_META * field;
	DBG_ENTER("xmysqlnd_node_stmt::create_meta_field");
	field = xmysqlnd_result_field_meta_init(ctx->stmt->persistent, &ctx->stmt->data->session->object_factory, ctx->stats, ctx->error_info);
	DBG_RETURN(field);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::has_more_results */
static zend_bool
XMYSQLND_METHOD(xmysqlnd_node_stmt, has_more_results)(const XMYSQLND_NODE_STMT * const stmt)
{
	DBG_ENTER("xmysqlnd_node_stmt::has_more_results");
	DBG_INF_FMT("has_more=%s", stmt->data->msg_stmt_exec.has_more_results? "TRUE":"FALSE");
	DBG_RETURN(stmt->data->msg_stmt_exec.has_more_results);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::get_buffered_result */
static struct st_xmysqlnd_node_stmt_result *
XMYSQLND_METHOD(xmysqlnd_node_stmt, get_buffered_result)(XMYSQLND_NODE_STMT * const stmt, zend_bool * const has_more_results, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info)
{
	struct st_xmysqlnd_create_result_or_meta_ctx create_ctx = { stmt, stats, error_info };
	const struct st_xmysqlnd_result_create_bind create_result = { stmt->data->m.create_result_buffered, &create_ctx };
	const struct st_xmysqlnd_meta_create_bind create_meta = { stmt->data->m.create_meta, &create_ctx };
	const struct st_xmysqlnd_meta_field_create_bind create_meta_field = { stmt->data->m.create_meta_field, &create_ctx };
	DBG_ENTER("xmysqlnd_node_stmt::get_buffered_result");

	/*
	  Maybe we can inject a callbacks that creates `meta` on demand, but we still DI it.
	  This way we don't pre-create `meta` and in case of UPSERT we don't waste cycles.
	  For now, we just pre-create.
	*/
	if (FAIL == stmt->data->msg_stmt_exec.init_read(&stmt->data->msg_stmt_exec, create_result, create_meta, create_meta_field)) {
		DBG_RETURN(NULL);
	}

	stmt->data->msg_stmt_exec.read_response(&stmt->data->msg_stmt_exec, (size_t)~0, NULL);
	*has_more_results = stmt->data->msg_stmt_exec.has_more_results;
	DBG_INF_FMT("has_more=%s", *has_more_results? "TRUE":"FALSE");
	DBG_RETURN(stmt->data->msg_stmt_exec.current_result);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::get_fwd_result */
static struct st_xmysqlnd_node_stmt_result *
XMYSQLND_METHOD(xmysqlnd_node_stmt, get_fwd_result)(XMYSQLND_NODE_STMT * const stmt, const size_t rows, zend_bool * const has_more_rows_in_set, zend_bool * const has_more_results, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info)
{
	const struct st_xmysqlnd_result_create_bind create_result = { stmt->data->m.create_result_fwd, &stmt->data->read_ctx };
	const struct st_xmysqlnd_meta_create_bind create_meta = { stmt->data->m.create_meta, &stmt->data->read_ctx };
	const struct st_xmysqlnd_meta_field_create_bind create_meta_field = { stmt->data->m.create_meta_field, &stmt->data->read_ctx };
	DBG_ENTER("xmysqlnd_node_stmt::get_fwd_result");
	DBG_INF_FMT("rows="MYSQLND_LLU_SPEC, rows);

	if (FALSE == stmt->data->partial_read_started) {
		stmt->data->read_ctx.stmt = stmt;
		stmt->data->read_ctx.stats = stats;
		stmt->data->read_ctx.error_info = error_info;
		if (FAIL == stmt->data->msg_stmt_exec.init_read(&stmt->data->msg_stmt_exec, create_result, create_meta, create_meta_field)) {
			DBG_RETURN(NULL);
		}
		stmt->data->partial_read_started = TRUE;
	}
	/*
	  We can't be sure about more rows in the set, so we speculate if rows == 0.
	  If rows > 0, then we will read at least 1 row and we will be sure
	*/
	*has_more_rows_in_set = TRUE;
	*has_more_results = FALSE;

	stmt->data->read_ctx.fwd_prefetch_count = rows;

	if (rows) {
		stmt->data->msg_stmt_exec.read_response(&stmt->data->msg_stmt_exec, rows, NULL);
		*has_more_rows_in_set = stmt->data->msg_stmt_exec.has_more_rows_in_set;
		*has_more_results = stmt->data->msg_stmt_exec.has_more_results;
	}
	DBG_INF_FMT("has_more=%s", *has_more_results? "TRUE":"FALSE");
	DBG_RETURN(stmt->data->msg_stmt_exec.current_result);
}
/* }}} */



/* {{{ xmysqlnd_node_stmt::skip_one_result */
static enum_func_status
XMYSQLND_METHOD(xmysqlnd_node_stmt, skip_one_result)(XMYSQLND_NODE_STMT * const stmt, zend_bool * const has_more_results, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info)
{
	const struct st_xmysqlnd_result_create_bind create_result = { NULL, NULL };
	const struct st_xmysqlnd_meta_create_bind create_meta = { NULL, NULL };
	const struct st_xmysqlnd_meta_field_create_bind create_meta_field = { NULL, NULL };

	DBG_ENTER("xmysqlnd_node_stmt::skip_one_result");
	if (FAIL == stmt->data->msg_stmt_exec.init_read(&stmt->data->msg_stmt_exec, create_result, create_meta, create_meta_field)) {
		DBG_RETURN(FAIL);
	}

	stmt->data->msg_stmt_exec.read_response(&stmt->data->msg_stmt_exec, (size_t)~0, NULL);
	*has_more_results = stmt->data->msg_stmt_exec.has_more_results;
	DBG_INF_FMT("has_more=%s", *has_more_results? "TRUE":"FALSE");
	DBG_RETURN(PASS);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::skip_all_results */
static enum_func_status
XMYSQLND_METHOD(xmysqlnd_node_stmt, skip_all_results)(XMYSQLND_NODE_STMT * const stmt, MYSQLND_STATS * const stats, MYSQLND_ERROR_INFO * const error_info)
{
	enum_func_status ret;
	zend_bool has_more;
	DBG_ENTER("xmysqlnd_node_stmt::skip_all_results");
	do {
		ret = stmt->data->m.skip_one_result(stmt, &has_more, stats, error_info);
	} while (PASS == ret && has_more == TRUE);
	DBG_RETURN(ret);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::get_reference */
static XMYSQLND_NODE_STMT *
XMYSQLND_METHOD(xmysqlnd_node_stmt, get_reference)(XMYSQLND_NODE_STMT * const stmt)
{
	DBG_ENTER("xmysqlnd_node_stmt::get_reference");
	++stmt->data->refcount;
	DBG_INF_FMT("session=%p new_refcount=%u", stmt, stmt->data->refcount);
	DBG_RETURN(stmt);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::free_reference */
static enum_func_status
XMYSQLND_METHOD(xmysqlnd_node_stmt, free_reference)(XMYSQLND_NODE_STMT * const stmt, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info)
{
	enum_func_status ret = PASS;
	DBG_ENTER("xmysqlnd_node_stmt::free_reference");
	DBG_INF_FMT("stmt=%p old_refcount=%u", stmt, stmt->data->refcount);
	if (!(--stmt->data->refcount)) {
		stmt->data->m.dtor(stmt, stats, error_info);
	}
	DBG_RETURN(ret);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::free_contents */
static void
XMYSQLND_METHOD(xmysqlnd_node_stmt, free_contents)(XMYSQLND_NODE_STMT * const stmt)
{
	DBG_ENTER("xmysqlnd_node_stmt::free_contents");
	if (stmt->data->query.s) {
		mnd_pefree(stmt->data->query.s, stmt->data->persistent);
	}
	DBG_VOID_RETURN;
}
/* }}} */


/* {{{ xmysqlnd_node_stmt::dtor */
static void
XMYSQLND_METHOD(xmysqlnd_node_stmt, dtor)(XMYSQLND_NODE_STMT * const stmt, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info)
{
	DBG_ENTER("xmysqlnd_node_stmt::dtor");
	if (stmt) {
		stmt->data->m.free_contents(stmt);
		stmt->data->session->m->free_reference(stmt->data->session);

		mnd_pefree(stmt->data, stmt->data->persistent);
		mnd_pefree(stmt, stmt->persistent);
	}
	DBG_VOID_RETURN;
}
/* }}} */


MYSQLND_CLASS_METHODS_START(xmysqlnd_node_stmt)
	XMYSQLND_METHOD(xmysqlnd_node_stmt, init),

	XMYSQLND_METHOD(xmysqlnd_node_stmt, send_query),

	XMYSQLND_METHOD(xmysqlnd_node_stmt, has_more_results),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, get_buffered_result),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, get_fwd_result),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, skip_one_result),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, skip_all_results),

	XMYSQLND_METHOD(xmysqlnd_node_stmt, create_result_fwd),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, create_result_buffered),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, create_meta),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, create_meta_field),

	XMYSQLND_METHOD(xmysqlnd_node_stmt, get_reference),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, free_reference),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, free_contents),
	XMYSQLND_METHOD(xmysqlnd_node_stmt, dtor),
MYSQLND_CLASS_METHODS_END;


/* {{{ xmysqlnd_node_stmt_init */
PHPAPI XMYSQLND_NODE_STMT *
xmysqlnd_node_stmt_init(XMYSQLND_NODE_SESSION_DATA * session, const MYSQLND_CSTRING query, const zend_bool persistent, MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_object_factory) *object_factory,  MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info)
{
	MYSQLND_CLASS_METHODS_TYPE(xmysqlnd_object_factory) *factory = object_factory? object_factory : &MYSQLND_CLASS_METHOD_TABLE_NAME(xmysqlnd_object_factory);
	XMYSQLND_NODE_STMT * stmt = NULL;
	DBG_ENTER("xmysqlnd_node_stmt_init");
	if (query.s && query.l) {
		stmt = factory->get_node_stmt(factory, session, query, persistent, stats, error_info);	
	}
	DBG_RETURN(stmt);
}
/* }}} */


/* {{{ xmysqlnd_node_stmt_free */
PHPAPI void
xmysqlnd_node_stmt_free(XMYSQLND_NODE_STMT * const stmt, MYSQLND_STATS * stats, MYSQLND_ERROR_INFO * error_info)
{
	DBG_ENTER("xmysqlnd_node_stmt_free");
	DBG_INF_FMT("stmt=%p  stmt->data=%p  dtor=%p", stmt, stmt? stmt->data:NULL, stmt? stmt->data->m.dtor:NULL);
	if (stmt) {
		if (!stats) {
			stats = stmt->data->session->stats;
		}
		if (!error_info) {
			error_info = stmt->data->session->error_info;
		}
		stmt->data->m.free_reference(stmt, stats, error_info);
	}
	DBG_VOID_RETURN;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
