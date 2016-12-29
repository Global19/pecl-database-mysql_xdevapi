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
  | Authors: Darek Slusarczyk <marines@php.net>                          |
  +----------------------------------------------------------------------+
*/
extern "C" {
#include <php.h>
#undef ERROR
#undef inline
#include <zend_exceptions.h>
#include <ext/mysqlnd/mysqlnd.h>
#include <ext/mysqlnd/mysqlnd_debug.h>
#include <ext/mysqlnd/mysqlnd_alloc.h>
}
#include <xmysqlnd/xmysqlnd.h>
#include <xmysqlnd/xmysqlnd_node_session.h>
#include <xmysqlnd/xmysqlnd_node_schema.h>
#include <xmysqlnd/xmysqlnd_node_collection.h>
#include <xmysqlnd/xmysqlnd_node_stmt.h>
#include <xmysqlnd/xmysqlnd_index_collection_commands.h>
#include "php_mysqlx.h"
#include "mysqlx_class_properties.h"
#include "mysqlx_exception.h"
#include "mysqlx_executable.h"
#include "mysqlx_expression.h"
#include "mysqlx_node_sql_statement.h"
#include "mysqlx_node_collection__create_index.h"

static zend_class_entry *mysqlx_node_collection__create_index_class_entry;

#define DONT_ALLOW_NULL 0
#define NO_PASS_BY_REF 0

