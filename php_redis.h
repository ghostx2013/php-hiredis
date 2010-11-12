/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2009 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Original author: Alfonso Jimenez <yo@alfonsojimenez.com>             |
  | Maintainer: Nicolas Favre-Felix <n.favre-felix@owlient.eu>           |
  | Maintainer: Nasreddine Bouafif <n.bouafif@owlient.eu>                |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_REDIS_H
#define PHP_REDIS_H

#include "hiredis/hiredis.h"

PHP_METHOD(HiRedis, __construct);
PHP_METHOD(HiRedis, connect);
PHP_METHOD(HiRedis, close);
PHP_METHOD(HiRedis, ping);
PHP_METHOD(HiRedis, get);
PHP_METHOD(HiRedis, set);
PHP_METHOD(HiRedis, delete);
PHP_METHOD(HiRedis, pipeline);
PHP_METHOD(HiRedis, multi);
PHP_METHOD(HiRedis, exec);
/*
PHP_METHOD(Redis, setnx);
PHP_METHOD(Redis, getSet);
PHP_METHOD(Redis, randomKey);
PHP_METHOD(Redis, renameKey);
PHP_METHOD(Redis, renameNx);
PHP_METHOD(Redis, getMultiple);
PHP_METHOD(Redis, exists);
PHP_METHOD(Redis, incr);
PHP_METHOD(Redis, decr);
PHP_METHOD(Redis, type);
PHP_METHOD(Redis, getKeys);
PHP_METHOD(Redis, sortAsc);
PHP_METHOD(Redis, sortAscAlpha);
PHP_METHOD(Redis, sortDesc);
PHP_METHOD(Redis, sortDescAlpha);
PHP_METHOD(Redis, lPush);
PHP_METHOD(Redis, rPush);
PHP_METHOD(Redis, lPop);
PHP_METHOD(Redis, rPop);
PHP_METHOD(Redis, lSize);
PHP_METHOD(Redis, lRemove);
PHP_METHOD(Redis, listTrim);
PHP_METHOD(Redis, lGet);
PHP_METHOD(Redis, lGetRange);
PHP_METHOD(Redis, lSet);
PHP_METHOD(Redis, sAdd);
PHP_METHOD(Redis, sSize);
PHP_METHOD(Redis, sRemove);
PHP_METHOD(Redis, sMove);
PHP_METHOD(Redis, sPop);
PHP_METHOD(Redis, sContains);
PHP_METHOD(Redis, sMembers);
PHP_METHOD(Redis, sInter);
PHP_METHOD(Redis, sInterStore);
PHP_METHOD(Redis, sUnion);
PHP_METHOD(Redis, sUnionStore);
PHP_METHOD(Redis, sDiff);
PHP_METHOD(Redis, sDiffStore);
PHP_METHOD(Redis, setTimeout);
PHP_METHOD(Redis, save);
PHP_METHOD(Redis, bgSave);
PHP_METHOD(Redis, lastSave);
PHP_METHOD(Redis, flushDB);
PHP_METHOD(Redis, flushAll);
PHP_METHOD(Redis, dbSize);
PHP_METHOD(Redis, auth);
PHP_METHOD(Redis, ttl);
PHP_METHOD(Redis, info);
PHP_METHOD(Redis, select);
PHP_METHOD(Redis, move);
PHP_METHOD(Redis, zAdd);
PHP_METHOD(Redis, zDelete);
PHP_METHOD(Redis, zRange);
PHP_METHOD(Redis, zReverseRange);
PHP_METHOD(Redis, zRangeByScore);
PHP_METHOD(Redis, zDeleteRangeByScore);
PHP_METHOD(Redis, zCard);
PHP_METHOD(Redis, zScore);
PHP_METHOD(Redis, zIncrBy);
PHP_METHOD(Redis, zInter);
PHP_METHOD(Redis, zUnion);
PHP_METHOD(Redis, expireAt);

PHP_METHOD(Redis, mset);
PHP_METHOD(Redis, rpoplpush);

PHP_METHOD(Redis, hGet);
PHP_METHOD(Redis, hSet);
PHP_METHOD(Redis, hDel);
PHP_METHOD(Redis, hLen);
PHP_METHOD(Redis, hKeys);
PHP_METHOD(Redis, hVals);
PHP_METHOD(Redis, hGetAll);
PHP_METHOD(Redis, hExists);
PHP_METHOD(Redis, hIncrBy);
*/

#ifdef PHP_WIN32
#define PHP_REDIS_API __declspec(dllexport)
#else
#define PHP_REDIS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(hiredis);
PHP_MSHUTDOWN_FUNCTION(hiredis);
PHP_RINIT_FUNCTION(hiredis);
PHP_RSHUTDOWN_FUNCTION(hiredis);
PHP_MINFO_FUNCTION(hiredis);

typedef enum {REDIS_MODE_BLOCKING, REDIS_MODE_PIPELINE, REDIS_MODE_TRANSACTION} redis_mode;

/* {{{ struct RedisSock */
typedef struct RedisSock_ {
	redisContext *ctx;
	redis_mode mode;
	int enqueued_commands;
} RedisSock;
/* }}} */

#define redis_sock_name "Redis Socket Buffer"

#define REDIS_SOCK_STATUS_FAILED 0
#define REDIS_SOCK_STATUS_DISCONNECTED 1
#define REDIS_SOCK_STATUS_UNKNOWN 2
#define REDIS_SOCK_STATUS_CONNECTED 3

/* properties */
#define REDIS_NOT_FOUND 0
#define REDIS_STRING 1
#define REDIS_SET 2
#define REDIS_LIST 3


/* {{{ internal function protos */
void add_constant_long(zend_class_entry *ce, char *name, int value);

PHPAPI void redis_check_eof(RedisSock *redis_sock TSRMLS_DC);
PHPAPI RedisSock* redis_sock_create(char *host, int host_len, unsigned short port, long timeout);
PHPAPI int redis_sock_connect(RedisSock *redis_sock TSRMLS_DC);
PHPAPI int redis_sock_disconnect(RedisSock *redis_sock TSRMLS_DC);
PHPAPI int redis_sock_server_open(RedisSock *redis_sock, int TSRMLS_DC);
PHPAPI char * redis_sock_read(RedisSock *redis_sock, int *buf_len TSRMLS_DC);
PHPAPI char * redis_sock_read_bulk_reply(RedisSock *redis_sock, int bytes);
PHPAPI int redis_sock_read_multibulk_reply(INTERNAL_FUNCTION_PARAMETERS, RedisSock *redis_sock TSRMLS_DC);
PHPAPI int redis_sock_write(RedisSock *redis_sock, char *cmd, size_t sz);
PHPAPI void redis_free_socket(RedisSock *redis_sock);

PHPAPI void redis_boolean_response(INTERNAL_FUNCTION_PARAMETERS, RedisSock *redis_sock TSRMLS_DC);
PHPAPI void redis_long_response(INTERNAL_FUNCTION_PARAMETERS, RedisSock *redis_sock TSRMLS_DC);
PHPAPI void redis_bulk_double_response(INTERNAL_FUNCTION_PARAMETERS, RedisSock *redis_sock TSRMLS_DC);
PHPAPI void redis_1_response(INTERNAL_FUNCTION_PARAMETERS, RedisSock *redis_sock TSRMLS_DC);
PHPAPI void redis_atomic_increment(INTERNAL_FUNCTION_PARAMETERS, char *keyword, int keyword_len TSRMLS_DC);
PHPAPI int generic_multiple_args_cmd(INTERNAL_FUNCTION_PARAMETERS, char *keyword, int keyword_len,
                int min_argc, RedisSock **redis_sock TSRMLS_DC);
PHPAPI void generic_sort_cmd(INTERNAL_FUNCTION_PARAMETERS, char *sort, int use_alpha TSRMLS_DC);
PHPAPI void generic_empty_cmd(INTERNAL_FUNCTION_PARAMETERS, char *cmd, int cmd_len TSRMLS_DC);
PHPAPI void generic_empty_long_cmd(INTERNAL_FUNCTION_PARAMETERS, char *cmd, int cmd_len TSRMLS_DC);
PHPAPI void generic_push_function(INTERNAL_FUNCTION_PARAMETERS, char *keyword, int keyword_len TSRMLS_DC);
PHPAPI void generic_pop_function(INTERNAL_FUNCTION_PARAMETERS, char *keyword, int keyword_len TSRMLS_DC);

PHPAPI void array_zip_values_and_scores(INTERNAL_FUNCTION_PARAMETERS, int use_atof TSRMLS_DC);

/* }}} */

ZEND_BEGIN_MODULE_GLOBALS(hiredis)
ZEND_END_MODULE_GLOBALS(hiredis)

	/*
#ifdef ZTS
#define REDIS_G(v) TSRMG(redis_globals_id, zend_redis_globals *, v)
#else
#define REDIS_G(v) (redis_globals.v)
#endif
*/

#define PHP_HIREDIS_VERSION "0.1"

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