ZEND_BEGIN_ARG_INFO_EX(arginfo_mysqlx_node_collection__create_index__field, 0, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_TYPE_INFO(NO_PASS_BY_REF, doc_path, IS_STRING, DONT_ALLOW_NULL)
	ZEND_ARG_TYPE_INFO(NO_PASS_BY_REF, column_type, IS_STRING, DONT_ALLOW_NULL)
	ZEND_ARG_TYPE_INFO(NO_PASS_BY_REF, is_required, IS_LONG, DONT_ALLOW_NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mysqlx_node_collection__create_index__execute, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()


struct st_mysqlx_node_collection__create_index
{
	XMYSQLND_COLLECTION_OP__CREATE_INDEX* index_op;
	XMYSQLND_NODE_COLLECTION * collection;
};


#define MYSQLX_FETCH_NODE_COLLECTION_FROM_ZVAL(_to, _from) \
{ \
	const struct st_mysqlx_object * const mysqlx_object = Z_MYSQLX_P((_from)); \
	(_to) = (struct st_mysqlx_node_collection__create_index *) mysqlx_object->ptr; \
	if (!(_to) || !(_to)->collection) { \
		php_error_docref(NULL, E_WARNING, "invalid object of class %s", ZSTR_VAL(mysqlx_object->zo.ce->name)); \
		DBG_VOID_RETURN; \
	} \
} \


/* {{{ mysqlx_node_collection__create_index::__construct */
static
PHP_METHOD(mysqlx_node_collection__create_index, __construct)
{
}
/* }}} */


/* {{{ mysqlx_node_collection__create_index::field */
static
PHP_METHOD(mysqlx_node_collection__create_index, field)
{
	struct st_mysqlx_node_collection__create_index * object;
	zval * object_zv;
	MYSQLND_CSTRING doc_path = {NULL, 0};
	MYSQLND_CSTRING column_type = {NULL, 0};
	zend_bool is_required = FALSE;

	DBG_ENTER("mysqlx_node_collection__create_index::field");

	if (FAILURE == zend_parse_method_parameters(
		ZEND_NUM_ARGS(), getThis(), "Ossb",
		&object_zv, mysqlx_node_collection__create_index_class_entry,
		&(doc_path.s), &(doc_path.l),
		&(column_type.s), &(column_type.l),
		&is_required))
	{
		DBG_VOID_RETURN;
	}

	MYSQLX_FETCH_NODE_COLLECTION_FROM_ZVAL(object, object_zv);

	RETVAL_FALSE;

	if (object->index_op && object->collection) {
		enum_func_status ret = xmysqlnd_collection_create_index__add_field(object->index_op, doc_path, column_type, is_required);
		if (FAIL == ret) {
			static const unsigned int errcode = 10004;
			static const MYSQLND_CSTRING sqlstate = { "HY000", sizeof("HY000") - 1 };
			static const MYSQLND_CSTRING errmsg = { "Error while adding an index field", sizeof("Error while adding an index field") - 1 };
			mysqlx_new_exception(errcode, sqlstate, errmsg);
		} else {
			ZVAL_COPY(return_value, object_zv);
		}
	}

	DBG_VOID_RETURN;
}
/* }}} */


/* {{{ mysqlx_node_collection_create_index_on_error */
static const enum_hnd_func_status
mysqlx_node_collection_create_index_on_error(void * context, XMYSQLND_NODE_SESSION * session, struct st_xmysqlnd_node_stmt * const stmt, const unsigned int code, const MYSQLND_CSTRING sql_state, const MYSQLND_CSTRING message)
{
	DBG_ENTER("mysqlx_node_collection_create_index_on_error");
	mysqlx_new_exception(code, sql_state, message);
	//throw
	DBG_RETURN(HND_PASS_RETURN_FAIL);
}
/* }}} */


/* {{{ proto mixed mysqlx_node_collection__create_index::execute() */
static
PHP_METHOD(mysqlx_node_collection__create_index, execute)
{
	zend_long flags = MYSQLX_EXECUTE_FLAG_BUFFERED;
	struct st_mysqlx_node_collection__create_index * object;
	zval * object_zv;

	DBG_ENTER("mysqlx_node_collection__create_index::execute");

	if (FAILURE == zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "O|l",
												&object_zv, mysqlx_node_collection__create_index_class_entry,
												&flags))
	{
		DBG_VOID_RETURN;
	}

	MYSQLX_FETCH_NODE_COLLECTION_FROM_ZVAL(object, object_zv);

	RETVAL_FALSE;

	if (object->index_op && object->collection) {
		if (FALSE == xmysqlnd_collection_create_index__is_initialized(object->index_op)) {
			static const unsigned int errcode = 10008;
			static const MYSQLND_CSTRING sqlstate = { "HY000", sizeof("HY000") - 1 };
			static const MYSQLND_CSTRING errmsg = { "CreateIndex not completely initialized", sizeof("CreateIndex not completely initialized") - 1 };
			mysqlx_new_exception(errcode, sqlstate, errmsg);
		} else {
			const struct st_xmysqlnd_node_session_on_error_bind on_error = { mysqlx_node_collection_create_index_on_error, NULL };
			if (PASS == xmysqlnd_collection_create_index__execute(object->collection->data->schema->data->session, object->index_op, on_error)) {
				RETVAL_TRUE;
			}
		}
	}

	DBG_VOID_RETURN;
}
/* }}} */


/* {{{ mysqlx_node_collection__create_index_methods[] */
static const zend_function_entry mysqlx_node_collection__create_index_methods[] = {
	PHP_ME(mysqlx_node_collection__create_index, __construct, NULL, ZEND_ACC_PRIVATE)

	PHP_ME(mysqlx_node_collection__create_index, field, arginfo_mysqlx_node_collection__create_index__field, ZEND_ACC_PUBLIC)
	PHP_ME(mysqlx_node_collection__create_index, execute, arginfo_mysqlx_node_collection__create_index__execute, ZEND_ACC_PUBLIC)

	{NULL, NULL, NULL}
};
/* }}} */


static zend_object_handlers mysqlx_object_node_collection__create_index_handlers;
static HashTable mysqlx_node_collection__create_index_properties;

const struct st_mysqlx_property_entry mysqlx_node_collection__create_index_property_entries[] =
{
	{{NULL,	0}, NULL, NULL}
};

/* {{{ mysqlx_node_collection__create_index_free_storage */
static void
mysqlx_node_collection__create_index_free_storage(zend_object * object)
{
	struct st_mysqlx_object * mysqlx_object = mysqlx_fetch_object_from_zo(object);
	struct st_mysqlx_node_collection__create_index * inner_obj = (struct st_mysqlx_node_collection__create_index *) mysqlx_object->ptr;

	if (inner_obj) {
		if (inner_obj->collection) {
			xmysqlnd_node_collection_free(inner_obj->collection, NULL, NULL);
			inner_obj->collection = NULL;
		}
		if (inner_obj->index_op) {
			xmysqlnd_collection_create_index__destroy(inner_obj->index_op);
			inner_obj->index_op = NULL;
		}
		mnd_efree(inner_obj);
	}
	mysqlx_object_free_storage(object);
}
/* }}} */


/* {{{ php_mysqlx_node_collection__create_index_object_allocator */
static zend_object *
php_mysqlx_node_collection__create_index_object_allocator(zend_class_entry * class_type)
{
	struct st_mysqlx_object * mysqlx_object = static_cast<st_mysqlx_object *>(mnd_ecalloc(1, sizeof(struct st_mysqlx_object) + zend_object_properties_size(class_type)));
	struct st_mysqlx_node_collection__create_index * object = static_cast<st_mysqlx_node_collection__create_index *>(mnd_ecalloc(1, sizeof(struct st_mysqlx_node_collection__create_index)));

	DBG_ENTER("php_mysqlx_node_collection__create_index_object_allocator");
	if (!mysqlx_object || !object) {
		DBG_RETURN(NULL);
	}
	mysqlx_object->ptr = object;

	zend_object_std_init(&mysqlx_object->zo, class_type);
	object_properties_init(&mysqlx_object->zo, class_type);

	mysqlx_object->zo.handlers = &mysqlx_object_node_collection__create_index_handlers;
	mysqlx_object->properties = &mysqlx_node_collection__create_index_properties;


	DBG_RETURN(&mysqlx_object->zo);
}
/* }}} */

#ifdef  __cplusplus
extern "C" {
#endif

/* {{{ mysqlx_register_node_collection__create_index_class */
void
mysqlx_register_node_collection__create_index_class(INIT_FUNC_ARGS, zend_object_handlers * mysqlx_std_object_handlers)
{
	mysqlx_object_node_collection__create_index_handlers = *mysqlx_std_object_handlers;
	mysqlx_object_node_collection__create_index_handlers.free_obj = mysqlx_node_collection__create_index_free_storage;

	{
		zend_class_entry tmp_ce;
		INIT_NS_CLASS_ENTRY(tmp_ce, "mysql_xdevapi", "NodeCollectionCreateIndex", mysqlx_node_collection__create_index_methods);
		tmp_ce.create_object = php_mysqlx_node_collection__create_index_object_allocator;
		mysqlx_node_collection__create_index_class_entry = zend_register_internal_class(&tmp_ce);
		zend_class_implements(mysqlx_node_collection__create_index_class_entry, 1,
							  mysqlx_executable_interface_entry);
	}

	zend_hash_init(&mysqlx_node_collection__create_index_properties, 0, NULL, mysqlx_free_property_cb, 1);

	/* Add name + getter + setter to the hash table with the properties for the class */
	mysqlx_add_properties(&mysqlx_node_collection__create_index_properties, mysqlx_node_collection__create_index_property_entries);
}
/* }}} */


/* {{{ mysqlx_unregister_node_collection__create_index_class */
void
mysqlx_unregister_node_collection__create_index_class(SHUTDOWN_FUNC_ARGS)
{
	zend_hash_destroy(&mysqlx_node_collection__create_index_properties);
}
/* }}} */


/* {{{ mysqlx_new_node_collection__create_index */
void
mysqlx_new_node_collection__create_index(zval * return_value,
	const MYSQLND_CSTRING index_name,
	const zend_bool is_unique,
	XMYSQLND_NODE_COLLECTION * collection,
	const zend_bool clone_collection)
{
	DBG_ENTER("mysqlx_new_node_collection__create_index");
	if (SUCCESS == object_init_ex(return_value, mysqlx_node_collection__create_index_class_entry) && IS_OBJECT == Z_TYPE_P(return_value)) {
		const struct st_mysqlx_object * const mysqlx_object = Z_MYSQLX_P(return_value);
		struct st_mysqlx_node_collection__create_index * const object = (struct st_mysqlx_node_collection__create_index *) mysqlx_object->ptr;
		if (!object) {
			goto err;
		}
		object->collection = clone_collection? collection->data->m.get_reference(collection) : collection;
		object->index_op = xmysqlnd_collection_create_index__create(
			mnd_str2c(object->collection->data->schema->data->schema_name),
			mnd_str2c(object->collection->data->collection_name));
		if (!object->index_op) {
			goto err;
		}
		if (!index_name.s ||
			!index_name.l ||
			(FAIL == xmysqlnd_collection_create_index__set_index_name(object->index_op, index_name)))
		{
			goto err;
		}
		if (FAIL == xmysqlnd_collection_create_index__set_unique(object->index_op, is_unique))
		{
			goto err;
		}
		goto end;
err:
		DBG_ERR("Error");
		php_error_docref(NULL, E_WARNING, "invalid object of class %s", ZSTR_VAL(mysqlx_object->zo.ce->name));
		if (object->collection && clone_collection) {
			object->collection->data->m.free_reference(object->collection, NULL, NULL);
		}
		zval_ptr_dtor(return_value);
		ZVAL_NULL(return_value);
	}
end:
	DBG_VOID_RETURN;
}
/* }}} */

#ifdef  __cplusplus
} /* extern "C" */
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */